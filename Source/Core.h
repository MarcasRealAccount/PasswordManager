#pragma once

#include "Flags.h"

#include <cstdint>
#include <sstream>

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

#define PM_IS_CONFIG_DEBUG (PM_CONFIG == PM_CONFIG_DEBUG)
#define PM_IS_CONFIG_DIST (PM_CONFIG == PM_CONFIG_DIST)

#define PM_IS_SYSTEM_WINDOWS (PM_SYSTEM == PM_SYSTEM_WINDOWS)
#define PM_IS_SYSTEM_MACOSX (PM_SYSTEM == PM_SYSTEM_MACOSX)
#define PM_IS_SYSTEM_LINUX (PM_SYSTEM == PM_SYSTEM_LINUX)

#define PM_IS_TOOLSET_MSVC (PM_TOOLSET == PM_TOOLSET_MSVC)
#define PM_IS_TOOLSET_CLANG (PM_TOOLSET == PM_TOOLSET_CLANG)
#define PM_IS_TOOLSET_GCC (PM_TOOLSET == PM_TOOLSET_GCC)

#define PM_IS_PLATFORM_X86 (PM_PLATFORM == PM_PLATFORM_X86)
#define PM_IS_PLATFORM_AMD64 (PM_PLATFORM == PM_PLATFORM_AMD64)

namespace Core {
	using PMConfigFlags   = Flags<std::uint16_t>;
	using PMSystemFlags   = Flags<std::uint16_t>;
	using PMToolsetFlags  = Flags<std::uint16_t>;
	using PMPlatformFlags = Flags<std::uint16_t>;

	std::ostream& printPMConfigFlags(std::ostream& stream, const PMConfigFlags& flags);
	std::ostream& printPMSystemFlags(std::ostream& stream, const PMSystemFlags& flags);
	std::ostream& printPMToolsetFlags(std::ostream& stream, const PMToolsetFlags& flags);
	std::ostream& printPMPlatformFlags(std::ostream& stream, const PMPlatformFlags& flags);

	namespace PMConfigFlag {
		static constexpr PMConfigFlags Unknown = 0;
		static constexpr PMConfigFlags Debug   = 1;
		static constexpr PMConfigFlags Dist    = 2;
	} // namespace PMConfigFlag

	namespace PMSystemFlag {
		static constexpr PMSystemFlags Unknown = 0;
		static constexpr PMSystemFlags Windows = 1;
		static constexpr PMSystemFlags Macosx  = 2;
		static constexpr PMSystemFlags Linux   = 4;
		static constexpr PMSystemFlags Unix    = 8;
	} // namespace PMSystemFlag

	namespace PMToolsetFlag {
		static constexpr PMToolsetFlags Unknown = 0;
		static constexpr PMToolsetFlags MSVC    = 1;
		static constexpr PMToolsetFlags Clang   = 2;
		static constexpr PMToolsetFlags GCC     = 4;
	} // namespace PMToolsetFlag

