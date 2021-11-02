#include "ByteBuffer.h"

#include <utility>

ByteBuffer::ByteBuffer()
    : m_Bytes({}) { }

ByteBuffer::ByteBuffer(const std::vector<std::uint8_t>& bytes)
    : m_Bytes(bytes) { }

ByteBuffer::ByteBuffer(std::vector<std::uint8_t>&& bytes)
    : m_Bytes(std::move(bytes)) { }

void ByteBuffer::setUI1(std::size_t position, std::uint8_t value) {
	if (position < m_Bytes.size())
		m_Bytes[position] = value;
}

void ByteBuffer::setUI2LE(std::size_t position, std::uint16_t value) {
	setUI1(position, value & 0xFF);
	setUI1(position + 1, (value >> 8) & 0xFF);
}

void ByteBuffer::setUI2BE(std::size_t position, std::uint16_t value) {
	setUI1(position, (value >> 8) & 0xFF);
	setUI1(position + 1, value & 0xFF);
}

void ByteBuffer::setUI4LE(std::size_t position, std::uint32_t value) {
	setUI1(position, value & 0xFF);
	setUI1(position + 1, (value >> 8) & 0xFF);
	setUI1(position + 2, (value >> 16) & 0xFF);
	setUI1(position + 3, (value >> 24) & 0xFF);
}

void ByteBuffer::setUI4BE(std::size_t position, std::uint32_t value) {
	setUI1(position, (value >> 24) & 0xFF);
	setUI1(position + 1, (value >> 16) & 0xFF);
	setUI1(position + 2, (value >> 8) & 0xFF);
	setUI1(position + 3, value & 0xFF);
}

void ByteBuffer::setUI8LE(std::size_t position, std::uint64_t value) {
	setUI1(position, value & 0xFF);
	setUI1(position + 1, (value >> 8) & 0xFF);
	setUI1(position + 2, (value >> 16) & 0xFF);
	setUI1(position + 3, (value >> 24) & 0xFF);
	setUI1(position + 4, (value >> 32) & 0xFF);
	setUI1(position + 5, (value >> 40) & 0xFF);
	setUI1(position + 6, (value >> 48) & 0xFF);
	setUI1(position + 7, (value >> 56) & 0xFF);
}

void ByteBuffer::setUI8BE(std::size_t position, std::uint64_t value) {
	setUI1(position, (value >> 56) & 0xFF);
	setUI1(position + 1, (value >> 48) & 0xFF);
	setUI1(position + 2, (value >> 40) & 0xFF);
	setUI1(position + 3, (value >> 32) & 0xFF);
	setUI1(position + 4, (value >> 24) & 0xFF);
	setUI1(position + 5, (value >> 16) & 0xFF);
	setUI1(position + 6, (value >> 8) & 0xFF);
	setUI1(position + 7, value & 0xFF);
}

void ByteBuffer::setStringNT(std::size_t position, std::string_view string) {
	std::size_t actualLength = std::min(m_Bytes.size() - position, string.size() + 1);
	std::memcpy(m_Bytes.data() + position, string.data(), string.size() - 1);
	m_Bytes[position + actualLength] = '\0';
}

void ByteBuffer::setString(std::size_t position, std::string_view string) {
	std::size_t actualLength = std::min(m_Bytes.size() - position, string.size());
	std::memcpy(m_Bytes.data() + position, string.data(), actualLength);
}

void ByteBuffer::setBytes(std::size_t position, const std::vector<std::uint8_t>& bytes) {
	std::size_t actualLength = std::min(m_Bytes.size() - position, bytes.size());
	std::memcpy(m_Bytes.data() + position, bytes.data(), actualLength);
}

void ByteBuffer::addUI1(std::size_t position, std::uint8_t value) {
	if (position >= m_Bytes.size())
		m_Bytes.push_back(value);
	else
		m_Bytes.insert(m_Bytes.begin() + position, value);
}

void ByteBuffer::addUI2LE(std::size_t position, std::uint16_t value) {
	addUI1(position, value & 0xFF);
	addUI1(position + 1, (value >> 8) & 0xFF);
}

void ByteBuffer::addUI2BE(std::size_t position, std::uint16_t value) {
	addUI1(position, (value >> 8) & 0xFF);
	addUI1(position + 1, value & 0xFF);
}

void ByteBuffer::addUI2LE(std::uint16_t value) {
	addUI2LE(m_Offset, value);
	m_Offset += 2;
}

void ByteBuffer::addUI2BE(std::uint16_t value) {
	addUI2BE(m_Offset, value);
	m_Offset += 2;
}

void ByteBuffer::addUI4LE(std::size_t position, std::uint32_t value) {
	addUI1(position, value & 0xFF);
	addUI1(position + 1, (value >> 8) & 0xFF);
	addUI1(position + 2, (value >> 16) & 0xFF);
	addUI1(position + 4, (value >> 24) & 0xFF);
}

void ByteBuffer::addUI4BE(std::size_t position, std::uint32_t value) {
	addUI1(position, (value >> 24) & 0xFF);
	addUI1(position + 1, (value >> 16) & 0xFF);
	addUI1(position + 2, (value >> 8) & 0xFF);
	addUI1(position + 3, value & 0xFF);
}

void ByteBuffer::addUI4LE(std::uint32_t value) {
	addUI4LE(m_Offset, value);
	m_Offset += 4;
}

void ByteBuffer::addUI4BE(std::uint32_t value) {
	addUI4BE(m_Offset, value);
	m_Offset += 4;
}

