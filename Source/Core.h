#pragma once

#include "Flags.h"

#include <cstdint>
#include <sstream>

#define PM_IS_FLAG(FLAGS, FLAG) ((FLAGS & FLAG) == FLAG)

#define PM_CONFIG_UNKNOWN 0
#define PM_CONFIG_DEBUG 1
#define PM_CONFIG_RELEASE 2
#define PM_CONFIG_DIST 3

#define PM_SYSTEM_UNKNOWN 0
#define PM_SYSTEM_WINDOWS 1
#define PM_SYSTEM_MACOSX 2
#define PM_SYSTEM_LINUX 3

#define PM_TOOLSET_UNKNOWN 0
#define PM_TOOLSET_MSVC 1
#define PM_TOOLSET_CLANG 2
#define PM_TOOLSET_GCC 3

#define PM_PLATFORM_UNKNOWN 0
#define PM_PLATFORM_X86 1
#define PM_PLATFORM_AMD64 2

#define PM_IS_CONFIG_DEBUG PM_IS_FLAG(PM_CONFIG, PM_CONFIG_DEBUG)
#define PM_IS_CONFIG_DIST PM_IS_FLAG(PM_CONFIG, PM_CONFIG_DIST)

#define PM_IS_SYSTEM_WINDOWS PM_IS_FLAG(PM_SYSTEM, PM_SYSTEM_WINDOWS)
#define PM_IS_SYSTEM_MACOSX PM_IS_FLAG(PM_SYSTEM, PM_SYSTEM_MACOSX)
#define PM_IS_SYSTEM_LINUX PM_IS_FLAG(PM_SYSTEM, PM_SYSTEM_LINUX)

#define PM_IS_TOOLSET_MSVC PM_IS_FLAG(PM_TOOLSET, PM_TOOLSET_MSVC)
#define PM_IS_TOOLSET_CLANG PM_IS_FLAG(PM_TOOLSET, PM_TOOLSET_CLANG)
#define PM_IS_TOOLSET_GCC PM_IS_FLAG(PM_TOOLSET, PM_TOOLSET_GCC)

#define PM_IS_PLATFORM_X86 PM_IS_FLAG(PM_PLATFORM, PM_PLATFORM_X86)
#define PM_IS_PLATFORM_AMD64 PM_IS_FLAG(PM_PLATFORM, PM_PLATFORM_AMD64)

namespace Core {
	struct EPMConfigFlags : public Flags<std::uint16_t> {
	public:
		using Flags::Flags;
		using Flags::operator=;

		friend std::ostream& operator<<(std::ostream& stream, const EPMConfigFlags& flags);
	};

	struct EPMSystemFlags : public Flags<std::uint16_t> {
	public:
		using Flags::Flags;
		using Flags::operator=;

		friend std::ostream& operator<<(std::ostream& stream, const EPMSystemFlags& flags);
	};

	struct EPMToolsetFlags : public Flags<std::uint16_t> {
	public:
		using Flags::Flags;
		using Flags::operator=;

		friend std::ostream& operator<<(std::ostream& stream, const EPMToolsetFlags& flags);
	};

	struct EPMPlatformFlags : public Flags<std::uint16_t> {
	public:
		using Flags::Flags;
		using Flags::operator=;

		friend std::ostream& operator<<(std::ostream& stream, const EPMPlatformFlags& flags);
	};

	namespace EPMConfigFlag {
		static constexpr EPMConfigFlags Unknown = 0;
		static constexpr EPMConfigFlags Debug   = 1;
		static constexpr EPMConfigFlags Dist    = 2;
	} // namespace EPMConfigFlag

	namespace EPMSystemFlag {
		static constexpr EPMSystemFlags Unknown = 0;
		static constexpr EPMSystemFlags Windows = 1;
		static constexpr EPMSystemFlags Macosx  = 2;
		static constexpr EPMSystemFlags Linux   = 4;
		static constexpr EPMSystemFlags Unix    = 8;
	} // namespace EPMSystemFlag

	namespace EPMToolsetFlag {
		static constexpr EPMToolsetFlags Unknown = 0;
		static constexpr EPMToolsetFlags MSVC    = 1;
		static constexpr EPMToolsetFlags Clang   = 2;
		static constexpr EPMToolsetFlags GCC     = 4;
	} // namespace EPMToolsetFlag

