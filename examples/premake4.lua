
local Saurobyte_Example_LibDir = "../"..Saurobyte_OutputDir
local Saurobyte_Example_IncDir = "../"..Saurobyte_OutputIncDir

solution(Saurobyte_Solution_Name)

	-- Set project options
	project("test")
		kind("ConsoleApp")
		language("C++")
		includedirs(Saurobyte_Example_IncDir)
		libdirs(Saurobyte_Example_LibDir)
		targetdir(".")

		-- Set source files
		files({"test.cpp"})

		-- Link libraries per platform
		links({"Saurobyte"})

		-- Set rpath
		configuration({"linux", "gmake"})
			linkoptions("-Wl,-R\\$$ORIGIN/"..Saurobyte_Example_LibDir)
		configuration("macosx", "gmake")
			linkoptions("-Wl,-R@rpath/"..Saurobyte_Example_LibDir)

		configuration("Debug")
			flags({"Symbols"})

		configuration("Release")
			flags({"Optimize"})
