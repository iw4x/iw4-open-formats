iw4of = {}

iw4of_dependencies = {
	basePath = "./deps"
}

function iw4of_dependencies.load()
	dir = path.join(iw4of_dependencies.basePath, "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function iw4of_dependencies.imports()
	for dep_name, dependency in pairs(iw4of) do
		dependency.import()
	end
end

function iw4of_dependencies.projects()
	for dep_name, dependency in pairs(iw4of) do
		dependency.project()
	end
end

newoption {
	trigger = "copy-to",
	description = "Optional, copy the EXE to a custom folder after build, define the path here if wanted.",
	value = "PATH"
}

iw4of_dependencies.load()

workspace "iw4-of"
	startproject "iw4-of-interactive"
	location "./build"
	objdir "%{wks.location}/obj"
	targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

	configurations {"Debug", "Release"}

	language "C++"
	cppdialect "C++latest"

	architecture "x86"
	platforms "Win32"

	systemversion "latest"
	symbols "On"
	staticruntime "On"
	editandcontinue "Off"
	warnings "Extra"
	characterset "ASCII"

	flags {
		"NoIncrementalLink",
		"NoMinimalRebuild",
		"MultiProcessorCompile",
		"No64BitChecks"
	}

	filter "platforms:Win*"
		defines {"_WINDOWS", "WIN32"}
	filter {}

	filter "configurations:Release"
		optimize "Size"
		buildoptions {"/Os"}
		defines {"NDEBUG"}
		flags {"FatalCompileWarnings"}
	filter {}

	filter "configurations:Debug"
		optimize "Debug"
		defines {"DEBUG", "_DEBUG"}
	filter  {}

	
	project "iw4-of-interactive"
		kind "ConsoleApp"
		language "C++"

		files {
			"./src/interactive/**.hpp",
			"./src/interactive/**.cpp"
		}

		includedirs {
			"./include"
		}
		
		links { "iw4-of" }

	project "iw4-of"
		kind "StaticLib"
		language "C++"

		pchheader "std_include.hpp" -- must be exactly same as used in #include directives
		pchsource "src/iw4-of/std_include.cpp" -- real path
		
		files {
			"./src/iw4-of/**.hpp",
			"./src/iw4-of/**.cpp"
		}
		
		includedirs {
			"./src/iw4-of",
			"./include"
		}

		iw4of_dependencies.imports()

	group "iw4of_dependencies"
		iw4of_dependencies.projects()
