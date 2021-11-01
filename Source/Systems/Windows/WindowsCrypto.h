#pragma once

#include "Core.h"

#include "Crypto.h"

#ifdef PM_IS_SYSTEM_WINDOWS
	#include <Windows.h>
	#include <bcrypt.h>

namespace Crypto {
	struct Key {
	public:
		BCRYPT_KEY_HANDLE m_Key = nullptr;
	};
} // namespace Crypto
#endif