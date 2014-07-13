
-- Grab a utility function for file copying
dofile("./luautil.lua")


-----------------------------------------------------------------------------------------------
--  Declaring global variables
-----------------------------------------------------------------------------------------------

Saurobyte_OperatingSystem = os.get()
-- TODO need premake 4.4 to make arch dirs for libs Saurobyte_OSArch = "32bit"

-- Various output directories
Saurobyte_OutputDir = "build/bin/"
Saurobyte_Dep_OutputDir = "build/dependencies/"
Saurobyte_Premake_BuildDir = "build/premake"
Saurobyte_ExampleDir = "examples/"

-- Linking & including dependencies
Saurobyte_Dep_LibDir = "dependencies/lib/"..Saurobyte_OperatingSystem.."/"
Saurobyte_Dep_IncDir = "dependencies/inc/"
Saurobyte_Dep_IncDirs = 
{
	Saurobyte_Dep_IncDir.."common/*", -- Common headers
	Saurobyte_Dep_IncDir..Saurobyte_OperatingSystem.."/*"-- Platform specific headers
}
Saurobyte_Dep_SourceDir = "dependencies/src/"

-- Libraries to link against
Saurobyte_Linux_Links =
{
	"GL", "GLEW", "SDL2", "SDL2_image", "lua", "openal", "sndfile"
}
Saurobyte_Linux_Static_Links =
{
	--"vorbis.a", "vorbisfile.a", "ogg.a"
}

Saurobyte_Mac_Links =
{
	"Don't even know cause not supported yet"
}
Saurobyte_Window_Links =
{
	"Don't even know cause not supported yet"
}

-- Source related directories
Saurobyte_BaseSourceDir = "src/"
Saurobyte_SourceDir = Saurobyte_BaseSourceDir.."Saurobyte/"

-- Project names
Saurobyte_Solution_Name = "Saurobyte_Solution"
Saurobyte_Project_Name = "Saurobyte"

-----------------------------------------------------------------------------------------------
--  Prepare output
-----------------------------------------------------------------------------------------------

-- Clean lib directory
os.rmdir(path.getdirectory(Saurobyte_Dep_OutputDir))
os.mkdir(path.getdirectory(Saurobyte_Dep_OutputDir))

-- Clean away .dll files
for _, dllFileName in pairs(os.matchfiles(Saurobyte_Dep_OutputDir.."*.dll")) do
	os.remove(dllFileName)
end

if (Saurobyte_OperatingSystem == "linux") then
	os.copyfile(Saurobyte_Dep_LibDir.."libSDL2-2.0.so.0.2.0", Saurobyte_Dep_OutputDir.."libSDL2-2.0.so.0.2.0")
	os.copyfile(Saurobyte_Dep_LibDir.."libopenal.so.1.14.0", Saurobyte_Dep_OutputDir.."libopenal.so.1.14.0")
elseif (Saurobyte_OperatingSystem == "windows") then
	os.copydir(Saurobyte_Dep_LibDir, Saurobyte_Dep_OutputDir, "*.dll")
end

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
	configuration({"gmake"})
		buildoptions({"-std=c++11"})


-----------------------------------------------------------------------------------------------
--  Saurobyte library project setup
-----------------------------------------------------------------------------------------------

	project(Saurobyte_Project_Name)
		kind("ConsoleApp")
		language("C++")
		includedirs({Saurobyte_BaseSourceDir}) -- Saurobyte headers
		includedirs({Saurobyte_Dep_IncDirs, Saurobyte_Dep_SourceDir.."*"}) -- Dependency headers
		libdirs(Saurobyte_Dep_LibDir)
		targetdir(Saurobyte_OutputDir)

		-- Set source files
		files({
			"src/**.hpp", "src/**.cpp", -- Saurobyte source
			Saurobyte_Dep_SourceDir.."**.cpp", Saurobyte_Dep_SourceDir.."**.c" -- Dependencies provided in-source
			})

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
--include(Saurobyte_ExampleDir)

