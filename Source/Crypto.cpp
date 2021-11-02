#include "Crypto.h"

#include <openssl/evp.h>
#include <openssl/kdf.h>

#include <iostream>

namespace Crypto {
	EVP_CIPHER_CTX* cipherCtx = nullptr;
	EVP_CIPHER* aes256        = nullptr;

	EVP_KDF_CTX* scryptCtx = nullptr;

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

		initialized = true;
	}

	void deinit() {
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

		Key* k   = new Key();
		k->m_Key = key;
		return k;
	}

	void deleteKey(Key* key) {
		delete[] key->m_Key;
		delete key;
	}

	void encrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output) {
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
} // namespace Crypto
