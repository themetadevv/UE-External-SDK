project "Sample-Project"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"
	flags { "MultiProcessorCompile" }

	targetdir ("bin/builds/%{cfg.system}_%{cfg.buildcfg}")
    objdir ("bin/intermediates/%{cfg.system}_%{cfg.buildcfg}")
	
	PrjLoc = "%{prj.location}/"

    -- copies .exe to exports dir
    postbuildcommands {
		'{MKDIR}    "%{wks.location}/Exports/%{cfg.system}_%{cfg.buildcfg}/"',
		'{COPYFILE} "%{cfg.buildtarget.abspath}" "%{wks.location}/Exports/%{cfg.system}_%{cfg.buildcfg}/"',
    }

	files {
		"**.lua",
		"**.h",
		"**.cpp",
	}

    -- inlcude dirs
	includedirs {
        PrjLoc,
		PrjLoc.. "Source",
        Projects.Utils,
        Projects.UE_427_EXTERNAL_SDK
	}

    links {
        "Utils",
        "UE-4.27-External-SDK"
    }

    -- platform
    filter "system:windows"
        systemversion "latest"
        defines { 
            "PLATFORM_WINDOWS"
        }
    filter {}

    -- debug configuration
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        optimize "off"

        defines {
            "DEBUG_BUILD"
        }
    filter {}

    -- release configuration
    filter "configurations:Release"
        runtime "Release"
        symbols "off"
        optimize "on"

        defines {
            "RELEASE_BUILD"
        }
    filter {}