void ByteBuffer::addUI8LE(std::size_t position, std::uint64_t value) {
	addUI1(position, value & 0xFF);
	addUI1(position + 1, (value >> 8) & 0xFF);
	addUI1(position + 2, (value >> 16) & 0xFF);
	addUI1(position + 4, (value >> 24) & 0xFF);
	addUI1(position + 5, (value >> 32) & 0xFF);
	addUI1(position + 6, (value >> 40) & 0xFF);
	addUI1(position + 7, (value >> 48) & 0xFF);
	addUI1(position + 8, (value >> 56) & 0xFF);
}

void ByteBuffer::addUI8BE(std::size_t position, std::uint64_t value) {
	addUI1(position, (value >> 56) & 0xFF);
	addUI1(position + 1, (value >> 48) & 0xFF);
	addUI1(position + 2, (value >> 40) & 0xFF);
	addUI1(position + 3, (value >> 32) & 0xFF);
	addUI1(position + 4, (value >> 24) & 0xFF);
	addUI1(position + 5, (value >> 16) & 0xFF);
	addUI1(position + 6, (value >> 8) & 0xFF);
	addUI1(position + 7, value & 0xFF);
}

void ByteBuffer::addUI8LE(std::uint64_t value) {
	addUI8LE(m_Offset, value);
	m_Offset += 8;
}

void ByteBuffer::addUI8BE(std::uint64_t value) {
	addUI8BE(m_Offset, value);
	m_Offset += 8;
}

void ByteBuffer::addStringNT(std::size_t position, std::string_view string) {
	if (1 + position + string.size() > m_Bytes.size())
		m_Bytes.resize(1 + position + string.size());
	std::memcpy(m_Bytes.data() + position, string.data(), string.size());
	m_Bytes[position + string.size()] = '\0';
}

void ByteBuffer::addString(std::size_t position, std::string_view string) {
	if (position + string.size() > m_Bytes.size())
		m_Bytes.resize(position + string.size());
	std::memcpy(m_Bytes.data() + position, string.data(), string.size());
}

void ByteBuffer::addStringNT(std::string_view string) {
	addStringNT(m_Offset, string);
	m_Offset += string.size() + 1;
}

void ByteBuffer::addString(std::string_view string) {
	addString(m_Offset, string);
	m_Offset += string.size();
}

void ByteBuffer::addBytes(std::size_t position, const std::vector<std::uint8_t>& bytes) {
	if (position + bytes.size() > m_Bytes.size())
		m_Bytes.resize(position + bytes.size());
	std::memcpy(m_Bytes.data() + position, bytes.data(), bytes.size());
}

void ByteBuffer::addBytes(const std::vector<std::uint8_t>& bytes) {
	addBytes(m_Offset, bytes);
	m_Offset += bytes.size();
}

std::uint16_t ByteBuffer::getUI2LE(std::size_t position) const {
	return getUI1(position) | getUI1(position + 1) << 8;
}

std::uint16_t ByteBuffer::getUI2BE(std::size_t position) const {
	return getUI1(position) << 8 | getUI1(position + 1);
}

std::uint16_t ByteBuffer::getUI2LE() {
	auto value = getUI2LE(m_Offset);
	m_Offset += 2;
	return value;
}

std::uint16_t ByteBuffer::getUI2BE() {
	auto value = getUI2BE(m_Offset);
	m_Offset += 2;
	return value;
}

std::uint32_t ByteBuffer::getUI4LE(std::size_t position) const {
	return getUI2LE(position) | getUI2LE(position + 2) << 16;
}

std::uint32_t ByteBuffer::getUI4BE(std::size_t position) const {
	return getUI2BE(position) << 16 | getUI2BE(position + 2);
}

std::uint32_t ByteBuffer::getUI4LE() {
	auto value = getUI4LE(m_Offset);
	m_Offset += 4;
	return value;
}

std::uint32_t ByteBuffer::getUI4BE() {
	auto value = getUI4BE(m_Offset);
	m_Offset += 4;
	return value;
}

std::uint64_t ByteBuffer::getUI8LE(std::size_t position) const {
	return static_cast<std::uint64_t>(getUI4LE(position)) | static_cast<std::uint64_t>(getUI4LE(position + 4)) << 32;
}

std::uint64_t ByteBuffer::getUI8BE(std::size_t position) const {
	return static_cast<std::uint64_t>(getUI4BE(position)) << 32 | static_cast<std::uint64_t>(getUI4BE(position + 4));
}

std::uint64_t ByteBuffer::getUI8LE() {
	auto value = getUI8LE(m_Offset);
	m_Offset += 8;
	return value;
}

std::uint64_t ByteBuffer::getUI8BE() {
	auto value = getUI8BE(m_Offset);
	m_Offset += 8;
	return value;
}

std::string ByteBuffer::getString(std::size_t position, std::size_t length) const {
	auto start = m_Bytes.begin() + position;
	auto end   = start + length;
	return std::string(start, end);
}

std::string ByteBuffer::getStringNT(std::size_t position) const {
	std::size_t length = 0;
	for (std::size_t i = position; i < m_Bytes.size() && m_Bytes[i] != '\0'; ++i, ++length)
		;
	return getString(position, length);
}

std::string ByteBuffer::getString(std::size_t length) {
	auto str = getString(m_Offset, length);
	m_Offset += str.size();
	return str;
}

std::string ByteBuffer::getStringNT() {
	auto str = getString(m_Offset);
	m_Offset += str.size();
	return str;
}

std::vector<std::uint8_t> ByteBuffer::getBytes(std::size_t position, std::size_t length) const {
	auto start = m_Bytes.begin() + position;
	auto end   = start + length;
	return std::vector<std::uint8_t>(start, end);
}

std::vector<std::uint8_t> ByteBuffer::getBytes(std::size_t length) {
	auto bytes = getBytes(m_Offset, length);
	m_Offset += bytes.size();
	return bytes;
}
