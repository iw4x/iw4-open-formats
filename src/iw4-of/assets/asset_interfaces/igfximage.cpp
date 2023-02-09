#include <std_include.hpp>

#include "igfximage.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#define IW4X_IMG_VERSION 1

namespace iw4of::interfaces
{
	std::filesystem::path igfximage::get_file_name(const native::XAssetHeader& header) const
	{
		return is_special(header.image) ? get_special_file_name(header.image->name) : get_file_name(header.image->name);
	}

	std::filesystem::path igfximage::get_file_name(const std::string& name) const
	{
		return std::format("{}.iwi", name);
	}

	std::filesystem::path igfximage::get_special_file_name(const std::string& name) const
	{
		return std::format("{}.iw4xImage", name);
	}

	void* igfximage::read_internal(const std::string& name) const
	{
		auto image = local_allocator.allocate<native::GfxImage>();
		if (!image)
		{
			print_error("Failed to allocate GfxImage structure!");
			return nullptr;
		}

		image->name = local_allocator.duplicate_string(name);
		image->semantic = native::IMG_CATEGORY_LOAD_FROM_FILE;

		const char* temp_name = image->name;
		if (temp_name[0] == '*') temp_name++;

		const auto special_path = get_special_file_name(temp_name).string();
		if (utils::io::file_exists(special_path))
		{
			auto contents = utils::io::read_file(special_path);
			auto reader = utils::stream::reader(
				&local_allocator, 
				contents
			);

			void* magic = reader.read(8);
			if (std::memcmp(&magic, "IW4xImg", 7))
			{
				print_error("Reading image '{}' failed, header is invalid!", name);
				return nullptr;
			}

			unsigned char version = reinterpret_cast<char*>(magic)[7];
			bool isLegacyZeroVersion = false;

			if (version == '0')
			{
				// Legacy
				isLegacyZeroVersion = true;
			}
			else if (version > IW4X_IMG_VERSION)
			{
				print_error("Reading image '{}' failed, image version is too new! expected {} and got {}!", IW4X_IMG_VERSION, version);
				return nullptr;
			}

			image->mapType = reader.read<char>();
			image->semantic = reader.read<native::GfxImageCategory>();
			image->category = reader.read<char>();

			int dataLength = reader.read<int>();
			image->cardMemory.platform[0] = dataLength;
			image->cardMemory.platform[1] = dataLength;

			image->texture.loadDef = local_allocator.allocate<iw4of::native::GfxImageLoadDef>();

			if (isLegacyZeroVersion)
			{
				struct LegacyLoadDef
				{
					char levelCount;
					char flags;
					__int16 dimensions[3];
					int format;
					int resourceSize;
					char data[1];
				};

				LegacyLoadDef loadDef = *reinterpret_cast<LegacyLoadDef*>(reader.read_array<char>(dataLength + 16));

				image->texture.loadDef->levelCount = loadDef.levelCount;
				image->texture.loadDef->flags = loadDef.flags;
				image->texture.loadDef->format = loadDef.format;
				image->texture.loadDef->resourceSize = loadDef.resourceSize;

				image->width = loadDef.dimensions[0];
				image->height = loadDef.dimensions[1];
				image->depth = loadDef.dimensions[2];
			}
			else
			{
				image->texture.loadDef->flags = reader.read<int>();
				image->width = reader.read<unsigned short>();
				image->height = reader.read<unsigned short>();
				image->depth = reader.read<unsigned short>();

				image->texture.loadDef->format = reader.read<int>();
			}

			ZeroMemory(image->texture.loadDef->pad, 3);

			if (image->texture.loadDef->resourceSize != dataLength)
			{
				print_error("Resource size doesn't match the data length ({})!\n", name);
			}

			image->delayLoadPixels = true;

			return image;
		}
		else if (name[0] != '*')
		{
			const auto file_path = get_file_name(name).string();

			if (!utils::io::file_exists(file_path))
			{
				print_error("Loading image '{}' failed!", file_path);
				return nullptr;
			}

			auto iwiBuffer = utils::io::read_file(file_path);

			const native::GfxImageFileHeader* iwiHeader = reinterpret_cast<const native::GfxImageFileHeader*>(iwiBuffer.data());

			if (std::memcmp(iwiHeader->tag, "IWi", 3) && iwiHeader->version == 8)
			{
				print_error("Image is not a valid IWi!");
				return nullptr;
			}

			image->mapType = native::IMG_FLAG_MAPTYPE_2D;
			image->cardMemory.platform[0] = iwiHeader->fileSizeForPicmip[0] - 32;
			image->cardMemory.platform[1] = iwiHeader->fileSizeForPicmip[0] - 32;

			image->texture.loadDef = local_allocator.allocate<native::GfxImageLoadDef>();
			if (!image->texture.loadDef)
			{
				print_error("Failed to allocate GfxImageLoadDef structure!");
				return nullptr;
			}

			image->texture.loadDef->flags = iwiHeader->flags;
			image->texture.loadDef->levelCount = 0;

			image->width = iwiHeader->dimensions[0];
			image->height = iwiHeader->dimensions[1];
			image->depth = iwiHeader->dimensions[2];

			switch (iwiHeader->format)
			{
			case native::IMG_FORMAT_BITMAP_RGBA:
			{
				image->texture.loadDef->format = 21;
				break;
			}

			case native::IMG_FORMAT_BITMAP_RGB:
			{
				image->texture.loadDef->format = 20;
				break;
			}

			case native::IMG_FORMAT_DXT1:
			{
				image->texture.loadDef->format = 0x31545844;
				break;
			}

			case native::IMG_FORMAT_DXT3:
			{
				image->texture.loadDef->format = 0x33545844;
				break;
			}

			case native::IMG_FORMAT_DXT5:
			{
				image->texture.loadDef->format = 0x35545844;
				break;
			}

			default:
			{
				break;
			}
			}

			return image;
		}

		return nullptr;
	}

