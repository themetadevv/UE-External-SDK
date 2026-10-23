workspace "UE_SDK"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "SampleUsage"

-- globals
WorkspaceLoc = "%{wks.location}/"
ExportsDir   = "%{wks.location}/Exports/"
ExternalsDir = "%{wks.location}/Externals/"

-- projects include directories
Projects = {
	Utils = WorkspaceLoc.. "Projects/Utils",
	UE_427_EXTERNAL_SDK = WorkspaceLoc.. "Projects/UE-4.27-External-SDK"
}

-- External Libraries
group "Externals"

group ""

-- Projects
group "Projects"
    include "Projects/Utils/Utils.lua"
    include "Projects/Sample-Project/Sample-Project.lua"
    include "Projects/UE-4.27-External-SDK/UE-4.27-External-SDK.lua"
group ""