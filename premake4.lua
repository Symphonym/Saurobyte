

local operatingSystem = os.get()
local osDefines = {}

if(operatingSystem == "linux") then
	table.insert(osDefines, "OS_LINUX")
elseif(operatingSystem == "windows") then
	table.insert(osDefines, "OS_WINDOWS")
elseif(operatingSystem == "macosx") then
	table.insert(osDefines, "OS_MACOSX")
end



-- A solution contains projects, and defines the available configurations
solution "MyApplication"
	
	configurations { "Debug", "Release" }
		location("build")
		includedirs({"inc"})
		libdirs("lib")
		defines(osDefines)

	-- Set rpath and C++11 for GCC
	configuration({"linux", "gmake"})
		buildoptions({"-std=c++0x"})
		linkoptions("-Wl,-R\\$$ORIGIN/lib")


	-- A project defines one build target
	project("MyApplication")
		kind("WindowedApp")
		language("C++")
		includedirs({"src"})
		files({"src/*.hpp", "src/*.cpp",
			"src/Systems/*.hpp", "src/Systems/*.cpp",
			"src/Components/*.hpp", "src/Components/*.cpp",
			"src/Lua/*.hpp", "src/Lua/*.cpp"})
		links({"SDL2", "SDL2_image", "lua", "dl"})
		targetdir("bin")


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
			os.execute("cd ./bin && ./MyApplication")
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
			os.execute("cd ./bin && gdb -tui ./MyApplication")
		end
	end
})