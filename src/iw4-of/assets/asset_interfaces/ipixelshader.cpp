#include <std_include.hpp>

#include "ipixelshader.hpp"

#include <utils/memory.hpp>
#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <assets/assets.hpp>

#define GFX_RENDERER_SHADER_SM3 0

namespace iw4of::interfaces
{
	bool ipixelshader::write_internal(const native::XAssetHeader& header) const
	{
		auto ps = header.pixelShader;
		if (!ps) return false;

		utils::stream buffer;
		buffer.save_array(ps->prog.loadDef.program, ps->prog.loadDef.programSize);

		utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());

		return true;
	}

	void* ipixelshader::read_internal(const std::string& name) const
	{
		auto path = get_work_path(name).string();

		if (utils::io::file_exists(path))
		{
			auto ps = local_allocator.allocate<native::MaterialPixelShader>();
			ps->name = local_allocator.duplicate_string(name);

			auto size = utils::io::file_size(path);
			const auto& buff = utils::io::read_file(path);


			ps->prog.loadDef.loadForRenderer = GFX_RENDERER_SHADER_SM3;
			ps->prog.loadDef.programSize = static_cast<unsigned short>(size);
			ps->prog.loadDef.program = reinterpret_cast<unsigned int*>(local_allocator.allocate(size));
			memcpy_s(ps->prog.loadDef.program, size, buff.data(), buff.size());

			return ps;
		}

		return nullptr;
	}

	std::filesystem::path ipixelshader::get_file_name(const std::string& basename) const
	{
		return std::format("{}.cso", basename);
	}
}