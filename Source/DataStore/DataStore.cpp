#include "DataStore.h"
#include "ByteBuffer/ByteBuffer.h"

#include <fstream>
#include <unordered_map>

DataStore::DataStore()
    : m_Rng(std::random_device()()) { }

std::uint64_t DataStore::addElement(const std::string& name, const std::vector<std::uint8_t>& data) {
	std::uint64_t id = m_Rng();
	while (getElement(id))
		id = m_Rng();

	m_Elements.insert({ id, { name, data } });
	return id;
}

DataElement* DataStore::getElement(std::uint64_t id) {
	if (id == 0)
		return nullptr;
	auto itr = m_Elements.find(id);
	return itr != m_Elements.end() ? &itr->second : nullptr;
}

void DataStore::deserialize(Crypto::Key* key, std::filesystem::path filepath) {
	std::ifstream file(filepath, std::ios::ate | std::ios::binary);
	if (!file.is_open())
		return;

	ByteBuffer decryptedData;
	{
		std::vector<std::uint8_t> data = std::vector<std::uint8_t>(file.tellg());
		file.seekg(0);
		file.read(reinterpret_cast<char*>(data.data()), data.size());
		file.close();

		Crypto::decrypt(key, data, decryptedData.bytes());
	}

	auto namesBegin   = decryptedData.getUI8();
	auto datasBegin   = decryptedData.getUI8();
	auto elementCount = decryptedData.getUI8();
	m_Elements.clear();
	m_Elements.reserve(elementCount);
	for (std::uint64_t i = 0; i < elementCount; ++i) {
		auto nameOffset = decryptedData.getUI8();
		auto dataOffset = decryptedData.getUI8();
		auto dataLength = decryptedData.getUI8();
		auto id         = decryptedData.getUI8();

		auto name = decryptedData.getStringNT(namesBegin + nameOffset);
		auto data = decryptedData.getBytes(datasBegin + dataOffset, dataLength);

		m_Elements.insert({ id, { name, data } });
	}
}

void DataStore::serialize(Crypto::Key* key, std::filesystem::path filepath) {
	std::ofstream file(filepath, std::ios::binary);
	if (!file.is_open())
		return;

	std::unordered_map<std::string, std::uint64_t> namesMap;
	std::uint64_t nameOffset = 0;
	for (auto& element : m_Elements) {
		auto& name = element.second.m_Name;
		auto itr   = namesMap.find(name);
		if (itr != namesMap.end())
			continue;

		namesMap.insert({ name, nameOffset });
		nameOffset += name.size() + 1;
	}

	ByteBuffer bytes;
	bytes.addUI8(0);                 // NamesBegin
	bytes.addUI8(0);                 // DatasBegin
	bytes.addUI8(m_Elements.size()); // ElementCount
	std::uint64_t dataOffset = 0;
	for (auto& element : m_Elements) {
		auto itr = namesMap.find(element.second.m_Name);
		if (itr == namesMap.end())
			throw std::runtime_error("FUCKKKKKKK UUUU!");

		bytes.addUI8(itr->second);                  // NameOffset;
		bytes.addUI8(dataOffset);                   // DataOffset;
		bytes.addUI8(element.second.m_Data.size()); // DataLength;
		bytes.addUI8(element.first);                // Id;

		dataOffset += element.second.m_Data.size();
	}
	std::uint64_t namesBegin = bytes.getOffset();
	bytes.setUI8(0, namesBegin);
	for (auto& name : namesMap)
		bytes.addStringNT(namesBegin + name.second, name.first);
	bytes.setOffset(namesBegin + nameOffset);
	std::uint64_t datasBegin = bytes.getOffset();
	bytes.setUI8(8, datasBegin);
	for (auto& element : m_Elements)
		bytes.addBytes(element.second.m_Data);

	std::vector<std::uint8_t> encryptedData;
	Crypto::encrypt(key, bytes.bytes(), encryptedData);

	file.write(reinterpret_cast<char*>(encryptedData.data()), encryptedData.size());
	file.close();
}