	namespace EPMPlatformFlag {
		static constexpr EPMPlatformFlags Unknown = 0;
		static constexpr EPMPlatformFlags X86     = 1;
		static constexpr EPMPlatformFlags AMD64   = 2;
	} // namespace EPMPlatformFlag

#if PM_IS_CONFIG_DEBUG
	static constexpr EPMConfigFlags s_Config = EPMConfigFlag::Debug;
	static constexpr bool s_IsConfigDebug    = true;
	static constexpr bool s_IsConfigDist     = false;
#elif PM_IS_CONFIG_RELEASE
	static constexpr EPMConfigFlags s_Config     = EPMConfigFlag::Debug | EPMConfigFlags::Dist;
	static constexpr bool s_IsConfigDebug        = true;
	static constexpr bool s_IsConfigDist         = true;
#elif PM_IS_CONFIG_DIST
	static constexpr EPMConfigFlags s_Config     = EPMConfigFlag::Dist;
	static constexpr bool s_IsConfigDebug        = false;
	static constexpr bool s_IsConfigDist         = true;
#else
	static constexpr EPMConfigFlags s_Config   = EPMConfigFlag::Unknown;
	static constexpr bool s_IsConfigDebug      = false;
	static constexpr bool s_IsConfigDist       = false;
#endif

#if PM_IS_SYSTEM_WINDOWS
	static constexpr EPMSystemFlags s_System = EPMSystemFlag::Windows;
	static constexpr bool s_IsSystemWindows  = true;
	static constexpr bool s_IsSystemMacosx   = false;
	static constexpr bool s_IsSystemLinux    = false;
	static constexpr bool s_IsSystemUnix     = false;
#elif PM_IS_SYSTEM_MACOSX
	static constexpr EPMSystemFlags s_System     = EPMSystemFlag::Macosx | EPMSystemFlag::Unix;
	static constexpr bool s_IsSystemWindows      = false;
	static constexpr bool s_IsSystemMacosx       = true;
	static constexpr bool s_IsSystemLinux        = false;
	static constexpr bool s_IsSystemUnix         = true;
#elif PM_IS_SYSTEM_LINUX
	static constexpr EPMSystemFlags s_System     = EPMSystemFlag::Linux | EPMSystemFlag::Unix;
	static constexpr bool s_IsSystemWindows      = false;
	static constexpr bool s_IsSystemMacosx       = false;
	static constexpr bool s_IsSystemLinux        = true;
	static constexpr bool s_IsSystemUnix         = true;
#else
	static constexpr EPMSystemFlags s_System   = EPMSystemFlag::Unknown;
	static constexpr bool s_IsSystemWindows    = false;
	static constexpr bool s_IsSystemMacosx     = false;
	static constexpr bool s_IsSystemLinux      = false;
	static constexpr bool s_IsSystemUnix       = false;
#endif

#if PM_IS_TOOLSET_MSVC
	static constexpr EPMToolsetFlags s_Toolset = EPMToolsetFlag::MSVC;
	static constexpr bool s_IsToolsetMSVC      = true;
	static constexpr bool s_IsToolsetClang     = false;
	static constexpr bool s_IsToolsetGCC       = false;
#elif PM_IS_TOOLSET_CLANG
	static constexpr EPMToolsetFlags s_Toolset   = EPMToolsetFlag::Clang;
	static constexpr bool s_IsToolsetMSVC        = false;
	static constexpr bool s_IsToolsetClang       = true;
	static constexpr bool s_IsToolsetGCC         = false;
#elif PM_IS_TOOLSET_GCC
	static constexpr EPMToolsetFlags s_Toolset   = EPMToolsetFlag::GCC;
	static constexpr bool s_IsToolsetMSVC        = false;
	static constexpr bool s_IsToolsetClang       = false;
	static constexpr bool s_IsToolsetGCC         = true;
#else
	static constexpr EPMToolsetFlags s_Toolset = EPMToolsetFlag::Unknown;
	static constexpr bool s_IsToolsetMSVC      = false;
	static constexpr bool s_IsToolsetClang     = false;
	static constexpr bool s_IsToolsetGCC       = false;
#endif

#if PM_IS_PLATFORM_X86
	static constexpr EPMPlatformFlags s_Platform = EPMPlatformFlag::X86;
	static constexpr bool s_IsPlatformX86        = true;
	static constexpr bool s_IsPlatformAMD64      = false;
#elif PM_IS_PLATFORM_AMD64
	static constexpr EPMPlatformFlags s_Platform = EPMPlatformFlag::AMD64;
	static constexpr bool s_IsPlatformX86        = false;
	static constexpr bool s_IsPlatformAMD64      = true;
#else
	static constexpr EPMPlatformFlags s_Platform = EPMPlatformFlag::Unknown;
	static constexpr bool s_IsPlatformX86        = false;
	static constexpr bool s_IsPlatformAMD64      = false;
#endif
} // namespace Core