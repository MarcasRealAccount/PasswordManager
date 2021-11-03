local function removeOsSpecificLibraryAffixes(filename)
	if path.islinkable(filename) then
		local basename = path.getbasename(filename)
		if os.host() == "windows" then
			return basename
		elseif string.startswith(basename, "lib") then
			return string.sub(basename, 4, -1)
		else
			return basename
		end
	end
	return filename
end

require("Premake/cleanAction")
require("Premake/formatTidyAction")

newoption({
	trigger = "glfw_use_wayland",
	description = "Should glfw use wayland for linux build",
	value = "boolean",
	allowed = {
		{ "N", "No" },
		{ "Y", "Yes" }
	},
	default = "N"
})

local openSSLLibCryptoPath = os.getenv("OPENSSL_LIB_CRYPTO_PATH")
if not openSSLLibCryptoPath then
	error("Please find the path to openssl libcrypto and set 'OPENSSL_LIB_CRYPTO_PATH' environment variable to that!")
end
local openSSLLibCryptoSharedPath = os.getenv("OPENSSL_LIB_CRYPTO_SHARED_PATH")
if not openSSLLibCryptoSharedPath then
	error("Please find the path to openssl libcrypto shared library and set 'OPENSSL_LIB_CRYPTO_SHARED_PATH' environment variable to that!")
end
local openSSLInclude = os.getenv("OPENSSL_INCLUDE")
if not openSSLInclude then
	error("Please find the path to openssl include and set 'OPENSSL_INCLUDE' environment variable to that!")
end

local vulkanSDKPath = os.getenv("VULKAN_SDK")
if not vulkanSDKPath then
	local hostOS = os.host()
	if hostOS == "windows" then
		error("Have you installed the Vulkan SDK correctly.\nIf you have then please go into environment variables and add 'VULKAN_SDK' with the path to the SDK!")
	elseif hostOS == "macosx" then
		error("Please find the Vulkan SDK and run the 'setup-env.sh' script in a terminal environment before running premake again!\nYou can open '~/.zshrc' or '~/.bashrc' and add:\ncd \"PathToVulkanSDK\"\nsource setup-env.sh\ncd ~/")
	else
		error("Please find the Vulkan SDK and run the 'setup-env.sh' script in a terminal environment before running premake again!\nYou can open '~/.zshrc' or '~/.bashrc' and add 'source \"PathToVulkanSDK/setup-env.sh\"'")
	end
end

workspace("PasswordManager")
	configurations({ "Debug", "Release", "Dist" })
	platforms({ "x64" }) -- "x86", 
	
	cppdialect("C++17")
	rtti("Off")
	exceptionhandling("On")
	flags("MultiProcessorCompile")
	runclangformat(false)
	runclangtidy(false)
	
	filter("configurations:Debug")
		defines({ "PM_CONFIG=PM_CONFIG_DEBUG" })
		optimize("Off")
		symbols("On")
		
	filter("configurations:Release")
		defines({ "PM_CONFIG=PM_CONFIG_RELEASE" })
		optimize("Full")
		symbols("On")
		
	filter("configurations:Dist")
		defines({ "PM_CONFIG=PM_CONFIG_DIST" })
		optimize("Full")
		symbols("Off")
		
	filter("system:windows")
		defines({
			"PM_SYSTEM=PM_SYSTEM_WINDOWS",
			"NOMINMAX",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS"
		})
		toolset("msc")
		
	filter("system:macosx")
		defines({ "PM_SYSTEM=PM_SYSTEM_MACOSX" })
		
	filter("system:linux")
		defines({ "PM_SYSTEM=PM_SYSTEM_LINUX" })
	
	filter("toolset:msc")
		defines({ "PM_TOOLSET=PM_TOOLSET_MSVC" })
	
	filter("toolset:clang")
		defines({ "PM_TOOLSET=PM_TOOLSET_CLANG" })
	
	filter("toolset:gcc")
		defines({ "PM_TOOLSET=PM_TOOLSET_GCC" })
	
	filter("platforms:x86")
		defines({ "PM_PLATFORM=PM_PLATFORM_X86" })
	
	filter("platforms:x64")
		defines({ "PM_PLATFORM=PM_PLATFORM_AMD64" })
	
	filter({})
	
	startproject("PasswordManager")
	
	project("GLFW")
		location("ThirdParty/GLFW/")
		kind("StaticLib")
		targetdir("%{wks.location}/Bin/Int-%{cfg.system}-%{cfg.platform}-%{cfg.buildcfg}/%{prj.name}/")
		objdir("%{wks.location}/Bin/Int-%{cfg.system}-%{cfg.platform}-%{cfg.buildcfg}/%{prj.name}/")
		
		warnings("Off")

		includedirs({ "%{prj.location}/include/" })

		files({
			"%{prj.location}/include/**",
			"%{prj.location}/src/context.c",
			"%{prj.location}/src/init.c",
			"%{prj.location}/src/input.c",
			"%{prj.location}/src/internal.h",
			"%{prj.location}/src/mappings.h",
			"%{prj.location}/src/monitor.c",
			"%{prj.location}/src/null_*",
			"%{prj.location}/src/platform.h",
			"%{prj.location}/src/platform.c",
			"%{prj.location}/src/vulkan.c",
			"%{prj.location}/src/window.c",
			"%{prj.location}/src/egl_*",
			"%{prj.location}/src/osmesa_*"
		})

		filter("system:windows")
			files({
				"%{prj.location}/src/win32_*",
				"%{prj.location}/src/wgl_*"
			})

			defines({ "_GLFW_WIN32" })

		filter("system:linux")
			files({
				"%{prj.location}/src/linux_*",
				"%{prj.location}/src/posix_*",
				"%{prj.location}/src/xkb_*",
				"%{prj.location}/src/glx_*"
			})
			
