iw4of.libtomcrypt = {
	source = path.join(iw4of_dependencies.basePath, "libtomcrypt"),
}

function iw4of.libtomcrypt.import()
	links {
		"libtomcrypt"
	}

	iw4of.libtomcrypt.includes()
end

function iw4of.libtomcrypt.includes()
	includedirs {
		path.join(iw4of.libtomcrypt.source, "src/headers")
	}

	defines {
		"LTC_NO_FAST",
		"LTC_NO_PROTOTYPES",
		"LTC_NO_RSA_BLINDING",
		"LTC_NO_FILE",
		"ARGTYPE=4",
	}
end

function iw4of.libtomcrypt.project()
	project "libtomcrypt"
		language "C"

		iw4of.libtomcrypt.includes()
		iw4of.libtommath.import()

		files {
			path.join(iw4of.libtomcrypt.source, "src/**.c"),
		}

		removefiles {
			path.join(iw4of.libtomcrypt.source, "src/**/*_test.c"),
			path.join(iw4of.libtomcrypt.source, "src/**/*tab.c"),
			path.join(iw4of.libtomcrypt.source, "src/encauth/ocb3/**.c"),
		}

		defines {
			"_CRT_SECURE_NO_WARNINGS",
			"LTC_SOURCE",
			"_LIB",
			"USE_LTM"
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
