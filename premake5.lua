require("Premake/cleanAction")
require("Premake/formatTidyAction")

workspace("PasswordManager")
	configurations({ "Debug", "Release", "Dist" })
	platforms({ "x86", "x64" })
	
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
		
		links({ "crypto" })
		
		includedirs({ "%{prj.location}/Source/" })
		
		files({ "%{prj.location}/**" })
		
		filter("files:**.h")
			runclangformat(true)
		
		filter("files:**.cpp or **.mm")
			runclangformat(true)
			runclangtidy(true)
		
		filter({})
