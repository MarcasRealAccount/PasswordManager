#pragma once

#include "Crypto.h"

#include <cstdint>

#include <filesystem>
#include <random>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct DataElement {
	std::string m_Name;
	std::vector<std::uint8_t> m_Data;
};

class DataStore {
public:
	DataStore();

	auto& getElements() const { return m_Elements; }

	std::uint64_t addElement(const std::string& name, const std::vector<std::uint8_t>& data);
	DataElement* getElement(std::uint64_t id);

	void deserialize(Crypto::Key* key, std::filesystem::path filepath);
	void serialize(Crypto::Key* key, std::filesystem::path filepath);

private:
	std::mt19937_64 m_Rng;
	std::unordered_map<std::uint64_t, DataElement> m_Elements;
};