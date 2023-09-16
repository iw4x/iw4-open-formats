#include <std_include.hpp>

#include "ifx.hpp"
#include "itechniqueset.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>
#include <utils/json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#define IW4X_FX_VERSION 3

namespace iw4of::interfaces
{
    bool ifx::write_internal(const native::XAssetHeader& header) const
    {
        AssertSize(native::FxEffectDef, 32);

        auto asset = header.fx;
        rapidjson::Document output(rapidjson::kObjectType);
        auto& allocator = output.GetAllocator();
        utils::memory::allocator str_duplicator;
		
        output.AddMember("version", IW4X_FX_VERSION, allocator);
        output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

        output.AddMember("flags", asset->flags, allocator);
        output.AddMember("totalSize", asset->totalSize, allocator);
        output.AddMember("msecLoopingLife", asset->msecLoopingLife, allocator);
        output.AddMember("elemDefCountLooping", asset->elemDefCountLooping, allocator);
        output.AddMember("elemDefCountOneShot", asset->elemDefCountOneShot, allocator);
        output.AddMember("elemDefCountEmission", asset->elemDefCountEmission, allocator);

        rapidjson::Value elemTable(rapidjson::kArrayType);
        if (asset->elemDefs)
        {
            for (int i = 0; i < (asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot); ++i)
            {
                rapidjson::Value json_elem_def(rapidjson::kObjectType);
                native::FxElemDef* elem_def = &asset->elemDefs[i];

                json_elem_def.AddMember("flags", elem_def->flags, allocator);

                {
                    rapidjson::Value spawn_def(rapidjson::kObjectType);
                    spawn_def.AddMember("A", elem_def->spawn.looping.count, allocator);
                    spawn_def.AddMember("B", elem_def->spawn.looping.intervalMsec, allocator);
                    json_elem_def.AddMember("spawn", spawn_def, allocator);
                }

#define SAVE_PROPERTY(x) json_elem_def.AddMember(#x, elem_def->x, allocator)
#define SAVE_RANGE(x) json_elem_def.AddMember(#x, utils::json::to_json(elem_def->x, allocator), allocator)
#define SAVE_RANGE_MULTIPLE(x)                                                        \
    {                                                                                 \
        rapidjson::Value arr(rapidjson::kArrayType);                                  \
        for (int _ = 0; _ < ARRAYSIZE(elem_def->x); _++)                              \
            arr.PushBack(utils::json::to_json(elem_def->x[_], allocator), allocator); \
        json_elem_def.AddMember(#x, arr, allocator);                                  \
    }

                SAVE_RANGE(spawnRange);
                SAVE_RANGE(fadeInRange);
                SAVE_RANGE(fadeOutRange);
                SAVE_PROPERTY(spawnFrustumCullRadius);

                SAVE_RANGE(spawnDelayMsec);
                SAVE_RANGE(lifeSpanMsec);
                SAVE_RANGE_MULTIPLE(spawnOrigin);

                SAVE_RANGE(spawnOffsetRadius);
                SAVE_RANGE(spawnOffsetHeight);
                SAVE_RANGE_MULTIPLE(spawnAngles);
                SAVE_RANGE_MULTIPLE(angularVelocity);
                SAVE_RANGE(initialRotation);
                SAVE_RANGE(gravity);
                SAVE_RANGE(reflectionFactor);

                // Save Atlas
                {
#define SAVE_ATLAS_PROPERTY(x) atlas.AddMember(#x, elem_def->atlas.x, allocator)
                    rapidjson::Value atlas(rapidjson::kObjectType);
                    SAVE_ATLAS_PROPERTY(behavior);
                    SAVE_ATLAS_PROPERTY(index);
                    SAVE_ATLAS_PROPERTY(fps);
                    SAVE_ATLAS_PROPERTY(loopCount);
                    SAVE_ATLAS_PROPERTY(colIndexBits);
                    SAVE_ATLAS_PROPERTY(rowIndexBits);
                    SAVE_ATLAS_PROPERTY(entryCount);

                    json_elem_def.AddMember("atlas", atlas, allocator);
#undef SAVE_ATLAS_PROPERTY
                }

                SAVE_PROPERTY(elemType);
                SAVE_PROPERTY(visualCount);

                SAVE_PROPERTY(velIntervalCount);
                SAVE_PROPERTY(visStateIntervalCount);

                if (elem_def->velSamples)
                {
                    rapidjson::Value vel_samples_json(rapidjson::kArrayType);

                    // note the <=
                    for (size_t j = 0; j <= elem_def->velIntervalCount; j++)
                    {
                        rapidjson::Value vel_sample(rapidjson::kObjectType);

                        rapidjson::Value local_sample(rapidjson::kObjectType);
                        local_sample.AddMember("velocity", utils::json::to_json(elem_def->velSamples[j].local.velocity, allocator), allocator);
                        local_sample.AddMember("totalDelta", utils::json::to_json(elem_def->velSamples[j].local.totalDelta, allocator), allocator);

                        rapidjson::Value world_sample(rapidjson::kObjectType);
                        world_sample.AddMember("velocity", utils::json::to_json(elem_def->velSamples[j].world.velocity, allocator), allocator);
                        world_sample.AddMember("totalDelta", utils::json::to_json(elem_def->velSamples[j].world.totalDelta, allocator), allocator);

                        vel_sample.AddMember("local", local_sample, allocator);
                        vel_sample.AddMember("world", world_sample, allocator);

                        vel_samples_json.PushBack(vel_sample, allocator);
                    }

                    json_elem_def.AddMember("velSamples", vel_samples_json, allocator);
                }

                if (elem_def->visSamples)
                {
                    rapidjson::Value vis_samples_json(rapidjson::kArrayType);

                    // note the <=
                    for (size_t j = 0; j <= elem_def->visStateIntervalCount; j++)
                    {
                        rapidjson::Value vis_sample(rapidjson::kObjectType);

                        vis_sample.AddMember("base", utils::json::to_json(elem_def->visSamples[j].base, allocator), allocator);
                        vis_sample.AddMember("amplitude", utils::json::to_json(elem_def->visSamples[j].amplitude, allocator), allocator);

                        vis_samples_json.PushBack(vis_sample, allocator);
                    }

                    json_elem_def.AddMember("visSamples", vis_samples_json, allocator);
                }

                // Save_FxElemDefVisuals
                {
                    if (elem_def->elemType == native::FX_ELEM_TYPE_DECAL)
                    {
                        if (elem_def->visuals.markArray)
                        {
                            rapidjson::Value mark_array_json(rapidjson::kArrayType);
                            for (char j = 0; j < elem_def->visualCount; ++j)
                            {
                                rapidjson::Value materials(rapidjson::kArrayType);
                                for (auto k = 0; k < 2; k++)
                                {
                                    if (elem_def->visuals.markArray[j].materials[k])
                                    {
                                        materials.PushBack(RAPIDJSON_STR(elem_def->visuals.markArray[j].materials[k]->info.name), allocator);
                                        assets->write(native::ASSET_TYPE_MATERIAL, elem_def->visuals.markArray[j].materials[k]);
                                    }
                                    else
                                    {
                                        materials.PushBack(rapidjson::Value(rapidjson::kNullType), allocator);
                                    }
                                }

                                mark_array_json.PushBack(materials, allocator);
                            }

                            json_elem_def.AddMember("markArray", mark_array_json, allocator);
                        }
                    }
                    else if (elem_def->visualCount > 1)
                    {
                        if (elem_def->visuals.array)
                        {
                            rapidjson::Value visuals_array_json(rapidjson::kArrayType);

                            for (char j = 0; j < elem_def->visualCount; ++j)
                            {
                                visuals_array_json.PushBack(to_json(&elem_def->visuals.array[j], elem_def->elemType, allocator), allocator);
                            }

                            json_elem_def.AddMember("visualsArray", visuals_array_json, allocator);
                        }
                    }
                    else if (elem_def->visualCount)
					{
                        json_elem_def.AddMember("instance", to_json(&elem_def->visuals.instance, elem_def->elemType, allocator), allocator);
                    }
                }

                json_elem_def.AddMember("collBounds", utils::json::to_json(elem_def->collBounds, allocator), allocator);

                if (elem_def->effectOnImpact.handle)
                {
                    json_elem_def.AddMember("effectOnImpact", RAPIDJSON_STR(elem_def->effectOnImpact.handle->name), allocator);
                    assets->write(native::ASSET_TYPE_FX, elem_def->effectOnImpact.handle);
                }

                if (elem_def->effectOnDeath.handle)
                {
                    json_elem_def.AddMember("effectOnDeath", RAPIDJSON_STR(elem_def->effectOnDeath.handle->name), allocator);
                    assets->write(native::ASSET_TYPE_FX, elem_def->effectOnDeath.handle);
                }

                if (elem_def->effectEmitted.handle)
                {
                    json_elem_def.AddMember("effectEmitted", RAPIDJSON_STR(elem_def->effectEmitted.handle->name), allocator);
                    assets->write(native::ASSET_TYPE_FX, elem_def->effectEmitted.handle);
                }

                SAVE_RANGE(emitDist);
                SAVE_RANGE(emitDistVariance);

                // Save_FxElemExtendedDefPtr
                {
                    if (elem_def->elemType == native::FX_ELEM_TYPE_TRAIL)
                    {
                        // Save_FxTrailDef
                        {
                            if (elem_def->extended.trailDef)
                            {
                                AssertSize(native::FxTrailDef, 36);

                                native::FxTrailDef* trail_def = elem_def->extended.trailDef;
                                rapidjson::Value json_trail_def(rapidjson::kObjectType);

#define SAVE_TRAILDEF_MEMBER(x) json_trail_def.AddMember(#x, trail_def->x, allocator)

                                SAVE_TRAILDEF_MEMBER(scrollTimeMsec);
                                SAVE_TRAILDEF_MEMBER(repeatDist);
                                SAVE_TRAILDEF_MEMBER(invSplitDist);
                                SAVE_TRAILDEF_MEMBER(invSplitArcDist);
                                SAVE_TRAILDEF_MEMBER(invSplitTime);

                                rapidjson::Value verts_json(rapidjson::kArrayType);
                                for (auto j = 0; j < trail_def->vertCount; j++)
                                {
                                    rapidjson::Value trail_vertex(rapidjson::kObjectType);

                                    trail_vertex.AddMember("pos", utils::json::make_json_array(trail_def->verts[j].pos, 2, allocator), allocator);
                                    trail_vertex.AddMember(
                                        "normal", utils::json::make_json_array(trail_def->verts[j].normal, 2, allocator), allocator);

                                    trail_vertex.AddMember("texCoord", trail_def->verts[j].texCoord, allocator);

                                    verts_json.PushBack(trail_vertex, allocator);
                                }

                                json_trail_def.AddMember("verts", verts_json, allocator);

                                json_trail_def.AddMember(
                                    "inds", utils::json::make_json_array(trail_def->inds, trail_def->indCount, allocator), allocator);

                                json_elem_def.AddMember("trailDef", json_trail_def, allocator);
#undef SAVE_TRAILDEF_MEMBER
                            }
                        }
                    }
                    else if (elem_def->elemType == native::FX_ELEM_TYPE_SPARK_FOUNTAIN)
                    {
                        if (elem_def->extended.sparkFountainDef)
                        {
                            const auto spark_fountain = elem_def->extended.sparkFountainDef;
                            rapidjson::Value json_spark_fountain(rapidjson::kObjectType);

#define SAVE_SPARK_FOUNTAIN_MEMBER(x) json_spark_fountain.AddMember(#x, spark_fountain->x, allocator)

                            SAVE_SPARK_FOUNTAIN_MEMBER(gravity);
                            SAVE_SPARK_FOUNTAIN_MEMBER(bounceFrac);
                            SAVE_SPARK_FOUNTAIN_MEMBER(bounceRand);
                            SAVE_SPARK_FOUNTAIN_MEMBER(sparkSpacing);
                            SAVE_SPARK_FOUNTAIN_MEMBER(sparkLength);
                            SAVE_SPARK_FOUNTAIN_MEMBER(sparkCount);
                            SAVE_SPARK_FOUNTAIN_MEMBER(loopTime);
                            SAVE_SPARK_FOUNTAIN_MEMBER(velMin);
                            SAVE_SPARK_FOUNTAIN_MEMBER(velMax);
                            SAVE_SPARK_FOUNTAIN_MEMBER(velConeFrac);
                            SAVE_SPARK_FOUNTAIN_MEMBER(restSpeed);
                            SAVE_SPARK_FOUNTAIN_MEMBER(boostTime);
                            SAVE_SPARK_FOUNTAIN_MEMBER(boostFactor);

#undef SAVE_SPARK_FOUNTAIN_MEMBER

                            json_elem_def.AddMember("sparkFountain", json_spark_fountain, allocator);
                        }
                    }
                }

                SAVE_PROPERTY(sortOrder);
                SAVE_PROPERTY(lightingFrac);
                SAVE_PROPERTY(useItemClip);
                SAVE_PROPERTY(fadeInfo);

#undef SAVE_RANGE_MULTIPLE
#undef SAVE_PROPERTY
#undef SAVE_RANGE

                elemTable.PushBack(json_elem_def, allocator);
            }
        }

        output.AddMember("elemDefs", elemTable, allocator);

        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
        output.Accept(writer);

        return utils::io::write_file(get_work_path(header).string(), buff.GetString());
    }

    std::filesystem::path ifx::get_legacy_work_path(const std::string& file_name) const
    {
        return get_work_path(get_binary_file_name(file_name));
    }

    void* ifx::read_internal(const std::string& name) const
    {
        const auto& path = get_work_path(name).string();

        if (utils::io::file_exists(path))
        {
            return read_plaintext(name, path);
        }
        else
        {
            const auto old_path = get_legacy_work_path(path).string();

            if (utils::io::file_exists(old_path))
            {
                return read_binary(name, old_path);
            }
        }

        return nullptr;
    }

    native::FxEffectDef* ifx::read_binary(const std::string& name, const std::string& path) const
    {
        auto contents = utils::io::read_file(path);
        utils::stream::reader buffer(&local_allocator, contents);

        int64_t magic = buffer.read<int64_t>();
        if (std::memcmp(&magic, "IW4xFx  ", 8))
        {
            print_error("Reading fx '{}' failed, header is invalid!", name);
            return nullptr;
        }

        int32_t version = buffer.read<int32_t>();
        if (version > IW4X_FX_VERSION)
        {
            print_error("Reading fx '{}' failed, expected version is {}, but it was {}!", name, IW4X_FX_VERSION, version);
            return nullptr;
        }

        native::FxEffectDef* asset = buffer.read_object<native::FxEffectDef>();

        if (asset->name)
        {
            asset->name = buffer.read_cstring();
        }

        if (asset->elemDefs)
        {
            asset->elemDefs =
                buffer.read_array<native::FxElemDef>(asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot);

            for (int i = 0; i < (asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot); ++i)
            {
                native::FxElemDef* elemDef = &asset->elemDefs[i];

                if (elemDef->velSamples)
                {
                    elemDef->velSamples = buffer.read_array<native::FxElemVelStateSample>(elemDef->velIntervalCount + 1);
                }

                if (elemDef->visSamples)
                {
                    elemDef->visSamples = buffer.read_array<native::FxElemVisStateSample>(elemDef->visStateIntervalCount + 1);
                }

                // Save_FxElemDefVisuals
                {
                    if (elemDef->elemType == native::FX_ELEM_TYPE_DECAL)
                    {
                        if (elemDef->visuals.markArray)
                        {
                            elemDef->visuals.markArray = buffer.read_array<native::FxElemMarkVisuals>(elemDef->visualCount);

                            for (char j = 0; j < elemDef->visualCount; ++j)
                            {
                                if (elemDef->visuals.markArray[j].materials[0])
                                {
                                    elemDef->visuals.markArray[j].materials[0] =
                                        find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, buffer.read_string().data());
                                }

                                if (elemDef->visuals.markArray[j].materials[1])
                                {
                                    elemDef->visuals.markArray[j].materials[1] =
                                        find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, buffer.read_string().data());
                                }
                            }
                        }
                    }
                    else if (elemDef->visualCount > 1)
                    {
                        if (elemDef->visuals.array)
                        {
                            elemDef->visuals.array = buffer.read_array<native::FxElemVisuals>(elemDef->visualCount);

                            for (char j = 0; j < elemDef->visualCount; ++j)
                            {
                                read(&elemDef->visuals.array[j], elemDef->elemType, &buffer);
                            }
                        }
                    }
                    else if (elemDef->visualCount == 1)
                    {
                        read(&elemDef->visuals.instance, elemDef->elemType, &buffer);
                    }
                }

                if (elemDef->effectOnImpact.handle)
                {
                    auto fx_name = buffer.read_string();
                    elemDef->effectOnImpact.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, fx_name.data());
                    RETURN_IF_NULL(elemDef->effectOnImpact.handle);
                }

                if (elemDef->effectOnDeath.handle)
                {
                    auto fx_name = buffer.read_string();
                    elemDef->effectOnDeath.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, fx_name.data());
                    RETURN_IF_NULL(elemDef->effectOnDeath.handle);
                }

