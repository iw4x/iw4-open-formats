#include <std_include.hpp>

#include "ivertexshader.hpp"

#include <utils/memory.hpp>
#include <utils/io.hpp>
#include <utils/stream.hpp>

#include <assets/assets.hpp>

#define GFX_RENDERER_SHADER_SM3 0

namespace iw4of::interfaces
{
    bool ivertexshader::write_internal(const native::XAssetHeader& header) const
    {
        auto vs = header.vertexShader;
        if (!vs) return false;

        utils::stream buffer;
        buffer.save_array(vs->prog.loadDef.program, vs->prog.loadDef.programSize);

        utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());

        return true;
    }

    void* ivertexshader::read_internal(const std::string& name) const
    {
        auto path = get_work_path(name).string();

        if (utils::io::file_exists(path))
        {
            auto vs = local_allocator.allocate<native::MaterialVertexShader>();
            vs->name = local_allocator.duplicate_string(name);

            auto size = utils::io::file_size(path);
            const auto& buff = utils::io::read_file(path);

            vs->prog.loadDef.loadForRenderer = GFX_RENDERER_SHADER_SM3;
            vs->prog.loadDef.programSize = static_cast<uint16_t>(size / sizeof(uint32_t));
            vs->prog.loadDef.program = local_allocator.allocate_array<uint32_t>(vs->prog.loadDef.programSize);
            memcpy_s(vs->prog.loadDef.program, size, buff.data(), buff.size());

            return vs;
        }

        return nullptr;
    }

    std::filesystem::path ivertexshader::get_file_name(const std::string& basename) const
    {
        return std::format("{}.cso", basename);
    }

} // namespace iw4of::interfaces