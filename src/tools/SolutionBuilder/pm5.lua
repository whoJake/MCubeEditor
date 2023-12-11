workspace "SolutionBuilder"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Deploy"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"
powershell = "%windir%\\System32\\WindowsPowerShell\\v1.0\\powershell.exe "
deploydir = "%{wks.location}/../../../../tools/%{cfg.system}-%{cfg.architecture}/%{wks.name}/%{prj.name}"

project "SolutionBuilderApp"
    location "SolutionBuilder"
    kind     "ConsoleApp"
    language "C++"

    targetdir (".../bin/%{wks.name}/%{prj.name}/" .. outputdir)
    objdir    (".../bin-int/%{wks.name}/%{prj.name}/" .. outputdir)

    files
    {
        "**.h",
        "**.cpp",
        "**.hpp",
        "**.rc",

        "pm5.lua"
    }

    includedirs
    {
        "%{prj.name}"
    }

    filter "system:windows"
        cppdialect "C++17"
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
        targetdir (deploydir .. " ")

        postbuildcommands
        {
            "{MKDIR} \"%userprofile%\\Start Menu\\Programs\\%{wks.name}\"",
            (powershell .. "$s=(New-Object -COM WScript.Shell).CreateShortcut('%userprofile%\\Start Menu\\Programs\\%{wks.name}\\%{prj.name}.lnk');$s.TargetPath='%{deploydir}\\%{prj.name}.exe';$s.IconLocation='%{prj.location}\\icon.ico';$s.Save()")
        }

        optimize "On"
        