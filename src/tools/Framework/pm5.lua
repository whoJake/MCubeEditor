workspace "Framework"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Deploy"
    }

-- Project names
prjBase = "Base"
prjExperimental = "Experimental"

vendordir = "../../3rdparty/"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

project (prjBase)
    location "%{prj.name}"
    kind     "StaticLib"
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
        "%{prjConfiguration}",
        "%{vendordir}/glfw",
        "%{vendordir}/vma3.0.1/include",
        "%{vendordir}/glslang/StandAlone",
        "%{vendordir}/glslang/Include",
        "%{vendordir}/glslang/SPIRV",

        -- vulkan
        "%VULKAN_SDK%/Include"
    }

    dependson
    {
    }

    libdirs
    {
        "%{vendordir}/glfw/lib-vc2022",
        "%VULKAN_SDK%/Lib",
    }

    links
    {
        "glfw3_mt",

        -- vulkan
        "vulkan-1",

        -- spirv/glslang
		"spirv-cross-core",
		"spirv-cross-glsl",

		"glslang",
		"HLSL",
		"SPIRV",
		"OGLCompiler",
		"OSDependent",
		"SPVRemapper",
		"SPIRV-Tools",
		"SPIRV-Tools-opt",
		"GenericCodeGen",
		"MachineIndependent",
		"glslang-default-resource-limits",
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
            "CFG_DEBUG",
            "USE_VK_VALIDATION_LAYERS",
        }
        symbols "On"

    filter "configurations:Release"
        defines
        {
            "CFG_RELEASE",
            "USE_VK_VALIDATION_LAYERS",
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
      
project (prjExperimental)
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
        "%{prjBase}",
        "%{vendordir}"
    }

    dependson
    {
        "%{prjBase}",
    }

    links
    {
        "%{prjBase}",
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