if _OPTIONS["glfw_use_wayland"] == "Y" then
			files({
				"%{prj.location}/src/wl_*"
			})

			defines({ "_GLFW_WAYLAND" })
else
			files({
				"%{prj.location}/src/x11_*"
			})

			defines({ "_GLFW_X11" })
end

		filter("system:macosx")
			files({
				"%{prj.location}/src/cocoa_*",
				"%{prj.location}/src/nsgl_*",
				"%{prj.location}/src/posix_*"
			})
			removefiles({
				"%{prj.location}/src/posix_time.h",
				"%{prj.location}/src/posix_time.c"
			})

			defines({ "_GLFW_COCOA" })

		filter({})
	
	project("VMA")
		location("ThirdParty/VMA/")
		kind("StaticLib")
		targetdir("%{wks.location}/Int/%{cfg.system}-%{cfg.platform}-%{cfg.buildcfg}/%{prj.name}/")
		objdir("%{wks.location}/Int/%{cfg.system}-%{cfg.platform}-%{cfg.buildcfg}/%{prj.name}/")
		removedefines({ "NOMINMAX", "WIN32_LEAN_AND_MEAN" })
		
		warnings("Off")

		includedirs({ "%{prj.location}/include/" })
		filter("system:windows")
			sysincludedirs({ vulkanSDKPath .. "/Include/" })

		filter("system:linux")
			sysincludedirs({ vulkanSDKPath .. "/include/" })
			
		filter("system:macosx")
			sysincludedirs({ vulkanSDKPath .. "/include/" })
		
		filter({})

		files({
			"%{prj.location}/include/**",
			"%{prj.location}/src/VmaUsage.h",
			"%{prj.location}/src/VmaUsage.cpp"
		})
	
	project("PasswordManager")
		location("%{wks.location}/")
		targetdir("%{wks.location}/Bin/%{cfg.system}-%{cfg.buildcfg}-%{cfg.platform}/")
		objdir("%{wks.location}/Bin/Int-%{cfg.system}-%{cfg.buildcfg}-%{cfg.platform}/%{prj.name}")
		debugdir("%{prj.location}/Run/")
		
		filter("configurations:Debug")
			kind("ConsoleApp")
		
		filter("configurations:Release or Dist")
			kind("WindowedApp")
		
		filter({})
		
		local openSSLLibCryptoPath = path.translate(openSSLLibCryptoPath, "/")
		local openSSLLibCryptoDir = path.getdirectory(openSSLLibCryptoPath)
		local openSSLLibCryptoSharedPath = path.translate(openSSLLibCryptoSharedPath, "/")
		
		postbuildcommands({
			"{COPY} \"" .. openSSLLibCryptoSharedPath .. "\" \"Bin/%{cfg.system}-%{cfg.buildcfg}-%{cfg.platform}/\""
		})
		
		libdirs({ path.getdirectory(openSSLLibCryptoPath) })
		
		links({
			"GLFW",
			"VMA",
			removeOsSpecificLibraryAffixes(path.getname(openSSLLibCryptoPath))
		})
		sysincludedirs({
			"%{wks.location}/ThirdParty/GLFW/include/",
			"%{wks.location}/ThirdParty/VMA/include/",
			"%{wks.location}/ThirdParty/STB/",
			openSSLInclude
		})
		
		filter("system:windows")
			libdirs({ vulkanSDKPath .. "/Lib/" })
			links({
				"vulkan-1.lib",
				"glslang.lib"
			})
			sysincludedirs({ vulkanSDKPath .. "/Include/" })
		
		filter("system:linux")
			libdirs({ vulkanSDKPath .. "/lib/" })
			links({
				"libvulkan.so.1",
				"libglslang.a"
			})
			sysincludedirs({ vulkanSDKPath .. "/include/" })
		
		filter("system:macosx")
			libdirs({ vulkanSDKPath .. "/lib/" })
			links({
				"vulkan",
				"glslang",
				"CoreGraphics.framework",
				"IOKit.framework",
				"AppKit.framework"
			})
			sysincludedirs({ vulkanSDKPath .. "/include/" })
		
		filter({})
		
		includedirs({ "%{prj.location}/Source/" })
		
		files({ "%{prj.location}/Source/**" })
		
		filter("files:**.h")
			runclangformat(true)
		
		filter("files:**.cpp or **.mm")
			runclangformat(true)
			runclangtidy(true)
		
		filter({})
