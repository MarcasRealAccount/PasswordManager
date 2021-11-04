#include "Crypto.h"

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/kdf.h>

#include <cstdio>

#include <chrono>

namespace Crypto {
	EVP_CIPHER_CTX* cipherCtx = nullptr;
	EVP_CIPHER* aes256        = nullptr;

	EVP_KDF_CTX* scryptCtx = nullptr;

	EVP_MD_CTX* mdCtx = nullptr;
	EVP_MD* sha1      = nullptr;

	bool initialized = false;

	void init() {
		cipherCtx = EVP_CIPHER_CTX_new();
		if (!cipherCtx) {
			deinit();
			return;
		}

		aes256 = EVP_CIPHER_fetch(nullptr, "AES-256-CBC", nullptr);
		if (!aes256) {
			deinit();
			return;
		}

		EVP_KDF* scrypt = EVP_KDF_fetch(nullptr, "SCRYPT", nullptr);
		if (!scrypt) {
			deinit();
			return;
		}
		scryptCtx = EVP_KDF_CTX_new(scrypt);
		if (!scryptCtx) {
			deinit();
			return;
		}
		EVP_KDF_free(scrypt);

		mdCtx = EVP_MD_CTX_new();
		if (!mdCtx) {
			deinit();
			return;
		}

		sha1 = EVP_MD_fetch(nullptr, "SHA-1", nullptr);
		if (!sha1) {
			deinit();
			return;
		}

		if (!EVP_DigestInit_ex(mdCtx, sha1, NULL)) {
			deinit();
			return;
		}

		initialized = true;
	}

	void deinit() {
		if (sha1) {
			EVP_MD_free(sha1);
			sha1 = nullptr;
		}
		if (mdCtx) {
			EVP_MD_CTX_free(mdCtx);
			mdCtx = nullptr;
		}
		if (scryptCtx) {
			EVP_KDF_CTX_free(scryptCtx);
			scryptCtx = nullptr;
		}
		if (aes256) {
			EVP_CIPHER_free(aes256);
			aes256 = nullptr;
		}
		if (cipherCtx) {
			EVP_CIPHER_CTX_free(cipherCtx);
			cipherCtx = nullptr;
		}
		initialized = false;
	}

	bool isInitialized() {
		return initialized;
	}

	Key* getKey(std::string_view password) {
		static std::string_view salt = "SomeSaltiness";

		std::uint8_t* key = new std::uint8_t[32];

		std::uint64_t n = 1024U;
		std::uint32_t r = 8U;
		std::uint32_t p = 6U;

		OSSL_PARAM params[6];
		params[0] = OSSL_PARAM_construct_octet_string("pass", const_cast<void*>(reinterpret_cast<const void*>(password.data())), password.size());
		params[1] = OSSL_PARAM_construct_octet_string("salt", const_cast<void*>(reinterpret_cast<const void*>(salt.data())), salt.size());
		params[2] = OSSL_PARAM_construct_uint64("n", &n);
		params[3] = OSSL_PARAM_construct_uint32("r", &r);
		params[4] = OSSL_PARAM_construct_uint32("p", &p);
		params[5] = OSSL_PARAM_construct_end();

		if (EVP_KDF_derive(scryptCtx, key, 32, params) <= 0) {
			delete[] key;
			return nullptr;
		}

		Key* k         = new Key();
		k->m_Key       = key;
		k->m_KeyLength = 32;
		return k;
	}

	void deleteKey(Key* key) {
		std::memset(key->m_Key, 0, key->m_KeyLength);
		delete[] key->m_Key;
		key->m_Key       = nullptr;
		key->m_KeyLength = 0;
		delete key;
	}

