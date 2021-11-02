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
	
	project("PasswordManager")
		kind("ConsoleApp")
		location("%{wks.location}/")
		targetdir("%{wks.location}/Bin/%{cfg.system}-%{cfg.buildcfg}-%{cfg.platform}/")
		objdir("%{wks.location}/Bin/Int-%{cfg.system}-%{cfg.buildcfg}-%{cfg.platform}/%{prj.name}")
		debugdir("%{prj.location}/Run/")
		
		local openSSLLibCryptoPath = path.translate(openSSLLibCryptoPath, "/")
		local openSSLLibCryptoDir = path.getdirectory(openSSLLibCryptoPath)
		local openSSLLibCryptoSharedPath = path.translate(openSSLLibCryptoSharedPath, "/")
		
		postbuildcommands({
			"{COPY} \"" .. openSSLLibCryptoSharedPath .. "\" \"Bin/%{cfg.system}-%{cfg.buildcfg}-%{cfg.platform}/\""
		})
		
		libdirs({ path.getdirectory(openSSLLibCryptoPath) })
		
		links({ removeOsSpecificLibraryAffixes(path.getname(openSSLLibCryptoPath)) })
		sysincludedirs({ openSSLInclude })
		
		includedirs({ "%{prj.location}/Source/" })
		
		files({ "%{prj.location}/**" })
		
		filter("files:**.h")
			runclangformat(true)
		
		filter("files:**.cpp or **.mm")
			runclangformat(true)
			runclangtidy(true)
		
		filter({})
