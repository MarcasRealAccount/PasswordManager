#pragma once

#include "Core.h"

#include <string_view>
#include <vector>

namespace Crypto {
	struct Key {
	public:
		std::uint8_t* m_Key;
	};

	void init();
	void deinit();
	bool isInitialized();

	Key* getKey(std::string_view password);
	void deleteKey(Key* key);

	void encrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output);
	void decrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output);
} // namespace Crypto
