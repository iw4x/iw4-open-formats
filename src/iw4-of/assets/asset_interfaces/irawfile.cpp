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
                uncompressed_buffer = std::string(local_allocator.duplicate_string(str), header.rawfile->len);
            }
            else
            {
                uncompressed_buffer = std::string(header.rawfile->buffer, header.rawfile->len);
            }

            write_named_assets(header.rawfile->name, uncompressed_buffer);

            const auto work_path = get_work_path(header).string();
            const auto data = std::string(uncompressed_buffer.data(), header.rawfile->len);
            return utils::io::write_file(work_path, data);
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
		const auto children = get_child_assets(script_name, script_data);
		for(const auto& child : children)
		{
			assets->write(child.type, child.header.data);
		}
    }

    void irawfile::read_named_assets(const std::string& script_name, const std::string& script_data) const {
		
		// And this should be enough
		get_child_assets(script_name, script_data);
	}

    std::vector<native::XAsset> irawfile::get_child_assets(const native::XAssetHeader& header) const
    {
        const std::string script_name = header.rawfile->name;
        std::string script_data;

        // Decompress
        if (header.rawfile->compressedLen > 0)
        {
            const auto& str = utils::compression::zlib::decompress(std::string(header.rawfile->buffer, header.rawfile->compressedLen));
            script_data = std::string(local_allocator.duplicate_string(str), header.rawfile->len);
        }
        else
        {
            script_data = std::string(header.rawfile->buffer, header.rawfile->len);
        }

		return get_child_assets(script_name, script_data);
    }

	std::vector<native::XAsset> irawfile::get_child_assets(const std::string& script_name, const std::string& script_data) const
	{
        auto result = std::vector<native::XAsset>();

        if (script_name.ends_with(".gsc"))
        {
            if (script_name.ends_with("_fx.gsc"))
            {
                if (script_name.starts_with("maps/createfx/"))
                {
                    // Createfx
                    const auto sounds = get_create_fx_sounds(script_data);
                    for (const auto& sound : sounds)
                    {
                        result.push_back({native::ASSET_TYPE_SOUND, sound});
                    }
                }
                else
                {
                    // General FX
                    const auto fxs = get_map_fx(script_data);
                    for (const auto& fx : fxs)
                    {
                        result.push_back({native::ASSET_TYPE_FX, fx});
                    }
                }
            }
            else if (script_name.ends_with("_precache.gsc"))
            {
                const auto precache = get_assets_in_precache(script_data);
                for (const auto& asset : precache)
                {
                    result.push_back(asset);
                }
            }

            const auto anim_trees = get_map_animtrees(script_data);
            for (const auto& anim_tree : anim_trees)
            {
                result.push_back({native::ASSET_TYPE_RAWFILE, anim_tree});
            }

            const auto ambient = get_ambient_play(script_data);
			if (ambient)
			{
                result.push_back({native::ASSET_TYPE_SOUND, ambient});
			}
        }
        else if (script_name.ends_with(".atr"))
        {
            const auto anims = get_animtree_anims(script_data);
            for (const auto& anim : anims)
            {
                result.push_back({native::ASSET_TYPE_XANIMPARTS, anim});
            }
        }

        return result;
	}

	std::vector<native::XAnimParts*> irawfile::get_animtree_anims(const std::string& script) const
    {
        auto result = std::vector<native::XAnimParts*>();
        std::istringstream iss(script);

        for (std::string line; std::getline(iss, line);)
        {
            line = line.erase(0, line.find_first_not_of(" \n\r\t"));
			while (line.length() > 0 && (line.ends_with("\n") || line.ends_with("\r") || line.ends_with(" ")))
			{
				line.pop_back();
			}

            if (line.length() > 0 && line[0] != '/' && line[1] != '/')
            {
                const auto parts = find<native::XAnimParts>(iw4of::native::XAssetType::ASSET_TYPE_XANIMPARTS, line);
                if (parts)
                {
                    result.push_back(parts);
                }
            }
        }

        return result;
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
