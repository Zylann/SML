project "Test"
	platforms { "x32" }
	kind "ConsoleApp"
	language "C++"
	location "."
	dependson {
		"LibSML"
	}
	includedirs {
		"../SML"
	}
	links {
		"LibSML"
	}
	files {
		"**.h",
		"**.hpp",
		"**.cpp"
	}
	filter "configurations:Debug"
		targetdir "_bin/debug"
		objdir "_obj/debug"
		flags {
			"Symbols" -- Produce debug symbols
		}
		defines {
			"SML_BUILD_DEBUG"
		}
	filter "configurations:Release"
		targetdir "_bin/release"
		objdir "_obj/release"
		optimize "On"
		flags {
			"LinkTimeOptimization"
		}




