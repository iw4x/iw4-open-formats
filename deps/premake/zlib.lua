iw4of.zlib = {
	source = path.join(iw4of_dependencies.basePath, "zlib"),
}

function iw4of.zlib.import()
	links { "zlib" }
	iw4of.zlib.includes()
end

function iw4of.zlib.includes()
	includedirs {
		iw4of.zlib.source
	}

	defines {
		"ZLIB_CONST",
	}
end

function iw4of.zlib.project()
	project "zlib"
		language "C"

		iw4of.zlib.includes()

		files {
			path.join(iw4of.zlib.source, "*.h"),
			path.join(iw4of.zlib.source, "*.c"),
		}

		defines {
			"_CRT_SECURE_NO_DEPRECATE",
		}

		warnings "Off"
		kind "StaticLib"
end