	void encrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output) {
		if (!key || input.size() == 0)
			return;

		if (EVP_EncryptInit_ex2(cipherCtx, aes256, key->m_Key, nullptr, nullptr) <= 0)
			return;

		std::size_t blockSize  = EVP_CIPHER_get_block_size(aes256);
		std::size_t maxAtATime = std::numeric_limits<int>::max() - blockSize + 1;
		output.clear();
		output.resize(input.size() + blockSize - 1, '\0');

		std::size_t inOffset  = 0;
		std::size_t outOffset = 0;
		int encryptedBytes    = 0;
		while (inOffset < input.size()) {
			int inLength = static_cast<int>(std::min(input.size() - inOffset, maxAtATime));
			if (EVP_EncryptUpdate(cipherCtx, output.data() + outOffset, &encryptedBytes, input.data() + inOffset, inLength) <= 0)
				return;

			inOffset += inLength;
			outOffset += encryptedBytes;
			if (inLength != encryptedBytes)
				break;
		}

		if (EVP_EncryptFinal_ex(cipherCtx, output.data() + outOffset, &encryptedBytes) <= 0)
			return;
		outOffset += encryptedBytes;
		output.resize(outOffset);
	}

	void decrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output) {
		if (!key || input.size() == 0)
			return;

		if (EVP_DecryptInit_ex2(cipherCtx, aes256, key->m_Key, nullptr, nullptr) <= 0)
			return;

		std::size_t blockSize  = EVP_CIPHER_get_block_size(aes256);
		std::size_t maxAtATime = std::numeric_limits<int>::max() - blockSize + 1;
		output.clear();
		output.resize(input.size() + blockSize - 1, '\0');

		std::size_t inOffset  = 0;
		std::size_t outOffset = 0;
		int decryptedBytes    = 0;
		while (inOffset < input.size()) {
			int inLength = static_cast<int>(std::min(input.size() - inOffset, maxAtATime));
			if (EVP_DecryptUpdate(cipherCtx, output.data() + outOffset, &decryptedBytes, input.data() + inOffset, inLength) <= 0)
				return;

			inOffset += inLength;
			outOffset += decryptedBytes;
			if (inLength != decryptedBytes)
				break;
		}

		if (EVP_DecryptFinal_ex(cipherCtx, output.data() + outOffset, &decryptedBytes) <= 0)
			return;
		outOffset += decryptedBytes;
		output.resize(outOffset);
	}

	Key* getTOTPKey(std::string_view secret) {
		if (secret.size() > 32 || secret.size() & 0x7)
			return nullptr;

		std::uint8_t* key     = new std::uint8_t[20];
		std::size_t keyLength = 0;
		std::size_t t         = 0;
		for (std::size_t i = 0; i < secret.size(); i += 8, keyLength += 5) {
			t = 0;

			for (std::size_t j = 0; j < 8; ++j) {
				std::uint8_t c = secret[i + j];
				std::uint8_t d = 0;
				if (c >= 'a' && c <= 'z')
					c = c + 'A' - 'a';

				if (c >= 'A' && c <= 'Z') {
					d = c - 'A';
				} else if (c >= '2' && c <= '7') {
					d = c - '2' + 26;
				} else if (c == '=') {
					d = 0;
				} else {
					delete[] key;
					return nullptr;
				}
				t <<= 5;
				t |= d;
			}

			for (std::size_t j = 4; j != ~0ULL; --j) {
				key[keyLength + j] = t & 0xFF;
				t >>= 8;
			}
		}

		Key* k         = new Key();
		k->m_Key       = key;
		k->m_KeyLength = keyLength;
		return k;
	}

	void deleteTOTPKey(Key* key) {
		std::memset(key->m_Key, 0, key->m_KeyLength);
		delete[] key->m_Key;
		key->m_Key       = nullptr;
		key->m_KeyLength = 0;
		delete key;
	}

	void getTOTP(Key* key, std::uint64_t time, std::uint64_t interval, std::uint64_t digits, std::string& output) {
		// Based on https://github.com/alex14fr/mini_totp_client Thanks ;)

		if (!key || digits < 6 || digits > 10)
			return;

		static const std::uint64_t digitMods[] = { 1000000, 10000000, 100000000, 1000000000, 10000000000 };
		std::uint64_t counter                  = time / interval;

		std::uint8_t counterBytes[8];
		for (std::size_t i = 7; i != ~0ULL; --i)
			counterBytes[i] = (counter >> (8 * (7 - i))) & 0xFF;

		std::uint8_t hmacResult[20];
		HMAC(sha1, key->m_Key, key->m_KeyLength, counterBytes, 8, hmacResult, nullptr);
		std::uint8_t offset   = hmacResult[19] & 0xF;
		std::uint64_t binCode = (hmacResult[offset] & 0x7F) << 24 | (hmacResult[offset + 1] & 0xFF) << 16 | (hmacResult[offset + 2] & 0xFF) << 8 | (hmacResult[offset + 3] & 0xFF);

		output = std::to_string(binCode % digitMods[digits - 6]);
		if (output.size() > digits) {
			output = output.substr(output.size() - digits);
		} else if (output.size() < digits) {
			std::size_t toPad = digits - output.size();
			output            = std::string(toPad, '0') + output;
		}
	}
} // namespace Crypto
