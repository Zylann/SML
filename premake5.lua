solution "SML"
	platforms { "x32" }
	--location "."

	-- Global default configurations
	configurations { "Debug", "Release" }

	-- Include projects
	include("SML")
	include("Test")

