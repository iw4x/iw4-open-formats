zstd = {
	source = path.join(dependencies.basePath, "zstd"),
}

function zstd.import()
	links { "zstd" }
	zstd.includes()
end

function zstd.includes()
	includedirs {
		path.join(zstd.source, "lib"),
		path.join(zstd.source, "lib/common"),
		--path.join(zstd.source, "zlibWrapper")
	}
end

function zstd.project()
	project "zstd"
		language "C"

		zstd.includes()

		files {
			path.join(zstd.source, "lib/**.h"),
			path.join(zstd.source, "lib/**.c"),
			--path.join(zstd.source, "zlibWrapper/zstd_zlibwrapper.h"),
			--path.join(zstd.source, "zlibWrapper/zstd_zlibwrapper.c"),
		}
		removefiles {
			path.join(zstd.source, "lib/legacy/**.*"),
			--path.join(zstd.source, "zlibWrapper/examples/**.*"),
		}
		defines {
			"zstd_DLL",
			"_CRT_SECURE_NO_DEPRECATE",
		}

		warnings "Off"
		kind "StaticLib"
end

table.insert(dependencies, zstd)
