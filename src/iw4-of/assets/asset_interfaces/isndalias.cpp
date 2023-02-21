#include <std_include.hpp>

#include "isndalias.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <assets/assets.hpp>

namespace iw4of::interfaces
{
    void* interfaces::isndalias::read_internal(const std::string& name) const
    {
        const auto& path = get_work_path(name).string();

        auto* alias_list = local_allocator.allocate<native::snd_alias_list_t>();
        if (!alias_list)
        {
            print_error("Failed to allocate memory for sound alias structure!\n");
            return nullptr;
        }

        if (!utils::io::file_exists(path))
        {
            return nullptr;
        }

        rapidjson::Document infoData;
        try
        {
            const auto& contents = utils::io::read_file(path);
            infoData.Parse(contents.data());
        }
        catch (const std::exception& ex)
        {
            print_error("Json Parse Error: {}\n", ex.what());
            return nullptr;
        }

        const auto& aliasesContainer = infoData["head"];
        const auto& aliases = aliasesContainer;

        alias_list->count = aliases.Size();

        // Allocate
        alias_list->head = local_allocator.allocate_array<native::snd_alias_t>(alias_list->count);
        if (!alias_list->head)
        {
            print_error("Error allocating memory for sound alias structure!\n");
            return nullptr;
        }

        alias_list->aliasName = local_allocator.duplicate_string(name);

        for (size_t i = 0; i < alias_list->count; i++)
        {
            const auto& head = aliasesContainer[i];

            if (!infoData.IsObject())
            {
                print_error("Failed to load sound {}!", name);
                return nullptr;
            }

            alias_list->head->soundFile = local_allocator.allocate<native::SoundFile>();
            if (!alias_list->head->soundFile)
            {
                print_error("Error allocating memory for sound alias structure!\n");
                return nullptr;
            }

            native::snd_alias_t* alias = alias_list->head;

            // try and parse everything and if it fails then fail for the whole file
            const auto& type = head["type"];
            const auto& subtitle = head["subtitle"];
            const auto& secondaryAliasName = head["secondaryAliasName"];
            const auto& chainAliasName = head["chainAliasName"];
            const auto& sequence = head["sequence"];
            const auto& volMin = head["volMin"];
            const auto& volMax = head["volMax"];
            const auto& pitchMin = head["pitchMin"];
            const auto& pitchMax = head["pitchMax"];
            const auto& distMin = head["distMin"];
            const auto& distMax = head["distMax"];
            const auto& flags = head["flags"];
            const auto& slavePercentage = head["slavePercentage"];
            const auto& probability = head["probability"];
            const auto& lfePercentage = head["lfePercentage"];
            const auto& centerPercentage = head["centerPercentage"];
            const auto& startDelay = head["startDelay"];
            const auto& volumeFalloffCurve = head["volumeFalloffCurve"];
            const auto& envelopMin = head["envelopMin"];
            const auto& envelopMax = head["envelopMax"];
            const auto& envelopPercentage = head["envelopPercentage"];
            const auto& speakerMap = head["speakerMap"];

            std::string sound_file;

            // Fix casing
            if (head["soundFile"].IsNull())
            {
                sound_file = head["soundfile"].GetString();

                print("Fixed casing on {}", name);
            }
            else
            {
                sound_file = head["soundFile"].GetString();
            }

            if (type.IsNull())
            {
                print_error("Failed to parse sound {}! Each alias must have at least a type and a soundFile\n", name);
                return nullptr;
            }

            alias->soundFile->exists = true;

            // These must be THE SAME POINTER !!
            // Wanna know why ? Check out 0x685646
            alias->aliasName = alias_list->aliasName;

            if (subtitle.IsString())
            {
                alias->subtitle = local_allocator.duplicate_string(subtitle.GetString());
            }
            if (secondaryAliasName.IsString())
            {
                alias->secondaryAliasName = local_allocator.duplicate_string(secondaryAliasName.GetString());
            }
            if (chainAliasName.IsString())
            {
                alias->chainAliasName = local_allocator.duplicate_string(chainAliasName.GetString());
            }

            alias->sequence = sequence.Get<int32_t>();
            alias->volMin = volMin.Get<float>();
            alias->volMax = volMax.Get<float>();
            alias->pitchMin = pitchMin.Get<float>();
            alias->pitchMax = pitchMax.Get<float>();
            alias->distMin = distMin.Get<float>();
            alias->distMax = distMax.Get<float>();
            alias->flags.intValue = flags.Get<int32_t>();
            alias->slavePercentage = slavePercentage.Get<float>();
            alias->probability = probability.Get<float>();
            alias->lfePercentage = lfePercentage.Get<float>();
            alias->centerPercentage = centerPercentage.Get<float>();
            alias->startDelay = startDelay.Get<int32_t>();
            alias->envelopMin = envelopMin.Get<float>();
            alias->envelopMax = envelopMax.Get<float>();
            alias->envelopPercentage = envelopPercentage.Get<float>();

            // Speaker map object
            if (!speakerMap.IsNull())
            {
                alias->speakerMap = local_allocator.allocate<native::SpeakerMap>();
                if (!alias->speakerMap)
                {
                    print_error("Error allocating memory for speakermap in sound alias{}!\n", alias->aliasName);
                    return nullptr;
                }

                alias->speakerMap->name = local_allocator.duplicate_string(speakerMap["name"].GetString());
                alias->speakerMap->isDefault = speakerMap["isDefault"].Get<bool>();

                if (speakerMap["channelMaps"].IsArray())
                {
                    const auto& channelMaps = speakerMap["channelMaps"];

                    assert(channelMaps.Size() <= 4);

                    // channelMapIndex should never exceed 1
                    for (size_t channelMapIndex = 0; channelMapIndex < 2; channelMapIndex++)
                    {
                        // subChannelIndex should never exceed 1
                        for (size_t subChannelIndex = 0; subChannelIndex < 2; subChannelIndex++)
                        {
                            const auto& channelMap = channelMaps[channelMapIndex * 2 + subChannelIndex]; // 0-3

                            const auto& speakers = channelMap["speakers"];

                            alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakerCount = speakers.Size();

                            for (auto speakerIndex = 0; speakerIndex < alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakerCount;
                                 speakerIndex++)
                            {
                                const auto& speaker = speakers[speakerIndex];
                                alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakers[speakerIndex].levels[0] =
                                    speaker["levels0"].Get<float>();
                                alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakers[speakerIndex].levels[1] =
                                    speaker["levels1"].Get<float>();
                                alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakers[speakerIndex].numLevels =
                                    speaker["numLevels"].Get<int32_t>();
                                alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakers[speakerIndex].speaker =
                                    speaker["speaker"].Get<int32_t>();
                            }
                        }
                    }
                }
            }

            if (volumeFalloffCurve.IsString())
            {
                std::string fallOffCurve = volumeFalloffCurve.GetString();

                if (fallOffCurve.empty())
                {
                    fallOffCurve = "$default";
                }

                auto curve = find<native::SndCurve>(native::XAssetType::ASSET_TYPE_SOUND_CURVE, fallOffCurve);

                RETURN_IF_NULL(curve);
                alias->volumeFalloffCurve = curve;
            }

            if (static_cast<native::snd_alias_type_t>(type.Get<int32_t>()) == native::snd_alias_type_t::SAT_LOADED) // Loaded
            {
                alias->soundFile->type = native::SAT_LOADED;
                alias->soundFile->u.loadSnd = find<native::LoadedSound>(native::XAssetType::ASSET_TYPE_LOADED_SOUND, sound_file);
            }
            else if (static_cast<native::snd_alias_type_t>(type.Get<int32_t>()) == native::snd_alias_type_t::SAT_STREAMED) // Streamed
            {
                alias->soundFile->type = native::SAT_STREAMED;

                std::string streamedFile = sound_file;
                std::string directory = ""s;
                int32_t split = streamedFile.find_last_of('/');

                if (split >= 0)
                {
                    directory = streamedFile.substr(0, split);
                    streamedFile = streamedFile.substr(split + 1);
                }

                alias->soundFile->u.streamSnd.name = local_allocator.duplicate_string(streamedFile.c_str());
                alias->soundFile->u.streamSnd.dir = local_allocator.duplicate_string(directory.c_str());
            }
            else
            {
                print_error("Failed to parse sound {}! Invalid sound type {}", name, type.GetString());
                return nullptr;
            }

            alias_list->head[i] = *alias;
        }

        return alias_list;
    }