	bool igfximage::write_internal(const native::XAssetHeader& header) const
	{
		auto image = header.image;
		std::string name = image->name;

		if (is_special(image))
		{
			if (name[0] == '*') name.erase(name.begin());

			if (name == "reflection_probe0"s)
			{
				// Reflection probe 0 is "all red" for IW3, IW4, and IW5
				// This is a sort of... tradition, i assume
				// But some IW5 maps like mp_nola still _use_ this reflection probe
				// On IW5 this is probably supported, but in IW4 it makes it look all red
				// Let's skip it and write RP1 instead over RP0.
				return false;
			}

			utils::stream buffer;
			buffer.save_array("IW4xImg", 7); // just stick version in the magic since we have an extra char
			buffer.save_byte(static_cast<unsigned char>(IW4X_IMG_VERSION));

			buffer.save_object(static_cast<unsigned char>(image->mapType));
			buffer.save_object(image->semantic);
			buffer.save_object(image->category);

			buffer.save_object(image->texture.loadDef->resourceSize);

			buffer.save_object(image->texture.loadDef->levelCount);
			buffer.save_object(image->texture.loadDef->flags);
			buffer.save_object(image->width);
			buffer.save_object(image->height);
			buffer.save_object(image->depth);
			buffer.save_object(image->texture.loadDef->format);

			buffer.save(image->texture.loadDef->data, image->texture.loadDef->resourceSize);

			utils::io::write_file(get_file_name(header).string(), buffer.to_buffer());
			
			if (name.starts_with("reflection_probe"))
			{
				auto path_to_rp0 = get_special_file_name("reflection_probe0").string();
				if (!utils::io::file_exists(path_to_rp0))
				{
					utils::io::write_file(path_to_rp0, buffer.to_buffer());
				}
			}
		}
		else
		{
			const auto game_file_path = std::format("images/{}.iwi", image->name);
			// > 10MB cannot use FS_FileRead (breaks hunk)
			auto contents = assets->read_file(game_file_path.data());

			if (contents.empty())
			{
				// Ignore that
				if (std::string(image->name).starts_with("watersetup")) {
					return false;
				}

				print("Image %s not found, mapping to normalmap!\n", name.data());

				contents = assets->read_file("images/$identitynormalmap.iwi");
			}

			if (contents.size() > 0)
			{
				utils::io::write_file(get_work_path(header).string(), contents);
			}
			else
			{
				print_error("Unable to map to normalmap, this should not happen!\n");
				return false;
			}
		}

		return true;
	}


}