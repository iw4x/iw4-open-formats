#include <std_include.hpp>

#include "irawfile.hpp"

#include "utils/compression.hpp"
#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"


namespace iw4of::interfaces
{
	bool irawfile::write_internal(const native::XAssetHeader& header) const
	{
		RETURN_IF_NULL(header.rawfile);
		RETURN_IF_NULL(header.rawfile->buffer);

		if (header.rawfile->len > 0)
		{
			char* uncompressed_buffer;
			if (header.rawfile->compressedLen > 0)
			{
				const auto& str = utils::compression::zlib::decompress(std::string(header.rawfile->buffer, header.rawfile->compressedLen));
				uncompressed_buffer = local_allocator.duplicate_string(str);
			}
			else
			{
				uncompressed_buffer = header.rawfile->buffer;
			}

			return utils::io::write_file(get_work_path(header).string(), std::string(uncompressed_buffer, header.rawfile->len));
		}

		// Happens
		return true;
	}
	void* irawfile::read_internal(const std::string& name) const
	{
		const auto& path = get_work_path(name).string();

		if (!utils::io::file_exists(path))
		{
			return nullptr;
		}

		const auto& contents = utils::io::read_file(path);

		auto* asset = local_allocator.allocate<native::RawFile>();

		RETURN_IF_NULL(asset);

		asset->name = local_allocator.duplicate_string(name);
		asset->len = static_cast<int>(contents.size());

		const auto& compressedData = utils::compression::zlib::compress(contents);

		// Only save the compressed buffer if we gained space
		if (compressedData.size() < asset->len)
		{
			asset->buffer = local_allocator.allocate_array<char>(compressedData.size());
			std::memcpy(const_cast<char*>(asset->buffer), compressedData.data(), compressedData.size());
			asset->compressedLen = static_cast<int>(compressedData.size());
		}
		else
		{
			asset->buffer = local_allocator.allocate_array<char>(contents.size() + 1);
			std::memcpy(const_cast<char*>(asset->buffer), contents.data(), contents.size());
			asset->compressedLen = 0;
		}

		return asset;
	}

	std::filesystem::path irawfile::get_file_name(const std::string& basename) const
	{
		return basename;
	}
}