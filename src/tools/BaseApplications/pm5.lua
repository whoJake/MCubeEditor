workspace "BaseApplications"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Deploy"
    }

-- Project names
prjConsoleApplication = "ConsoleApplication"
prjWindowedApplication = "WindowedApplication"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

project (prjConsoleApplication)
    location "%{prj.name}"
    kind     "ConsoleApp"
    language "C++"

    targetdir (".../bin/%{wks.name}/%{prj.name}/" .. outputdir)
    objdir    (".../bin-int/%{wks.name}/%{prj.name}/" .. outputdir)

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.hpp",

        "pm5.lua"
    }

    includedirs
    {
        "%{prj.name}"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        runtime "Release"
        buildoptions "/MD"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines
        {
            "CFG_DEBUG"
        }
        symbols "On"

    filter "configurations:Release"
        defines
        {
            "CFG_RELEASE"
        }
        optimize "On"

    filter "configurations:Deploy"
        defines
        {
            "CFG_RELEASE",
            "CFG_DEPLOY"
        }
        targetdir ("../../../tools/%{cfg.system}-%{cfg.architecture}/%{wks.name}/%{prj.name}")
        optimize "On"
        
project (prjWindowedApplication)
    location "%{prj.name}"
    kind     "ConsoleApp"
    language "C++"

    targetdir (".../bin/%{wks.name}/%{prj.name}/" .. outputdir)
    objdir    (".../bin-int/%{wks.name}/%{prj.name}/" .. outputdir)

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.hpp",

        "pm5.lua"
    }

    includedirs
    {
        "%{prj.name}",
        "%{prjConsoleApplication}"
    }

    dependson
    {
        "%{prjConsoleApplication}",
    }

    links
    {
        "%{prjConsoleApplication}"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        runtime "Release"
        buildoptions "/MD"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines
        {
            "CFG_DEBUG"
        }
        symbols "On"

    filter "configurations:Release"
        defines
        {
            "CFG_RELEASE"
        }
        optimize "On"

    filter "configurations:Deploy"
        defines
        {
            "CFG_RELEASE",
            "CFG_DEPLOY"
        }
        targetdir ("../../../tools/%{cfg.system}-%{cfg.architecture}/%{wks.name}/%{prj.name}")
        optimize "On"
 