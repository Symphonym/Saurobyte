

local operatingSystem = os.get()
local osDefines = {}

-- Delete library files in binary folder
local executableLibaryDir = "lib"
os.rmdir("bin/"..executableLibaryDir)

-- Define operating system and move libs to binary dir
if(operatingSystem == "linux") then
	table.insert(osDefines, "SAUROBYTE_OS_LINUX")
	os.execute("cp -r ./lib/linux/ ./bin/lib/")
elseif(operatingSystem == "windows") then
	table.insert(osDefines, "SAUROBYTE_OS_WINDOWS")
	os.execute("cp -r ./lib/windows/ ./bin/lib/")
elseif(operatingSystem == "macosx") then
	table.insert(osDefines, "SAUROBYTE_OS_MACOSX")
	os.execute("cp -r ./lib/mac/ ./bin/lib/")
end

-- Make sure we're exporting symbols on Windows
table.insert(osDefines, "SAUROBYTE_API_EXPORT")

Project_Name = "Saurobyte"
Solution_Name = "Saurobyte_Solution"

-- A solution contains projects, and defines the available configurations
solution(Solution_Name)
	
	configurations({ "Debug", "Release" })
		location("build")
		includedirs({"inc"})
		libdirs("lib/linux")
		defines(osDefines)

	-- Set rpath and C++11 for GCC
	configuration({"linux", "gmake"})
		buildoptions({"-std=c++0x"})
		linkoptions("-Wl,-R\\$$ORIGIN/"..executableLibaryDir.."/")


	-- Set project options
	project(Project_Name)
		kind("WindowedApp")
		language("C++")
		includedirs({"src"})
		targetdir("bin")
		
		-- Set source files
		files({"src/*.hpp", "src/*.cpp",
			"src/Systems/*.hpp", "src/Systems/*.cpp",
			"src/Components/*.hpp", "src/Components/*.cpp",
			"src/Lua/*.hpp", "src/Lua/*.cpp"})

		-- Link common libraries
		links({"SDL2", "SDL2_image", "lua", "dl", "openal", "sndfile"})

		-- Link platform specific libraries

		-- Link OpenGL on Linux
		configuration("linux")
			links({"GL", "GLEW"})

		configuration("Debug")
			flags({"Symbols"})

		configuration("Release")
			flags({"Optimize"})

-- Compile release action
newaction(
{
	trigger = "compile-release",
	description = "Compiles the program",
	execute = function ()

		if(operatingSystem == "linux") then
			print("\n<<< Compiling (release) project >>>> \n")
			os.execute("make config=release -C./build/")
		end
	end
})

-- Compile debug action
newaction(
{
	trigger = "compile-debug",
	description = "Compiles the program",
	execute = function ()

		if(operatingSystem == "linux") then
			print("\n<<< Compiling (debug) project >>>> \n")
			os.execute("make config=debug -C./build/")
		end
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
			os.execute("cd ./bin && ./"..Project_Name)
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
			os.execute("cd ./bin && gdb -tui ./"..Project_Name)
		end
	end
})