                if (elemDef->effectEmitted.handle)
                {
                    auto fx_name = buffer.read_string();
                    elemDef->effectEmitted.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, fx_name.data());
                    RETURN_IF_NULL(elemDef->effectEmitted.handle);
                }

                // Save_FxElemExtendedDefPtr
                {
                    if (elemDef->elemType == native::FX_ELEM_TYPE_TRAIL)
                    {
                        // Save_FxTrailDef
                        {
                            if (elemDef->extended.trailDef)
                            {
                                native::FxTrailDef* trailDef = buffer.read_object<native::FxTrailDef>();
                                elemDef->extended.trailDef = trailDef;

                                if (trailDef->verts)
                                {
                                    trailDef->verts = buffer.read_array<native::FxTrailVertex>(trailDef->vertCount);
                                }

                                if (trailDef->inds)
                                {
                                    trailDef->inds = buffer.read_array<uint16_t>(trailDef->indCount);
                                }
                            }
                        }
                    }
                    else if (version >= 2)
                    {
                        if (elemDef->elemType == native::FX_ELEM_TYPE_SPARK_FOUNTAIN)
                        {
                            if (elemDef->extended.sparkFountainDef)
                            {
                                elemDef->extended.sparkFountainDef = buffer.read_object<native::FxSparkFountainDef>();
                            }
                        }
                    }
                }
            }
        }

        return asset;
    }

    native::FxEffectDef* ifx::read_plaintext(const std::string& name, const std::string& path) const
    {
        if (!utils::io::file_exists(path)) return nullptr;

        auto contents = utils::io::read_file(path);
        rapidjson::Document fx_json;
        try
        {
            fx_json.Parse(contents.data());
        }
        catch (const std::exception& e)
        {
            print_error("Invalid JSON for FX {}! {}", name, e.what());
            return nullptr;
        }

        auto asset = local_allocator.allocate<native::FxEffectDef>();

        try
        {
            const auto version = fx_json["version"].Get<int32_t>();
            if (version > IW4X_FX_VERSION)
            {
                print_error("Wrong FX version for {}! expected {} and got {}!", name, IW4X_FX_VERSION, version);
                assert(false);
                return nullptr;
            }

            asset->name = local_allocator.duplicate_string(fx_json["name"].Get<std::string>());
            asset->flags = fx_json["flags"].Get<int32_t>();
            asset->totalSize = fx_json["totalSize"].Get<int32_t>();
            asset->msecLoopingLife = fx_json["msecLoopingLife"].Get<int32_t>();
            asset->elemDefCountLooping = fx_json["elemDefCountLooping"].Get<int32_t>();
            asset->elemDefCountOneShot = fx_json["elemDefCountOneShot"].Get<int32_t>();
            asset->elemDefCountEmission = fx_json["elemDefCountEmission"].Get<int32_t>();

            if (fx_json.HasMember("elemDefs") && !fx_json["elemDefs"].IsNull())
            {
                const auto json_elem_defs = fx_json["elemDefs"].GetArray();

                const auto json_elem_count = json_elem_defs.Size();
                assert(static_cast<int32_t>(json_elem_count) == asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot);

                asset->elemDefs = local_allocator.allocate_array<iw4of::native::FxElemDef>(json_elem_count);

                {
                    size_t i = 0;
                    for (const auto& elem : json_elem_defs)
                    {
                        const auto elem_def = &asset->elemDefs[i++];
                        elem_def->flags = elem["flags"].Get<int32_t>();
                        elem_def->spawn.looping.count = elem["spawn"]["A"].Get<int32_t>();
                        elem_def->spawn.looping.intervalMsec = elem["spawn"]["B"].Get<int32_t>();

                        elem_def->spawnRange = utils::json::float_range_from_json(elem["spawnRange"]);
                        elem_def->fadeInRange = utils::json::float_range_from_json(elem["fadeInRange"]);
                        elem_def->fadeOutRange = utils::json::float_range_from_json(elem["fadeOutRange"]);
                        elem_def->spawnFrustumCullRadius = elem["spawnFrustumCullRadius"].GetFloat();
                        elem_def->spawnDelayMsec = utils::json::int_range_from_json(elem["spawnDelayMsec"]);
                        elem_def->lifeSpanMsec = utils::json::int_range_from_json(elem["lifeSpanMsec"]);

                        for (size_t j = 0; j < ARRAYSIZE(elem_def->spawnOrigin); j++)
                        {
                            elem_def->spawnOrigin[j] = utils::json::float_range_from_json(elem["spawnOrigin"][j]);
                        }

                        elem_def->spawnOffsetRadius = utils::json::float_range_from_json(elem["spawnOffsetRadius"]);
                        elem_def->spawnOffsetHeight = utils::json::float_range_from_json(elem["spawnOffsetHeight"]);

                        for (size_t j = 0; j < ARRAYSIZE(elem_def->spawnAngles); j++)
                        {
                            elem_def->spawnAngles[j] = utils::json::float_range_from_json(elem["spawnAngles"][j]);
                        }

                        for (size_t j = 0; j < ARRAYSIZE(elem_def->angularVelocity); j++)
                        {
                            elem_def->angularVelocity[j] = utils::json::float_range_from_json(elem["angularVelocity"][j]);
                        }

                        elem_def->initialRotation = utils::json::float_range_from_json(elem["initialRotation"]);
                        elem_def->gravity = utils::json::float_range_from_json(elem["gravity"]);
                        elem_def->reflectionFactor = utils::json::float_range_from_json(elem["reflectionFactor"]);

                        elem_def->atlas.behavior = elem["atlas"]["behavior"].Get<uint8_t>();
                        elem_def->atlas.index = elem["atlas"]["index"].Get<uint8_t>();
                        elem_def->atlas.fps = elem["atlas"]["fps"].Get<uint8_t>();
                        elem_def->atlas.loopCount = elem["atlas"]["loopCount"].Get<uint8_t>();
                        elem_def->atlas.colIndexBits = elem["atlas"]["colIndexBits"].Get<uint8_t>();
                        elem_def->atlas.rowIndexBits = elem["atlas"]["rowIndexBits"].Get<uint8_t>();
                        elem_def->atlas.entryCount = elem["atlas"]["entryCount"].Get<int16_t>();

                        elem_def->elemType = elem["elemType"].Get<uint8_t>();
                        elem_def->visualCount = elem["visualCount"].Get<uint8_t>();
                        elem_def->velIntervalCount = elem["velIntervalCount"].Get<uint8_t>();
                        elem_def->visStateIntervalCount = elem["visStateIntervalCount"].Get<uint8_t>();

                        elem_def->velSamples = local_allocator.allocate_array<native::FxElemVelStateSample>(elem_def->velIntervalCount+1);
                        if (elem.HasMember("velSamples") && !elem["velSamples"].IsNull())
                        {
							// Mind the <=
                            for (size_t j = 0; j <= elem_def->velIntervalCount; j++)
                            {
                                const auto& json_vel_sample = elem["velSamples"][j];
                                auto vel_sample = &elem_def->velSamples[j];

                                vel_sample->local.velocity = utils::json::vec3_range_from_json(json_vel_sample["local"]["velocity"]);
                                vel_sample->local.totalDelta = utils::json::vec3_range_from_json(json_vel_sample["local"]["totalDelta"]);
                                vel_sample->world.velocity = utils::json::vec3_range_from_json(json_vel_sample["world"]["velocity"]);
                                vel_sample->world.totalDelta = utils::json::vec3_range_from_json(json_vel_sample["world"]["totalDelta"]);
                            }
                        }

                        elem_def->visSamples = local_allocator.allocate_array<native::FxElemVisStateSample>(elem_def->visStateIntervalCount+1);
                        if (elem.HasMember("visSamples") && !elem["visSamples"].IsNull())
                        {
							// Mind the <=
                            for (size_t j = 0; j <= elem_def->visStateIntervalCount; j++)
                            {
                                const auto& json_vel_sample = elem["visSamples"][j];
                                auto vis_sample = &elem_def->visSamples[j];

                                vis_sample->base = utils::json::visual_state_from_json(json_vel_sample["base"]);
                                vis_sample->amplitude = utils::json::visual_state_from_json(json_vel_sample["amplitude"]);
                            }
                        }

                        elem_def->collBounds = utils::json::read_bounds(elem["collBounds"]);

                        if (elem.HasMember("effectOnImpact") && !elem["effectOnImpact"].IsNull())
                        {
                            elem_def->effectOnImpact.handle = find<native::FxEffectDef>(native::ASSET_TYPE_FX, elem["effectOnImpact"].GetString());
                        }

                        if (elem.HasMember("effectOnDeath") && !elem["effectOnDeath"].IsNull())
                        {
                            elem_def->effectOnDeath.handle = find<native::FxEffectDef>(native::ASSET_TYPE_FX, elem["effectOnDeath"].GetString());
                        }

                        if (elem.HasMember("effectEmitted") && !elem["effectEmitted"].IsNull())
                        {
                            elem_def->effectEmitted.handle = find<native::FxEffectDef>(native::ASSET_TYPE_FX, elem["effectEmitted"].GetString());
                        }

                        // Read_FXElemDefVisuals
                        if (elem_def->elemType == native::FX_ELEM_TYPE_DECAL)
                        {
                            if (elem.HasMember("markArray") && !elem["markArray"].IsNull())
                            {
                                elem_def->visuals.markArray = local_allocator.allocate_array<native::FxElemMarkVisuals>(elem_def->visualCount);

                                for (size_t j = 0; j < elem_def->visualCount; j++)
                                {
                                    for (size_t k = 0; k < 2; k++)
                                    {
                                        const auto json_material = &elem["markArray"][j][k];

                                        if (json_material->IsNull())
                                        {
                                            // Nothing to do
                                        }
                                        else
                                        {
                                            elem_def->visuals.markArray[j].materials[k] =
                                                find<native::Material>(native::ASSET_TYPE_MATERIAL, json_material->GetString());
                                            assert(elem_def->visuals.markArray[j].materials[k]);
                                        }
                                    }
                                }
                            }
                        }
                        else if (elem_def->visualCount > 1)
                        {
                            if (elem.HasMember("visualsArray") && !elem["visualsArray"].IsNull())
                            {
                                elem_def->visuals.array = local_allocator.allocate_array<native::FxElemVisuals>(elem_def->visualCount);

                                for (char j = 0; j < elem_def->visualCount; ++j)
                                {
									const auto& json_array = elem["visualsArray"];
                                    read(&elem_def->visuals.array[j], elem_def->elemType, json_array[j]);
                                }
                            }
                        }
                        else if (elem_def->visualCount)
                        {
                            read(&elem_def->visuals.instance, elem_def->elemType, elem["instance"]);
                        }

                        elem_def->emitDist = utils::json::float_range_from_json(elem["emitDist"]);
                        elem_def->emitDistVariance = utils::json::float_range_from_json(elem["emitDistVariance"]);
                        elem_def->emitDistVariance = utils::json::float_range_from_json(elem["emitDistVariance"]);
                        // Save_FxElemExtendedDefPtr
                        {
                            if (elem_def->elemType == native::FX_ELEM_TYPE_TRAIL)
                            {
                                // Save_FxTrailDef
                                {
                                    if (elem.HasMember("trailDef") && !elem["trailDef"].IsNull())
                                    {
                                        elem_def->extended.trailDef = local_allocator.allocate<native::FxTrailDef>();

                                        native::FxTrailDef* trail_def = elem_def->extended.trailDef;
                                        const rapidjson::Value& json_trail_def = elem["trailDef"];

#define READ_TRAILDEF_MEMBER(x, y) trail_def->x = json_trail_def[#x].Get<y>()

                                        READ_TRAILDEF_MEMBER(scrollTimeMsec, int32_t);
                                        READ_TRAILDEF_MEMBER(repeatDist, int32_t);
                                        READ_TRAILDEF_MEMBER(invSplitDist, float);
                                        READ_TRAILDEF_MEMBER(invSplitArcDist, float);
                                        READ_TRAILDEF_MEMBER(invSplitTime, float);

                                        const rapidjson::Value& verts_json = json_trail_def["verts"];
                                        trail_def->vertCount = verts_json.Size();
                                        trail_def->verts = local_allocator.allocate_array<native::FxTrailVertex>(trail_def->vertCount);

                                        for (auto j = 0; j < trail_def->vertCount; j++)
                                        {
                                            auto trail_vertex = &trail_def->verts[j];
                                            utils::json::copy_array(trail_vertex->pos, verts_json[j]["pos"], 2);
                                            utils::json::copy_array(trail_vertex->normal, verts_json[j]["normal"], 2);

                                            trail_vertex->texCoord = verts_json[j]["texCoord"].GetFloat();
                                        }

                                        trail_def->indCount = json_trail_def["inds"].Size();
                                        trail_def->inds = local_allocator.allocate_array<uint16_t>(trail_def->indCount);
                                        utils::json::copy_array(trail_def->inds, json_trail_def["inds"], trail_def->indCount);
#undef READ_TRAILDEF_MEMBER
                                    }
                                }
                            }
                            else if (elem_def->elemType == native::FX_ELEM_TYPE_SPARK_FOUNTAIN)
                            {
                                if (elem.HasMember("sparkFountain") && !elem["sparkFountain"].IsNull())
                                {
                                    elem_def->extended.sparkFountainDef = local_allocator.allocate<iw4of::native::FxSparkFountainDef>();
                                    auto spark_fountain = elem_def->extended.sparkFountainDef;
                                    const rapidjson::Value& json_spark_fountain = elem["sparkFountain"];

#define READ_SPARK_FOUNTAIN_MEMBER(x, y) spark_fountain->x = json_spark_fountain[#x].Get<y>()

                                    READ_SPARK_FOUNTAIN_MEMBER(gravity, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(bounceFrac, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(bounceRand, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(sparkSpacing, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(sparkLength, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(sparkCount, int32_t);
                                    READ_SPARK_FOUNTAIN_MEMBER(loopTime, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(velMin, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(velMax, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(velConeFrac, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(restSpeed, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(boostTime, float);
                                    READ_SPARK_FOUNTAIN_MEMBER(boostFactor, float);

#undef READ_SPARK_FOUNTAIN_MEMBER
                                }
								else
								{
									// Guaranteed to crash
									print_error("Missing spark fountain definition for fx {} elem {}, this will crash on iw4!", name, i);
								}
                            }
                        }

                        elem_def->sortOrder = elem["sortOrder"].Get<uint8_t>();
                        elem_def->lightingFrac = elem["lightingFrac"].Get<uint8_t>();
                        elem_def->useItemClip = elem["useItemClip"].Get<uint8_t>();
                        elem_def->fadeInfo = elem["fadeInfo"].Get<uint8_t>();
                    }
                }
            }
        }
        catch (const std::exception& e)
        {
            print_error("Malformed JSON for clipmap {}! {}", name, e.what());
            return nullptr;
        }

        return asset;
    }

    std::filesystem::path iw4of::interfaces::ifx::get_binary_file_name(const std::string& basename) const
    {
        return std::format("{}.iw4xFx", basename);
    }

    std::filesystem::path iw4of::interfaces::ifx::get_file_name(const std::string& basename) const
    {
        return std::format("{}.iw4x.json", basename);
    }

    rapidjson::Value ifx::to_json(const native::FxElemVisuals* visuals, char elemType,
                                  rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const
    {
        rapidjson::Value output = rapidjson::Value(rapidjson::kNullType);
        switch (elemType)
        {
            case native::FX_ELEM_TYPE_MODEL:
                if (visuals->model)
                {
                    output = rapidjson::Value(rapidjson::kObjectType);
                    output.AddMember("model", RAPIDJSON_STR(visuals->model->name), allocator);
                    assets->write(native::ASSET_TYPE_XMODEL, visuals->model);
                }

                break;

            case native::FX_ELEM_TYPE_OMNI_LIGHT:
            case native::FX_ELEM_TYPE_SPOT_LIGHT: break;

            case native::FX_ELEM_TYPE_SOUND:
                if (visuals->soundName)
                {
                    output = rapidjson::Value(rapidjson::kObjectType);
                    output.AddMember("sound", RAPIDJSON_STR(visuals->soundName), allocator);

                    auto list = find<native::snd_alias_list_t>(native::ASSET_TYPE_SOUND, visuals->soundName);

                    if (list)
                    {
                        assets->write(native::ASSET_TYPE_SOUND, list);
                    }
                }

                break;

            case native::FX_ELEM_TYPE_RUNNER:
                if (visuals->effectDef.handle)
                {
                    output = rapidjson::Value(rapidjson::kObjectType);
                    output.AddMember("runner", RAPIDJSON_STR(visuals->effectDef.handle->name), allocator);
                    assets->write(native::ASSET_TYPE_FX, visuals->effectDef.handle);
                }

                break;

            default:
                if (visuals->material)
                {
                    output = rapidjson::Value(rapidjson::kObjectType);
                    output.AddMember("material", RAPIDJSON_STR(visuals->material->info.name), allocator);
                    assets->write(native::ASSET_TYPE_MATERIAL, visuals->material);
                }

                break;
        }

        return output;
    }


    void interfaces::ifx::read(native::FxElemVisuals* visuals, char elemType, utils::stream::reader* reader) const
    {
        switch (elemType)
        {
            case native::FX_ELEM_TYPE_MODEL:
            {
                if (visuals->model)
                {
                    visuals->model = find<native::XModel>(native::XAssetType::ASSET_TYPE_XMODEL, reader->read_string().data());
                }

                break;
            }

            case native::FX_ELEM_TYPE_OMNI_LIGHT:
            case native::FX_ELEM_TYPE_SPOT_LIGHT: break;

            case native::FX_ELEM_TYPE_SOUND:
            {
                if (visuals->soundName)
                {
                    visuals->soundName = reader->read_cstring();
                    find<native::snd_alias_list_t>(native::XAssetType::ASSET_TYPE_SOUND, visuals->soundName);
                }
                break;
            }

            case native::FX_ELEM_TYPE_RUNNER:
            {
                if (visuals->effectDef.handle)
                {
                    visuals->effectDef.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, reader->read_string().data());
                    assert(visuals->effectDef.handle);
                }

                break;
            }

            default:
            {
                if (visuals->material)
                {
                    visuals->material = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, reader->read_string().data());
                    assert(visuals->material);
                }

                break;
            }
        }
    }

    void interfaces::ifx::read(native::FxElemVisuals* visuals, char elemType, const rapidjson::Value& value) const
    {
        if (value.IsNull())
        {
            return;
        }

        switch (elemType)
        {
            case native::FX_ELEM_TYPE_MODEL:
            {
                if (value.HasMember("model"))
                {
                    visuals->model = find<native::XModel>(native::XAssetType::ASSET_TYPE_XMODEL, value["model"].GetString());
                }

                break;
            }

            case native::FX_ELEM_TYPE_OMNI_LIGHT:
            case native::FX_ELEM_TYPE_SPOT_LIGHT: break;

            case native::FX_ELEM_TYPE_SOUND:
            {
                if (value.HasMember("sound"))
                {
                    visuals->soundName = local_allocator.duplicate_string(value["sound"].GetString());
					const auto sound = find<native::snd_alias_list_t>(native::XAssetType::ASSET_TYPE_SOUND,  visuals->soundName);

					if (!sound)
					{
						print_error("Critical error - could not find sound {}. This will crash the game!", visuals->soundName);
					}
				}
                break;
            }

            case native::FX_ELEM_TYPE_RUNNER:
            {
                if (value.HasMember("runner"))
                {
                    visuals->effectDef.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, value["runner"].GetString());
					if (!visuals->effectDef.handle)
					{
						print_error("Critical error - could not find fx {}. This will crash the game!", value["runner"].GetString());
					}
                }

                break;
            }

            default:
            {
                if (value.HasMember("material"))
                {
                    visuals->material = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, value["material"].GetString());
					if (!visuals->material)
					{
						print_error("Critical error - could not find fx {}. This will crash the game!", value["material"].GetString());
					}
                }

                break;
            }
        }
    }

} // namespace iw4of::interfaces