	namespace PMPlatformFlag {
		static constexpr PMPlatformFlags Unknown = 0;
		static constexpr PMPlatformFlags X86     = 1;
		static constexpr PMPlatformFlags AMD64   = 2;
	} // namespace PMPlatformFlag

#if PM_IS_CONFIG_DEBUG
	static constexpr PMConfigFlags s_Config = PMConfigFlag::Debug;
	static constexpr bool s_IsConfigDebug   = true;
	static constexpr bool s_IsConfigDist    = false;
#elif PM_IS_CONFIG_RELEASE
	static constexpr PMConfigFlags s_Config     = PMConfigFlag::Debug | PMConfigFlags::Dist;
	static constexpr bool s_IsConfigDebug       = true;
	static constexpr bool s_IsConfigDist        = true;
#elif PM_IS_CONFIG_DIST
	static constexpr PMConfigFlags s_Config     = PMConfigFlag::Dist;
	static constexpr bool s_IsConfigDebug       = false;
	static constexpr bool s_IsConfigDist        = true;
#else
	static constexpr PMConfigFlags s_Config   = PMConfigFlag::Unknown;
	static constexpr bool s_IsConfigDebug     = false;
	static constexpr bool s_IsConfigDist      = false;
#endif

#if PM_IS_SYSTEM_WINDOWS
	static constexpr PMSystemFlags s_System = PMSystemFlag::Windows;
	static constexpr bool s_IsSystemWindows = true;
	static constexpr bool s_IsSystemMacosx  = false;
	static constexpr bool s_IsSystemLinux   = false;
	static constexpr bool s_IsSystemUnix    = false;
#elif PM_IS_SYSTEM_MACOSX
	static constexpr PMSystemFlags s_System     = PMSystemFlag::Macosx | PMSystemFlag::Unix;
	static constexpr bool s_IsSystemWindows     = false;
	static constexpr bool s_IsSystemMacosx      = true;
	static constexpr bool s_IsSystemLinux       = false;
	static constexpr bool s_IsSystemUnix        = true;
#elif PM_IS_SYSTEM_LINUX
	static constexpr PMSystemFlags s_System     = PMSystemFlag::Linux | PMSystemFlag::Unix;
	static constexpr bool s_IsSystemWindows     = false;
	static constexpr bool s_IsSystemMacosx      = false;
	static constexpr bool s_IsSystemLinux       = true;
	static constexpr bool s_IsSystemUnix        = true;
#else
	static constexpr PMSystemFlags s_System   = PMSystemFlag::Unknown;
	static constexpr bool s_IsSystemWindows   = false;
	static constexpr bool s_IsSystemMacosx    = false;
	static constexpr bool s_IsSystemLinux     = false;
	static constexpr bool s_IsSystemUnix      = false;
#endif

#if PM_IS_TOOLSET_MSVC
	static constexpr PMToolsetFlags s_Toolset = PMToolsetFlag::MSVC;
	static constexpr bool s_IsToolsetMSVC     = true;
	static constexpr bool s_IsToolsetClang    = false;
	static constexpr bool s_IsToolsetGCC      = false;
#elif PM_IS_TOOLSET_CLANG
	static constexpr PMToolsetFlags s_Toolset   = PMToolsetFlag::Clang;
	static constexpr bool s_IsToolsetMSVC       = false;
	static constexpr bool s_IsToolsetClang      = true;
	static constexpr bool s_IsToolsetGCC        = false;
#elif PM_IS_TOOLSET_GCC
	static constexpr PMToolsetFlags s_Toolset   = PMToolsetFlag::GCC;
	static constexpr bool s_IsToolsetMSVC       = false;
	static constexpr bool s_IsToolsetClang      = false;
	static constexpr bool s_IsToolsetGCC        = true;
#else
	static constexpr PMToolsetFlags s_Toolset = PMToolsetFlag::Unknown;
	static constexpr bool s_IsToolsetMSVC     = false;
	static constexpr bool s_IsToolsetClang    = false;
	static constexpr bool s_IsToolsetGCC      = false;
#endif

#if PM_IS_PLATFORM_X86
	static constexpr PMPlatformFlags s_Platform = PMPlatformFlag::X86;
	static constexpr bool s_IsPlatformX86       = true;
	static constexpr bool s_IsPlatformAMD64     = false;
#elif PM_IS_PLATFORM_AMD64
	static constexpr PMPlatformFlags s_Platform = PMPlatformFlag::AMD64;
	static constexpr bool s_IsPlatformX86       = false;
	static constexpr bool s_IsPlatformAMD64     = true;
#else
	static constexpr PMPlatformFlags s_Platform = PMPlatformFlag::Unknown;
	static constexpr bool s_IsPlatformX86       = false;
	static constexpr bool s_IsPlatformAMD64     = false;
#endif
} // namespace Core

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif