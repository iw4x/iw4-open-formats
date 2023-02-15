#include <std_include.hpp>

#include "ivertexdecl.hpp"

#include <utils/memory.hpp>
#include <utils/io.hpp>
#include <utils/stream.hpp>

#include <assets/assets.hpp>

#define IW4X_TECHSET_VERSION 1

namespace iw4of::interfaces
{
	bool ivertexdecl::write_internal(const native::XAssetHeader& header) const
	{
		native::MaterialVertexDeclaration* iw4_decl = header.vertexDecl;

		for (int i = 0; i < iw4_decl->streamCount; i++)
		{
			if (iw4_decl->routing.data[i].dest >= native::MaterialStreamRoutingDestination::STREAM_DST_COUNT
				|| iw4_decl->routing.data[i].source >= native::MaterialStreamRoutingSource::STREAM_SRC_COUNT)
			{
				assert(false);
			}
		}

		utils::stream buffer;
		buffer.save_array("IW4xDECL", 8);
		buffer.save_object(static_cast<char>(IW4X_TECHSET_VERSION));

		buffer.save_object(*iw4_decl);

		if (iw4_decl->name)
		{
			buffer.save_string(iw4_decl->name);
		}

		utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());

		return iw4_decl;

		return true;
	}

	void* ivertexdecl::read_internal(const std::string& name) const
	{
		auto path = get_work_path(name).string();

		if (utils::io::file_exists(path))
		{
			auto contents = utils::io::read_file(path);
			utils::stream::reader reader(&local_allocator, contents);

			char* magic = reader.read_array<char>(8);
			if (std::memcmp(magic, "IW4xDECL", 8))
			{
				print_error("Reading vertex declaration '{}' failed, header is invalid!", name);
				return nullptr;
			}

			auto version = reader.read<char>();
			if (version > IW4X_TECHSET_VERSION)
			{
				print_error("Reading vertex declaration '{}' failed, expected version is {}, but it was {:d}!",
					name, IW4X_TECHSET_VERSION, version);
				return nullptr;
			}

			native::MaterialVertexDeclaration* asset = reader.read_object<native::MaterialVertexDeclaration>();

			if (asset->name)
			{
				asset->name = reader.read_cstring();
			}

			return asset;
		}

		return nullptr;
	}

	std::filesystem::path ivertexdecl::get_file_name(const std::string& basename) const
	{
		return std::format("{}.iw4xDECL", basename);
	}		
}