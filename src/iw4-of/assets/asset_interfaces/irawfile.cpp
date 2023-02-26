#include <std_include.hpp>

#include "assets/assets.hpp"
#include "irawfile.hpp"

#include "utils/compression.hpp"
#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

namespace iw4of::interfaces
{
    bool irawfile::write_internal(const native::XAssetHeader& header) const
    {
        RETURN_IF_NULL(header.rawfile);
        RETURN_IF_NULL(header.rawfile->buffer);

        if (header.rawfile->len > 0)
        {
            std::string uncompressed_buffer;
            if (header.rawfile->compressedLen > 0)
            {
                const auto& str = utils::compression::zlib::decompress(std::string(header.rawfile->buffer, header.rawfile->compressedLen));
                uncompressed_buffer = local_allocator.duplicate_string(str);
            }
            else
            {
                uncompressed_buffer = header.rawfile->buffer;
            }

            write_named_assets(header.rawfile->name, uncompressed_buffer);

            return utils::io::write_file(get_work_path(header).string(), std::string(uncompressed_buffer.data(), header.rawfile->len));
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
        asset->len = static_cast<int32_t>(contents.size());

        read_named_assets(asset->name, contents);

        const auto& compressedData = utils::compression::zlib::compress(contents);

        // Only save the compressed buffer if we gained space
        if (compressedData.size() < asset->len)
        {
            asset->buffer = local_allocator.allocate_array<char>(compressedData.size());
            std::memcpy(const_cast<char*>(asset->buffer), compressedData.data(), compressedData.size());
            asset->compressedLen = static_cast<int32_t>(compressedData.size());
        }
        else
        {
            asset->buffer = local_allocator.allocate_array<char>(contents.size() + 1);
            std::memcpy(const_cast<char*>(asset->buffer), contents.data(), contents.size());
            asset->compressedLen = 0;
        }

        return asset;
    }

    void irawfile::write_named_assets(const std::string& script_name, const std::string& script_data) const
    {
        if (script_name.ends_with(".gsc"))
        {
            if (script_name.ends_with("_fx.gsc"))
            {
                if (script_name.starts_with("maps/createfx/"))
                {
                    // Createfx
                    auto snds = get_create_fx_sounds(script_data);
                    for (const auto& snd : snds)
                    {
                        assets->write(native::ASSET_TYPE_SOUND, snd);
                    }
                }
                else
                {
                    // General FX
                    auto fxs = get_map_fx(script_data);
                    for (const auto& fx : fxs)
                    {
                        assets->write(native::ASSET_TYPE_FX, fx);
                    }
                }
            }
            else if (script_name.ends_with("_precache.gsc"))
            {
                auto asset_list = get_assets_in_precache(script_data);
                for (const auto& asset : asset_list)
                {
                    assets->write(asset.type, asset.header.data);
                }
            }

            const auto& anim_trees = get_map_animtrees(script_data);
            for (const auto& tree : anim_trees)
            {
                assets->write(native::ASSET_TYPE_RAWFILE, tree);
            }

            auto ambient_snd = get_ambient_play(script_data);
            if (ambient_snd)
            {
                assets->write(native::ASSET_TYPE_SOUND, ambient_snd);
            }
        }
    }

    void irawfile::read_named_assets(const std::string& script_name, const std::string& script_data) const
    {
        if (script_name.ends_with(".gsc"))
        {
            if (script_name.ends_with("_fx.gsc"))
            {
                if (script_name.starts_with("maps/createfx/"))
                {
                    // Createfx
                    get_create_fx_sounds(script_data);
                }
                else
                {
                    // General FX
                    get_map_fx(script_data);
                }
            }
            else if (script_name.ends_with("_precache.gsc"))
            {
                get_assets_in_precache(script_data);
            }

            get_map_animtrees(script_data);
            get_ambient_play(script_data);
        }
    }

    std::vector<native::RawFile*> irawfile::get_map_animtrees(const std::string& script) const
    {
        static std::regex animtree_catcher("#using_animtree\\(\"(.*)\"\\);");
        std::smatch m;

        std::string::const_iterator search_start(script.cbegin());
        std::vector<native::RawFile*> trees{};

        while (std::regex_search(search_start, script.cend(), m, animtree_catcher))
        {
            if (m.size() > 1)
            {
                const auto& match = m[1];
                auto animtree_name = std::format("animtrees/{}.atr", match.str());
                auto animtree = find<native::RawFile>(native::ASSET_TYPE_RAWFILE, animtree_name);
                search_start = m.suffix().first;

                if (animtree)
                {
                    auto new_name = std::string(animtree->name);
                    trees.push_back(animtree);
                }
                else
                {
                    assert(false);
                }
            }
        }

        return trees;
    }

    std::vector<native::XAnimParts*> irawfile::get_map_animated_model_anims(const std::string& script) const
    {
        // We use % because it's used for MP and SP (on destructibles for instance it's for both)
        // but we also have to use "" for mp_specific anims
        static std::regex anim_catchers[] = {
            std::regex("%(.*);"), // SP & MP sometimes
            std::regex("\"\\] *= \"*(.*)\";") // MP only
        };

        std::vector<native::XAnimParts*> anim_parts{};

        for (size_t i = 0; i < ARRAYSIZE(anim_catchers); i++)
        {
            const auto& anim_catcher = anim_catchers[i];
            std::smatch m;
            std::string::const_iterator search_start(script.cbegin());
            while (std::regex_search(search_start, script.cend(), m, anim_catcher))
            {
                if (m.size() > 1)
                {
                    const auto& match = m[1];
                    auto anim_name = match.str();
                    auto anim = find<native::XAnimParts>(native::ASSET_TYPE_XANIMPARTS, anim_name);
                    search_start = m.suffix().first;

                    if (anim)
                    {
                        anim_parts.push_back(anim);
                    }
                }
            }
        }

        return anim_parts;
    }

    native::snd_alias_list_t* irawfile::get_ambient_play(const std::string& script) const
    {
        static std::regex ambientplay("ambientplay\\( \"((.*))\" \\);");
        std::smatch matches;

        if (std::regex_search(script.cbegin(), script.cend(), matches, ambientplay))
        {
            if (matches.size() > 1)
            {
                const auto& match = matches[1];
                auto sound_name = match.str();

                auto sound = find<native::snd_alias_list_t>(native::ASSET_TYPE_SOUND, sound_name);

                if (sound)
                {
                    return sound;
                }
            }
        }

        return nullptr;
    }

    std::vector<native::FxEffectDef*> irawfile::get_map_fx(const std::string& script) const
    {
        static std::regex fx_catcher("\\] = loadfx\\( *\"(.+)\" *\\);");
        std::smatch m;
        std::string::const_iterator search_start(script.cbegin());

        std::vector<native::FxEffectDef*> fxs{};

        while (std::regex_search(search_start, script.cend(), m, fx_catcher))
        {
            bool skip = true;
            for (auto match : m)
            {
                if (skip)
                {
                    skip = false;
                    continue;
                }

                auto fx_name = match.str();
                auto fx = find<native::FxEffectDef>(native::ASSET_TYPE_FX, fx_name);
                search_start = m.suffix().first;

                if (fx)
                {
                    fxs.push_back(fx);
                }
            }
        }

        return fxs;
    }

    std::vector<native::XAsset> irawfile::get_assets_in_precache(const std::string& script_contents) const
    {
        static std::regex anim_script_catcher(" *(.*)::main\\(\\);");
        std::smatch anim_script_matches;

        std::vector<native::XAsset> asset_list{};

        std::string::const_iterator search_start(script_contents.cbegin());
        while (std::regex_search(search_start, script_contents.cend(), anim_script_matches, anim_script_catcher))
        {
            if (anim_script_matches.size() > 1)
            {
                const auto& match = anim_script_matches[1];
                auto script_name = std::format("{}.gsc", match.str());
                std::replace(script_name.begin(), script_name.end(), '\\', '/');

                search_start = anim_script_matches.suffix().first;

                auto script = find<native::RawFile>(native::ASSET_TYPE_RAWFILE, script_name);

                if (script)
                {
                    asset_list.push_back({native::ASSET_TYPE_RAWFILE, script});

                    std::string contents = script->buffer;
                    if (script->compressedLen)
                    {
                        contents = utils::compression::zlib::decompress(std::string(script->buffer, script->compressedLen));
                    }

                    const auto& anim_trees = get_map_animtrees(contents);
                    const auto& model_anims = get_map_animated_model_anims(contents);

                    for (const auto& tree : anim_trees)
                    {
                        asset_list.push_back({native::ASSET_TYPE_RAWFILE, tree});
                    }

                    for (const auto& anims : model_anims)
                    {
                        asset_list.push_back({native::ASSET_TYPE_XANIMPARTS, anims});
                    }
                }
            }
        }

        return asset_list;
    }

    std::vector<native::snd_alias_list_t*> irawfile::get_create_fx_sounds(const std::string& script) const
    {
        static std::regex sound_catcher("(?:\\.v\\[\"soundalias\"\\] *= *\"(.+)\")");
        std::smatch m;

        std::vector<native::snd_alias_list_t*> sounds{};

        std::string::const_iterator search_start(script.cbegin());

        while (std::regex_search(search_start, script.cend(), m, sound_catcher))
        {
            bool skip = true;
            for (auto match : m)
            {
                if (skip)
                {
                    skip = false;
                    continue;
                }

                auto sound_name = match.str();
                auto sound = find<native::snd_alias_list_t>(native::ASSET_TYPE_SOUND, sound_name);
                search_start = m.suffix().first;

                if (sound)
                {
                    sounds.push_back(sound);
                }
            }
        }

        return sounds;
    }

    std::filesystem::path irawfile::get_file_name(const std::string& basename) const
    {
        return basename;
    }
} // namespace iw4of::interfaces