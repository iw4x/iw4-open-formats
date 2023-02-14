iw4of.zstd = {
	source = path.join(iw4of_dependencies.basePath, "zstd"),
}

function iw4of.zstd.import()
	links { "zstd" }
	iw4of.zstd.includes()
end

function iw4of.zstd.includes()
	includedirs {
		path.join(iw4of.zstd.source, "lib"),
		path.join(iw4of.zstd.source, "lib/common"),
		--path.join(zstd.source, "zlibWrapper")
	}
end

function iw4of.zstd.project()
	project "zstd"
		language "C"

		iw4of.zstd.includes()

		files {
			path.join(iw4of.zstd.source, "lib/**.h"),
			path.join(iw4of.zstd.source, "lib/**.c"),
			--path.join(zstd.source, "zlibWrapper/zstd_zlibwrapper.h"),
			--path.join(zstd.source, "zlibWrapper/zstd_zlibwrapper.c"),
		}
		removefiles {
			path.join(iw4of.zstd.source, "lib/legacy/**.*"),
			--path.join(zstd.source, "zlibWrapper/examples/**.*"),
		}
		defines {
			"zstd_DLL",
			"_CRT_SECURE_NO_DEPRECATE",
		}

		warnings "Off"
		kind "StaticLib"
end

