project "Utils"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"
	flags { "MultiProcessorCompile" }

	targetdir ("bin/builds/%{cfg.buildcfg}_%{cfg.system}")
    objdir ("bin/intermediates/%{cfg.buildcfg}_%{cfg.system}")
	
	PrjLoc = "%{prj.location}/"

    pchheader "pch.h"
    pchsource "Private/pch.cpp"

    -- preprocessors
	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	files {
		"**.lua",
		"**.h",
		"**.cpp",
	}

    -- include dirs
	includedirs {
        PrjLoc,
        PrjLoc.. "Private",
	}

    -- kind
    filter "kind:StaticLib"
        defines {
            "STATIC_BUILD"
        }
    filter {}

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