
-- Grab a utility function for file copying
dofile("./luautil.lua")

-----------------------------------------------------------------------------------------------
--  Declaring global variables
-----------------------------------------------------------------------------------------------

Saurobyte_OperatingSystem = os.get()

Saurobyte_BuildDir = "build/"
Saurobyte_OutputDir = Saurobyte_BuildDir.."lib/"
Saurobyte_OutputIncDir = Saurobyte_BuildDir.."inc/"
Saurobyte_ExampleDir = "examples/"
Saurobyte_Dep_LibDir = "dependencies/lib/"..Saurobyte_OperatingSystem.."/"
Saurobyte_Dep_IncDir = "dependencies/inc/"
Saurobyte_BaseSourceDir = "src/"
Saurobyte_SourceDir = Saurobyte_BaseSourceDir.."Saurobyte/"

Saurobyte_Premake_BuildDir = Saurobyte_BuildDir.."premake"

Saurobyte_Solution_Name = "Saurobyte_Solution"
Saurobyte_Project_Name = "Saurobyte"

Saurobyte_Linux_Links =
{
	"GL", "GLEW", "SDL2", "SDL2_image", "lua5.2", "openal", "sndfile"
}
Saurobyte_Mac_Links =
{
	"Don't even know cause not supported yet"
}
Saurobyte_Window_Links =
{
	"Don't even know cause not supported yet"
}

-----------------------------------------------------------------------------------------------
--  Print some system specific info
-----------------------------------------------------------------------------------------------

if(Saurobyte_OperatingSystem == "linux") then
	print("Linux based operating systems requires development packages of the following to be installed on your system:")
	
	for _, val in pairs(Saurobyte_Linux_Links) do
		print("* " .. val)
	end
end


-----------------------------------------------------------------------------------------------
--  Prepare output
-----------------------------------------------------------------------------------------------

-- Remove and recreate output directories
os.rmdir(path.getdirectory(Saurobyte_OutputDir))
os.mkdir(path.getdirectory(Saurobyte_OutputDir))

os.rmdir(path.getdirectory(Saurobyte_OutputIncDir))
os.mkdir(path.getdirectory(Saurobyte_OutputIncDir))

-- Copy Saurobyte headers to output dir
os.copydir(path.getdirectory(Saurobyte_BaseSourceDir), path.getdirectory(Saurobyte_OutputIncDir), "**.hpp")

-----------------------------------------------------------------------------------------------
--  Saurobyte solution setup
-----------------------------------------------------------------------------------------------

solution(Saurobyte_Solution_Name)
	location(Saurobyte_Premake_BuildDir)
	configurations({ "Debug", "Release" })

	-- Export symbols when compiling
	defines("SAUROBYTE_API_EXPORT")

	configuration("linux")
		defines("SAUROBYTE_OS_LINUX")
	configuration("windows")
		defines("SAUROBYTE_OS_WINDOWS")
	configuration("macosx")
		defines("SAUROBYTE_OS_MACOSX")


	-- Set C++11 for GCC
	configuration({"linux", "gmake"})
		buildoptions({"-std=c++11"})


-----------------------------------------------------------------------------------------------
--  Saurobyte library project setup
-----------------------------------------------------------------------------------------------
	
	project(Saurobyte_Project_Name)
		kind("SharedLib")
		language("C++")
		includedirs(
		{
			Saurobyte_BaseSourceDir, -- Saurobyte headers
			Saurobyte_Dep_IncDir -- Dependency headers
		})
		libdirs(Saurobyte_Dep_LibDir)
		targetdir(Saurobyte_OutputDir)


		-- Set source files
		files({
			Saurobyte_SourceDir.."*.hpp", Saurobyte_SourceDir.."*.cpp", -- Base files
			Saurobyte_SourceDir.."Systems/*.hpp", Saurobyte_SourceDir.."Systems/*.cpp", -- Systems
			Saurobyte_SourceDir.."Components/*.hpp", Saurobyte_SourceDir.."Components/*.cpp", -- Components
			Saurobyte_SourceDir.."Math/*.hpp", Saurobyte_SourceDir.."Math/*.cpp", -- Math
			Saurobyte_SourceDir.."Lua/*.hpp", Saurobyte_SourceDir.."Lua/*.cpp"}) -- Lua API

		-- Link libraries per platform
		configuration("linux")
			links(Saurobyte_Linux_Links)
		configuration("windows")
			links(Saurobyte_Windows_Links)
		configuration("macosx")
			links(Saurobyte_Mac_Links)

		configuration("Debug")
			flags({"Symbols"})

		configuration("Release")
			flags({"Optimize"})


-- Compile examples
include(Saurobyte_ExampleDir)

