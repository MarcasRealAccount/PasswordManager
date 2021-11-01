#include "WindowsCrypto.h"

#ifdef PM_IS_SYSTEM_WINDOWS

	#define NT_SUCCESS(Status) (((NTSTATUS) (Status)) >= 0)
	#define NT_STATUS_UNSUCCESSFUL ((NTSTATUS) 0xC0000001L)

	#include <algorithm>

namespace Crypto {
	static BCRYPT_ALG_HANDLE aesAlg = nullptr;
	static BCRYPT_ALG_HANDLE md5Alg = nullptr;

	void init() {
		if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&aesAlg, BCRYPT_AES_ALGORITHM, nullptr, 0)))
			return;
		if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&md5Alg, BCRYPT_MD5_ALGORITHM, nullptr, 0)))
			return;
	}

	void deinit() {
		BCryptCloseAlgorithmProvider(md5Alg, 0);
		BCryptCloseAlgorithmProvider(aesAlg, 0);
	}

	Key* getKey(std::string_view password) {
		DWORD hashLength = 0;
		DWORD result     = 0;
		if (!NT_SUCCESS(BCryptGetProperty(md5Alg, BCRYPT_HASH_LENGTH, reinterpret_cast<PUCHAR>(&hashLength), sizeof(hashLength), &result, 0)))
			return nullptr;

		std::uint8_t* secret = new std::uint8_t[hashLength];

		const char* pw  = password.data();
		PUCHAR fuckNTPW = reinterpret_cast<PUCHAR>(const_cast<char*>(pw));

		if (!NT_SUCCESS(BCryptHash(md5Alg, nullptr, 0, fuckNTPW, static_cast<ULONG>(password.size()), secret, hashLength)))
			return nullptr;

		BCRYPT_KEY_HANDLE key = nullptr;
		if (!NT_SUCCESS(BCryptGenerateSymmetricKey(aesAlg, &key, nullptr, 0, secret, hashLength, 0)))
			return nullptr;

		delete[] secret;

		Key* k   = new Key();
		k->m_Key = key;
		return k;
	}

	void deleteKey(Key* key) {
		BCryptDestroyKey(key->m_Key);
		delete key;
	}

	void encrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output) {
		const std::uint8_t* in = input.data();
		PUCHAR fuckNTIn        = reinterpret_cast<PUCHAR>(const_cast<std::uint8_t*>(in));

		ULONG outputSize = 0;
		if (!NT_SUCCESS(BCryptEncrypt(key->m_Key, fuckNTIn, static_cast<ULONG>(input.size()), nullptr, nullptr, 0, nullptr, 0, &outputSize, BCRYPT_BLOCK_PADDING)))
			return;

		output.resize(static_cast<std::size_t>(outputSize));

		if (!NT_SUCCESS(BCryptEncrypt(key->m_Key, fuckNTIn, static_cast<ULONG>(input.size()), nullptr, nullptr, 0, reinterpret_cast<PUCHAR>(output.data()), static_cast<ULONG>(output.size()), &outputSize, BCRYPT_BLOCK_PADDING)))
			return;
	}

	void decrypt(Key* key, const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output) {
		const std::uint8_t* in = input.data();
		PUCHAR fuckNTIn        = reinterpret_cast<PUCHAR>(const_cast<std::uint8_t*>(in));

		ULONG outputSize = 0;
		if (!NT_SUCCESS(BCryptDecrypt(key->m_Key, fuckNTIn, static_cast<ULONG>(input.size()), nullptr, nullptr, 0, nullptr, 0, &outputSize, BCRYPT_BLOCK_PADDING)))
			return;

		output.resize(static_cast<std::size_t>(outputSize));

		if (!NT_SUCCESS(BCryptDecrypt(key->m_Key, fuckNTIn, static_cast<ULONG>(input.size()), nullptr, nullptr, 0, reinterpret_cast<PUCHAR>(output.data()), static_cast<ULONG>(output.size()), &outputSize, BCRYPT_BLOCK_PADDING)))
			return;
	}
} // namespace Crypto

#endif