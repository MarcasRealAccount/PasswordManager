#pragma once

#include "Core.h"

#include <string>
#include <string_view>
#include <vector>

namespace Crypto {
	struct Key {
	public:
		std::uint8_t* m_Key     = nullptr;
		std::size_t m_KeyLength = 0;
	};

	void init();
	void deinit();
	bool isInitialized();

	Key* getKey(std::string_view password);
	void deleteKey(Key* key);

	void encrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output);
	void decrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output);

	Key* getTOTPKey(std::string_view secret);
	void deleteTOTPKey(Key* key);
	void getTOTP(Key* key, std::uint64_t time, std::uint64_t interval, std::uint64_t digits, std::string& output);
} // namespace Crypto
