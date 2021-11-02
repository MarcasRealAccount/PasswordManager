#include "Crypto.h"

#include <openssl/evp.h>
#include <openssl/bio.h>

namespace Crypto {
EVP_MD_CONTEXT* mdCtx=nullptr;
EVP_MD *sha256 = nullptr;

bool initialized = false;

	void init() {
		mdCtx = EVP_MD_CTX_new();
		if (!mdCtx)
			return;
		
		sha256 = EVP_MD_fetch(NULL, "SHA256", NULL);
		if (!sha256)
			return;
		
		if (!EVP_DigestInit_ex(ctx, sha256, NULL))
			return;
		
		initialized = true;
	}

	void deinit() {
		EVP_MD_free(sha256);
		EVP_MD_CTX_free(mdCtx);
		sha256 = nullptr;
		mdCtx = nullptr;
		initialized = false;
	}

bool isInitialized() {
	return initialized;
}

	Key* getKey(std::string_view password) {
		if (!EVP_DigestUpdate(ctx, password.data(), password.size()))
			return;
		
		std::size_t mdLength = EVP_MD_get_size(sha256);
		std::uint8_t* md = new std::uint8_t[mdLength];
		
		if (!EVP_DigestFinal_ex(ctx, md, &mdLength))
			return;
		
		delete[] md;
		
		Key* k = new Key();

		return k;
	}

	void deleteKey(Key* key) {
		delete key;
	}

	void encrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output) {
	}

	void decrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output) {
	}
} // namespace Crypto
