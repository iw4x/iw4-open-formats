dependencies = {
	basePath = "./deps"
}

function dependencies.load()
	dir = path.join(dependencies.basePath, "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function dependencies.imports()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.import()
		end
	end
end

function dependencies.projects()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.project()
		end
	end
end

newoption {
	trigger = "copy-to",
	description = "Optional, copy the EXE to a custom folder after build, define the path here if wanted.",
	value = "PATH"
}

dependencies.load()

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
		
		linkoptions {
			"/IGNORE:4254"
		}

		files {
			"./src/iw4-of/**.hpp",
			"./src/iw4-of/**.cpp"
		}
		
		includedirs {
			"./src/iw4-of",
			"./include"
		}

		dependencies.imports()

	group "Dependencies"
		dependencies.projects()
