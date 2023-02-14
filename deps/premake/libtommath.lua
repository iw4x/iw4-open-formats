iw4of.libtommath = {
	source = path.join(iw4of_dependencies.basePath, "libtommath"),
}

function iw4of.libtommath.import()
	links {
		"libtommath"
	}

	iw4of.libtommath.includes()
end

function iw4of.libtommath.includes()
	includedirs {
		iw4of.libtommath.source
	}

	defines {
		"LTM_DESC",
		"__STDC_IEC_559__",
		"MP_NO_DEV_URANDOM",
	}
end

function iw4of.libtommath.project()
	project "libtommath"
		language "C"

		iw4of.libtommath.includes()

		files {
			path.join(iw4of.libtommath.source, "*.c"),
		}

		defines {
			"_LIB"
		}

		removedefines {
			"_DLL",
			"_USRDLL"
		}

		linkoptions {
			"-IGNORE:4221"
		}

		warnings "Off"
		kind "StaticLib"
end