    bool isndalias::write_internal(const native::XAssetHeader& header) const
    {
        auto ents = header.sound;

        if (ents->count == 0)
        {
            // Very weird but it happens, notably on mp_crash_snow
            // Soundaliases with a zero-count list crash iw4 so we skip them
            // They should be empty anyway
            return false;
        }

        rapidjson::Document output(rapidjson::kObjectType);
        auto& allocator = output.GetAllocator();
        utils::memory::allocator str_duplicator;

        // Format is pretty transparent from iw3 to iw4, so no conversion is
        // necessary!
        rapidjson::Value head(rapidjson::kArrayType);

        for (size_t i = 0; i < ents->count; i++)
        {
            auto alias = ents->head[i];

            rapidjson::Value channel_maps(rapidjson::kArrayType);

            for (size_t j = 0; j < 2; j++)
            {
                for (size_t k = 0; k < 2; k++)
                {
                    auto channel_map = alias.speakerMap->channelMaps[j][k];
                    rapidjson::Value speakers(rapidjson::kArrayType);

                    for (auto speaker_index = 0; speaker_index < channel_map.speakerCount; speaker_index++)
                    {
                        auto game_speaker = channel_map.speakers[speaker_index];

                        rapidjson::Value speaker(rapidjson::kObjectType);
                        speaker.AddMember("levels0", game_speaker.numLevels > 0 ? game_speaker.levels[0] : 0, allocator);
                        speaker.AddMember("levels1", game_speaker.numLevels > 1 ? game_speaker.levels[1] : 0, allocator);
                        speaker.AddMember("numLevels", game_speaker.numLevels, allocator);
                        speaker.AddMember("speaker", game_speaker.speaker, allocator);
                        speakers.PushBack(speaker, allocator);
                    }

                    rapidjson::Value channelMap(rapidjson::kObjectType);
                    channelMap.AddMember("entryCount", channel_map.speakerCount, allocator);
                    channelMap.AddMember("speakers", speakers, allocator);
                    channel_maps.PushBack(channelMap, allocator);
                }
            }

            rapidjson::Value speakerMap(rapidjson::kObjectType);
            speakerMap.AddMember("channelMaps", channel_maps, allocator);
            speakerMap.AddMember("isDefault", alias.speakerMap->isDefault, allocator);
            speakerMap.AddMember("name", RAPIDJSON_STR(alias.speakerMap->name), allocator);

            std::string file_name{};
            if (alias.soundFile)
            {
                switch (alias.soundFile->type)
                {
                    // LOADED
                    case native::snd_alias_type_t::SAT_LOADED:
                    {
                        file_name = alias.soundFile->u.loadSnd->name;
                        assets->write(native::ASSET_TYPE_LOADED_SOUND, alias.soundFile->u.loadSnd);
                    }
                    break;

                    // STREAMED
                    case native::snd_alias_type_t::SAT_STREAMED:
                    {
                        file_name = alias.soundFile->u.streamSnd.name;

                        if (alias.soundFile->u.streamSnd.dir)
                        {
                            file_name = std::format("{}/{}", alias.soundFile->u.streamSnd.dir, file_name);
                        }
                    }
                    break;
                }
            }

            if (alias.volumeFalloffCurve)
            {
                assets->write(native::ASSET_TYPE_SOUND_CURVE, alias.volumeFalloffCurve);
            }

            rapidjson::Value json_alias(rapidjson::kObjectType);
            json_alias.AddMember("aliasName", RAPIDJSON_STR(alias.aliasName), allocator);
            json_alias.AddMember("centerPercentage", alias.centerPercentage, allocator);
            json_alias.AddMember("chainAliasName", RAPIDJSON_STR(alias.chainAliasName), allocator);
            json_alias.AddMember("distMax", alias.distMax, allocator);
            json_alias.AddMember("distMin", alias.distMin, allocator);
            json_alias.AddMember("envelopMax", alias.envelopMax, allocator);
            json_alias.AddMember("envelopMin", alias.envelopMin, allocator);
            json_alias.AddMember("envelopPercentage", alias.envelopPercentage, allocator);
            json_alias.AddMember("flags", alias.flags.intValue, allocator);
            json_alias.AddMember("lfePercentage", alias.lfePercentage, allocator);
            json_alias.AddMember("mixerGroup", rapidjson::Value(rapidjson::kNullType), allocator);
            json_alias.AddMember("pitchMax", alias.pitchMax, allocator);
            json_alias.AddMember("pitchMin", alias.pitchMin, allocator);
            json_alias.AddMember("probability", alias.probability, allocator);
            json_alias.AddMember("secondaryAliasName", RAPIDJSON_STR(alias.secondaryAliasName), allocator);
            json_alias.AddMember("sequence", alias.sequence, allocator);
            json_alias.AddMember("slavePercentage", alias.slavePercentage, allocator);
            json_alias.AddMember("speakerMap", speakerMap, allocator);
            json_alias.AddMember("soundFile", RAPIDJSON_STR(str_duplicator.duplicate_string(file_name)), allocator);
            json_alias.AddMember("startDelay", alias.startDelay, allocator);
            json_alias.AddMember("subtitle", RAPIDJSON_STR(alias.subtitle), allocator);
            json_alias.AddMember("type", alias.soundFile->type, allocator);
            json_alias.AddMember("volMax", alias.volMax, allocator);
            json_alias.AddMember("volMin", alias.volMin, allocator);
            json_alias.AddMember("volumeFalloffCurve", RAPIDJSON_STR(alias.volumeFalloffCurve->filename), allocator);

            head.PushBack(json_alias, allocator);
        }

        output.AddMember("aliasName", RAPIDJSON_STR(ents->aliasName), allocator);
        output.AddMember("count", ents->count, allocator);
        output.AddMember("head", head, allocator);

        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
        output.Accept(writer);

        const auto& dump = buff.GetString();

        utils::io::write_file(get_work_path(header).string(), dump);
        return true;
    }

    std::filesystem::path interfaces::isndalias::get_file_name(const std::string& asset_name) const
    {
        return std::format("{}.json", asset_name);
    }

    std::filesystem::path interfaces::isndalias::get_folder_name() const
    {
        return "sounds";
    }
} // namespace iw4of::interfaces