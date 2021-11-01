#pragma once

#include <cstdint>

#include <string>
#include <vector>

struct ByteBuffer {
public:
	ByteBuffer();
	ByteBuffer(const std::vector<std::uint8_t>& bytes);
	ByteBuffer(std::vector<std::uint8_t>&& bytes);

	void setOffset(std::size_t offset) { m_Offset = offset; }
	std::size_t getOffset() const { return m_Offset; }

	auto& bytes() { return m_Bytes; }
	auto& bytes() const { return m_Bytes; }

	void setUI1(std::size_t position, std::uint8_t value);

	void setUI2LE(std::size_t position, std::uint16_t value);
	void setUI2BE(std::size_t position, std::uint16_t value);

	void setUI2(std::size_t position, std::uint16_t value) { return setUI2LE(position, value); }

	void setUI4LE(std::size_t position, std::uint32_t value);
	void setUI4BE(std::size_t position, std::uint32_t value);

	void setUI4(std::size_t position, std::uint32_t value) { return setUI4LE(position, value); }

	void setUI8LE(std::size_t position, std::uint64_t value);
	void setUI8BE(std::size_t position, std::uint64_t value);

	void setUI8(std::size_t position, std::uint64_t value) { return setUI8LE(position, value); }

	void setStringNT(std::size_t position, std::string_view string);
	void setString(std::size_t position, std::string_view string);

	void setBytes(std::size_t position, const std::vector<std::uint8_t>& bytes);

	void addUI1(std::size_t position, std::uint8_t value);
	void addUI1(std::uint8_t value) { addUI1(m_Offset++, value); }

	void addUI2LE(std::size_t position, std::uint16_t value);
	void addUI2BE(std::size_t position, std::uint16_t value);
	void addUI2LE(std::uint16_t value);
	void addUI2BE(std::uint16_t value);

	void addUI4LE(std::size_t position, std::uint32_t value);
	void addUI4BE(std::size_t position, std::uint32_t value);
	void addUI4LE(std::uint32_t value);
	void addUI4BE(std::uint32_t value);

	void addUI8LE(std::size_t position, std::uint64_t value);
	void addUI8BE(std::size_t position, std::uint64_t value);
	void addUI8LE(std::uint64_t value);
	void addUI8BE(std::uint64_t value);

	void addUI2(std::size_t position, std::uint16_t value) { addUI2LE(position, value); }
	void addUI4(std::size_t position, std::uint32_t value) { addUI4LE(position, value); }
	void addUI8(std::size_t position, std::uint64_t value) { addUI8LE(position, value); }
	void addUI2(std::uint16_t value) { addUI2LE(value); }
	void addUI4(std::uint32_t value) { addUI4LE(value); }
	void addUI8(std::uint64_t value) { addUI8LE(value); }

	void addStringNT(std::size_t position, std::string_view string);
	void addString(std::size_t position, std::string_view string);
	void addStringNT(std::string_view string);
	void addString(std::string_view string);

	void addBytes(std::size_t position, const std::vector<std::uint8_t>& bytes);
	void addBytes(const std::vector<std::uint8_t>& bytes);

	std::uint8_t getUI1(std::size_t position) const { return position < m_Bytes.size() ? m_Bytes[position] : 0; }
	std::uint8_t getUI1() { return getUI1(m_Offset++); }

	std::uint16_t getUI2LE(std::size_t position) const;
	std::uint16_t getUI2BE(std::size_t position) const;
	std::uint16_t getUI2LE();
	std::uint16_t getUI2BE();

	std::uint32_t getUI4LE(std::size_t position) const;
	std::uint32_t getUI4BE(std::size_t position) const;
	std::uint32_t getUI4LE();
	std::uint32_t getUI4BE();

	std::uint64_t getUI8LE(std::size_t position) const;
	std::uint64_t getUI8BE(std::size_t position) const;
	std::uint64_t getUI8LE();
	std::uint64_t getUI8BE();

	std::uint16_t getUI2(std::size_t position) const { return getUI2LE(position); }
	std::uint32_t getUI4(std::size_t position) const { return getUI4LE(position); }
	std::uint64_t getUI8(std::size_t position) const { return getUI8LE(position); }
	std::uint16_t getUI2() { return getUI2LE(); }
	std::uint32_t getUI4() { return getUI4LE(); }
	std::uint64_t getUI8() { return getUI8LE(); }

	std::string getString(std::size_t position, std::size_t length) const;
	std::string getStringNT(std::size_t position) const;
	std::string getString(std::size_t length);
	std::string getStringNT();

	std::vector<std::uint8_t> getBytes(std::size_t position, std::size_t length) const;
	std::vector<std::uint8_t> getBytes(std::size_t length);

private:
	std::size_t m_Offset = 0;
	std::vector<std::uint8_t> m_Bytes;
};