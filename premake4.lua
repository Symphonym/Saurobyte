dofile("./luautil.lua")

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

-- Remove all previous binary output
os.rmdir(path.getdirectory(Saurobyte_OutputDir))
os.mkdir(path.getdirectory(Saurobyte_OutputDir))

os.rmdir(path.getdirectory(Saurobyte_OutputIncDir))
os.mkdir(path.getdirectory(Saurobyte_OutputIncDir))

-- Copy all dependencies to output dir
os.copydir(Saurobyte_Dep_LibDir, Saurobyte_OutputDir)

-- Copy Saurobyte headers to output dir
os.copydir(path.getdirectory(Saurobyte_BaseSourceDir), path.getdirectory(Saurobyte_OutputIncDir), "**.hpp")

-- A solution contains projects, and defines the available configurations
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


	-- Set project options
	project(Saurobyte_Project_Name)
		kind("SharedLib")
		language("C++")
		includedirs(
		{
			Saurobyte_BaseSourceDir, -- Saurobyte headers
			Saurobyte_Dep_IncDir -- Dependency headers
		})
		libdirs(Saurobyte_OutputDir)
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
			links({"GL", "GLEW", "SDL2", "SDL2_image", "lua5.2", "openal", "sndfile"})

		configuration("Debug")
			flags({"Symbols"})

		configuration("Release")
			flags({"Optimize"})


-- Compile examples
include(Saurobyte_ExampleDir)


-- Compile release action
newaction(
{
	trigger = "compile-release",
	description = "Compiles the program",
	execute = function ()

		print("\n<<< Compiling (release) project >>>> \n")
		os.execute("make config=release -C./"..Saurobyte_Premake_BuildDir)
	end
})

-- Compile debug action
newaction(
{
	trigger = "compile-debug",
	description = "Compiles the program",
	execute = function ()
		print("\n<<< Compiling (debug) project >>>> \n")
		os.execute("make config=debug -C./"..Saurobyte_Premake_BuildDir)
	end
})




-- Run action
newaction(
{
	trigger = "run",
	description = "Runs the program",
	execute = function ()

		if(operatingSystem == "linux") then
			print("\n<<< Running (release) project >>>> \n")
			os.execute("cd ./bin && ./"..Saurobyte_Project_Name)
		end
	end
})

-- GDB debug action
newaction(
{
	trigger = "debug",
	description = "Debugs the program",
	execute = function ()

		if(operatingSystem == "linux") then
			print("\n<<< Running (debug) project >>>> \n")
			os.execute("cd ./bin && gdb -tui ./"..Saurobyte_Project_Name)
		end
	end
})