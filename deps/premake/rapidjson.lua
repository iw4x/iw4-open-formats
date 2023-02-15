iw4of.rapidjson = {
	source = path.join(iw4of_dependencies.basePath, "rapidjson"),
}

function iw4of.rapidjson.import()
	iw4of.rapidjson.includes()
end

function iw4of.rapidjson.includes()
	includedirs {
		path.join(iw4of.rapidjson.source, "include"),
	}
end

function iw4of.rapidjson.project()

end
