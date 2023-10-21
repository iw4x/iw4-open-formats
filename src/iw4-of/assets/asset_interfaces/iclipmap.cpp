#include <std_include.hpp>
#include "iclipmap.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <utils/io.hpp>
#include <utils/json.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

namespace iw4of::interfaces
{
#define IW4X_CLIPMAP_VERSION 3

    static_assert(sizeof native::ClipMaterial == 12);
    static_assert(sizeof native::cbrushside_t == 8);
    static_assert(sizeof native::cNode_t == 8);
    static_assert(sizeof native::cLeaf_t == 40);
    static_assert(sizeof native::cLeafBrushNode_s == 20);
    static_assert(sizeof native::CollisionBorder == 28);
    static_assert(sizeof native::CollisionPartition == 12);
    static_assert(sizeof native::CollisionAabbTree == 32);
    static_assert(sizeof native::cmodel_t == 68);
    static_assert(sizeof native::SModelAabbNode == 28);

    std::filesystem::path iclipmap::get_file_name(const std::string& name) const
    {
        constexpr auto prefix = "maps/mp/";
        constexpr auto suffix = ".d3dbsp";

        std::string basename = name;
        utils::string::replace(basename, prefix, "");
        utils::string::replace(basename, suffix, "");

        return std::format("{}.iw4x.json", basename);
    }

    void* iclipmap::read_internal(const std::string& _name) const
    {
        auto path = iclipmap::get_work_path(_name);

        if (!utils::io::file_exists(path.string())) return nullptr;

        auto contents = utils::io::read_file(path.string());
        rapidjson::Document clipmap_json;
        try
        {
            clipmap_json.Parse<rapidjson::kParseNanAndInfFlag>(contents.data());
        }
        catch (const std::exception& e)
        {
            print_error("Invalid JSON for clipmap {}! {}", _name, e.what());
            return nullptr;
        }

        auto clipmap = local_allocator.allocate<native::clipMap_t>();

        try
        {
            assert(clipmap_json["version"].Get<int32_t>() <= IW4X_CLIPMAP_VERSION);

            clipmap->name = local_allocator.duplicate_string(clipmap_json["name"].Get<std::string>());
            clipmap->isInUse = clipmap_json["isInUse"].Get<int32_t>();

            // planes
            clipmap->planeCount = clipmap_json["planes"].Size();
            clipmap->planes = clipmap->planeCount == 0 ? nullptr : local_allocator.allocate_array<native::cplane_s>(clipmap->planeCount);

            for (size_t i = 0; i < clipmap->planeCount; i++)
            {
                auto plane = &clipmap->planes[i];
                auto& json_plane = clipmap_json["planes"][i];
                utils::json::copy_array(plane->normal, clipmap_json["planes"][i]["normal"], 3);
                plane->dist = json_plane["dist"].Get<float>();
                plane->type = json_plane["type"].Get<uint8_t>();
            }

            // Smodel list
            clipmap->numStaticModels = clipmap_json["staticModelList"].Size();
            clipmap->staticModelList =
                clipmap->numStaticModels == 0 ? nullptr : local_allocator.allocate_array<native::cStaticModel_s>(clipmap->numStaticModels);

            for (size_t i = 0; i < clipmap->numStaticModels; i++)
            {
                auto model = &clipmap->staticModelList[i];
                auto& json_model = clipmap_json["staticModelList"][i];

                const std::string model_name = json_model["xmodel"].GetString();
                model->xmodel = find<native::XModel>(native::XAssetType::ASSET_TYPE_XMODEL, model_name);
                RETURN_IF_NULL(model->xmodel);

                utils::json::copy_array(model->origin, json_model["origin"], 3);
                for (size_t j = 0; j < 3; j++)
                {
                    utils::json::copy_array(model->invScaledAxis[j], json_model["invScaledAxis"][j], 3);
                }

                model->absBounds = utils::json::read_bounds(json_model["absBounds"]);
            }

            clipmap->numMaterials = clipmap_json["materials"].Size();
            clipmap->materials = clipmap->numMaterials == 0 ? nullptr : local_allocator.allocate_array<native::ClipMaterial>(clipmap->numMaterials);

            for (size_t i = 0; i < clipmap->numMaterials; i++)
            {
                auto material = &clipmap->materials[i];
                auto& json_material = clipmap_json["materials"][i];

                material->name = local_allocator.duplicate_string(json_material["name"].Get<std::string>());
                material->surfaceFlags = json_material["surfaceFlags"].Get<int32_t>();
                material->contents = json_material["contents"].Get<int32_t>();
            }

            clipmap->numBrushSides = clipmap_json["brushsides"].Size();
            clipmap->brushsides =
                clipmap->numBrushSides == 0 ? nullptr : local_allocator.allocate_array<native::cbrushside_t>(clipmap->numBrushSides);

            for (size_t i = 0; i < clipmap->numBrushSides; i++)
            {
                auto* brushside = &clipmap->brushsides[i];
                auto& json_brushside = clipmap_json["brushsides"][i];

                assert(clipmap->planes);
                brushside->plane = &clipmap->planes[std::stoi(json_brushside["plane"].Get<std::string>().substr(1))];
                brushside->materialNum = json_brushside["materialNum"].Get<uint16_t>();
                brushside->firstAdjacentSideOffset = json_brushside["firstAdjacentSideOffset"].Get<char>();
                brushside->edgeCount = json_brushside["edgeCount"].Get<char>();
            }

            clipmap->numBrushEdges = clipmap_json["brushEdges"].Size();
            clipmap->brushEdges = clipmap->numBrushEdges == 0 ? nullptr : local_allocator.allocate_array<uint8_t>(clipmap->numBrushEdges);
            utils::json::copy_array(clipmap->brushEdges, clipmap_json["brushEdges"]);

            clipmap->numNodes = clipmap_json["nodes"].Size();
            clipmap->nodes = clipmap->numNodes == 0 ? nullptr : local_allocator.allocate_array<native::cNode_t>(clipmap->numNodes);

            for (size_t i = 0; i < clipmap->numNodes; i++)
            {
                auto node = &clipmap->nodes[i];
                auto& json_node = clipmap_json["nodes"][i];

                assert(clipmap->planes);
                node->plane = &clipmap->planes[std::stoi(json_node["plane"].Get<std::string>().substr(1))];
                node->children[0] = json_node["children"][0].Get<int16_t>();
                node->children[1] = json_node["children"][1].Get<int16_t>();
            }

            // LEaves
            clipmap->numLeafs = clipmap_json["leafs"].Size();
            clipmap->leafs = clipmap->numLeafs == 0 ? nullptr : local_allocator.allocate_array<native::cLeaf_t>(clipmap->numLeafs);

            for (size_t i = 0; i < clipmap->numLeafs; i++)
            {
                auto leaf = &clipmap->leafs[i];
                auto& json_leaf = clipmap_json["leafs"][i];

                leaf->bounds = utils::json::read_bounds(json_leaf["bounds"]);
                leaf->firstCollAabbIndex = json_leaf["firstCollAabbIndex"].Get<uint16_t>();
                leaf->collAabbCount = json_leaf["collAabbCount"].Get<uint16_t>();
                leaf->brushContents = json_leaf["brushContents"].Get<int32_t>();
                leaf->terrainContents = json_leaf["terrainContents"].Get<int32_t>();
                leaf->leafBrushNode = json_leaf["leafBrushNode"].Get<int32_t>();
            }

            // Leafbrushnodes
            clipmap->leafbrushNodesCount = clipmap_json["leafbrushNodes"].Size();
            clipmap->leafbrushNodes =
                clipmap->leafbrushNodesCount == 0 ? nullptr : local_allocator.allocate_array<native::cLeafBrushNode_s>(clipmap->leafbrushNodesCount);

            // Leafbrushes
            clipmap->numLeafBrushes = clipmap_json["leafbrushes"].Size();
            clipmap->leafbrushes = clipmap->numLeafBrushes == 0 ? nullptr : local_allocator.allocate_array<uint16_t>(clipmap->numLeafBrushes);
            utils::json::copy_array(clipmap->leafbrushes, clipmap_json["leafbrushes"]);

            clipmap->numLeafSurfaces = clipmap_json["leafsurfaces"].Size();
            clipmap->leafsurfaces = clipmap->numLeafSurfaces == 0 ? nullptr : local_allocator.allocate_array<uint32_t>(clipmap->numLeafSurfaces);
            utils::json::copy_array(clipmap->leafsurfaces, clipmap_json["leafsurfaces"]);

            clipmap->vertCount = clipmap_json["verts"].Size();
            clipmap->verts = clipmap->vertCount == 0 ? nullptr : local_allocator.allocate_array<native::vec3_t>(clipmap->vertCount);
            for (size_t i = 0; i < clipmap->vertCount; i++)
            {
                utils::json::copy_array(clipmap->verts[i], clipmap_json["verts"][i]);
            }

            for (size_t i = 0; i < clipmap->leafbrushNodesCount; i++)
            {
                auto* lbn = &clipmap->leafbrushNodes[i];
                auto& json_lbn = clipmap_json["leafbrushNodes"][i];

                lbn->axis = json_lbn["axis"].Get<uint8_t>();
                lbn->leafBrushCount = json_lbn["leafBrushCount"].Get<int16_t>();
                lbn->contents = json_lbn["contents"].Get<int32_t>();

                if (lbn->leafBrushCount > 0)
                {
                    const auto& json_lbn_data = json_lbn["data"];

                    if (json_lbn_data.IsString())
                    {
                        int32_t index = std::stoi(json_lbn["data"].Get<std::string>().substr(1));

                        assert(index < static_cast<int32_t>(clipmap->numLeafBrushes));
                        assert(index >= 0);

                        lbn->data.leaf.brushes = &clipmap->leafbrushes[index];
                        assert(lbn->data.leaf.brushes);
                    }
                    else if (json_lbn_data.IsArray())
                    {
                        lbn->data.leaf.brushes = local_allocator.allocate_array<unsigned short>(lbn->leafBrushCount);
                        for (short lbn_index = 0; lbn_index < lbn->leafBrushCount; lbn_index++)
                        {
                            lbn->data.leaf.brushes[lbn_index] = json_lbn_data[lbn_index].Get<unsigned short>();
                        }
                    }
                    else
                    {
                        print_error("Broken clipmap: invalid LBN data");
                        return nullptr;
                    }
                }
                else
                {
                    lbn->data.children.dist = json_lbn["data"]["dist"].Get<float>();
                    lbn->data.children.range = json_lbn["data"]["range"].Get<float>();
                    utils::json::copy_array(lbn->data.children.childOffset, json_lbn["data"]["childOffset"]);
                }
            }

            // Tri indices
            auto indiceCountJson = clipmap_json["triIndices"].Size();
            clipmap->triCount = indiceCountJson;
            clipmap->triIndices = clipmap->triCount == 0 ? nullptr : local_allocator.allocate_array<uint16_t>(clipmap->triCount * 3);

            for (size_t i = 0; i < clipmap->triCount * 3; i += 3)
            {
                utils::json::copy_array(&clipmap->triIndices[i], clipmap_json["triIndices"][i / 3]);
            }

            // Walkable
            auto walkableCount = 4 * ((3 * clipmap->triCount + 31) >> 5) * 3;
            assert(clipmap_json["triEdgeIsWalkable"].Size() == walkableCount);
            clipmap->triEdgeIsWalkable = walkableCount == 0 ? nullptr : local_allocator.allocate_array<uint8_t>(walkableCount);
            utils::json::copy_array(clipmap->triEdgeIsWalkable, clipmap_json["triEdgeIsWalkable"]);

            // Borders
            clipmap->borderCount = clipmap_json["borders"].Size();
            clipmap->borders = clipmap->borderCount == 0 ? nullptr : local_allocator.allocate_array<native::CollisionBorder>(clipmap->borderCount);
            for (size_t i = 0; i < clipmap->borderCount; i++)
            {
                auto border = &clipmap->borders[i];
                auto& json_border = clipmap_json["borders"][i];

                utils::json::copy_array(border->distEq, json_border["distEq"]);
                border->zBase = json_border["zBase"].Get<float>();
                border->zSlope = json_border["zSlope"].Get<float>();
                border->start = json_border["start"].Get<float>();
                border->length = json_border["length"].Get<float>();
            }

            // Collision partitions
            clipmap->partitionCount = clipmap_json["partitions"].Size();
            clipmap->partitions =
                clipmap->partitionCount == 0 ? nullptr : local_allocator.allocate_array<native::CollisionPartition>(clipmap->partitionCount);
            for (size_t i = 0; i < clipmap->partitionCount; i++)
            {
                auto partition = &clipmap->partitions[i];
                auto& json_partition = clipmap_json["partitions"][i];

                partition->triCount = json_partition["triCount"].Get<uint8_t>();
                partition->firstVertSegment = json_partition["firstVertSegment"].Get<uint8_t>();
                partition->firstTri = json_partition["firstTri"].Get<int32_t>();
                partition->borderCount = json_partition["borderCount"].Get<uint8_t>();

                if (partition->borderCount > 0)
                {
                    // They're always consecutive (I checked)
                    auto index = std::stoi(json_partition["firstBorder"].Get<std::string>().substr(1));
                    assert(clipmap->borders);
                    partition->borders = &clipmap->borders[index];
                }
            }

            // Tree
            clipmap->aabbTreeCount = clipmap_json["aabbTrees"].Size();
            clipmap->aabbTrees =
                clipmap->aabbTreeCount == 0 ? nullptr : local_allocator.allocate_array<native::CollisionAabbTree>(clipmap->aabbTreeCount);
            for (size_t i = 0; i < clipmap->aabbTreeCount; i++)
            {
                auto tree = &clipmap->aabbTrees[i];
                auto& json_tree = clipmap_json["aabbTrees"][i];

                utils::json::copy_array(tree->midPoint, json_tree["midPoint"]);
                utils::json::copy_array(tree->halfSize, json_tree["halfSize"]);
                tree->materialIndex = json_tree["materialIndex"].Get<uint16_t>();
                tree->childCount = json_tree["childCount"].Get<uint16_t>();
                tree->u = json_tree["u"].Get<uint32_t>();
            }

            // CModels
            clipmap->numSubModels = clipmap_json["cmodels"].Size();
            clipmap->cmodels = clipmap->numSubModels == 0 ? nullptr : local_allocator.allocate_array<native::cmodel_t>(clipmap->numSubModels);
            for (size_t i = 0; i < clipmap->numSubModels; i++)
            {
                auto cmodel = &clipmap->cmodels[i];
                auto& json_cmodel = clipmap_json["cmodels"][i];

                cmodel->bounds = utils::json::read_bounds(json_cmodel["bounds"]);
                cmodel->radius = json_cmodel["radius"].Get<float>();

                auto leaf = &cmodel->leaf;
                auto& json_leaf = json_cmodel["leaf"];

                leaf->firstCollAabbIndex = json_leaf["firstCollAabbIndex"].Get<uint16_t>();
                leaf->collAabbCount = json_leaf["collAabbCount"].Get<uint16_t>();
                leaf->brushContents = json_leaf["brushContents"].Get<int32_t>();
                leaf->terrainContents = json_leaf["terrainContents"].Get<int32_t>();
                leaf->leafBrushNode = json_leaf["leafBrushNode"].Get<int32_t>();
                leaf->bounds = utils::json::read_bounds(json_leaf["bounds"]);
            }

            // Brushes
            clipmap->numBrushes = static_cast<uint16_t>(clipmap_json["brushes"].Size());
            clipmap->brushes = clipmap->numBrushes == 0 ? nullptr : local_allocator.allocate_array<native::cbrush_t>(clipmap->numBrushes);
            for (size_t i = 0; i < clipmap->numBrushes; i++)
            {
                auto brush = &clipmap->brushes[i];
                auto& json_brush = clipmap_json["brushes"][i];

                brush->glassPieceIndex = json_brush["glassPieceIndex"].Get<uint16_t>();
                brush->numsides = json_brush["numsides"].Get<uint16_t>();

                if (brush->numsides)
                {
                    // Always consecutive
                    auto index = std::stoi(json_brush["firstSide"].Get<std::string>().substr(1));
                    assert(clipmap->brushsides);
                    brush->sides = &clipmap->brushsides[index];
                }

                if (json_brush["baseAdjacentSide"].IsString()) // Not null that means
                {
                    auto index = std::stoi(json_brush["baseAdjacentSide"].Get<std::string>().substr(1));
                    assert(clipmap->brushEdges);
                    brush->baseAdjacentSide = &clipmap->brushEdges[index];
                }

                for (size_t x = 0; x < 2; x++)
                {
                    utils::json::copy_array(brush->axialMaterialNum[x], json_brush["axialMaterialNum"][x]);
                    utils::json::copy_array(brush->firstAdjacentSideOffsets[x], json_brush["firstAdjacentSideOffsets"][x]);
                    utils::json::copy_array(brush->edgeCount[x], json_brush["edgeCount"][x]);
                }
            }

            assert(clipmap_json["brushes"].Size() == clipmap_json["brushBounds"].Size());
            clipmap->brushBounds = local_allocator.allocate_array<native::Bounds>(clipmap->numBrushes);
            for (size_t i = 0; i < clipmap->numBrushes; i++)
            {
                clipmap->brushBounds[i] = utils::json::read_bounds(clipmap_json["brushBounds"][i]);
            }

            assert(clipmap_json["brushes"].Size() == clipmap_json["brushContents"].Size());
            clipmap->brushContents = local_allocator.allocate_array<int32_t>(clipmap->numBrushes);
            utils::json::copy_array(clipmap->brushContents, clipmap_json["brushContents"]);

            auto& json_ents = clipmap_json["mapEnts"];
            if (!json_ents.IsNull())
            {
                auto ents_name = json_ents["name"].GetString();
                clipmap->mapEnts = find<native::MapEnts>(native::XAssetType::ASSET_TYPE_MAP_ENTS, ents_name);
                RETURN_IF_NULL(clipmap->mapEnts);

                if (clipmap->mapEnts)
                {
                    auto& json_trigger = json_ents["trigger"];
                    auto trigger = &clipmap->mapEnts->trigger;
                    trigger->count = json_trigger["models"].Size();
                    trigger->models = local_allocator.allocate_array<native::TriggerModel>(trigger->count);

                    for (size_t i = 0; i < trigger->count; i++)
                    {
                        trigger->models[i].contents = json_trigger["models"][i]["contents"].Get<int32_t>();
                        trigger->models[i].hullCount = json_trigger["models"][i]["hullCount"].Get<uint16_t>();
                        trigger->models[i].firstHull = json_trigger["models"][i]["firstHull"].Get<uint16_t>();
                    }

                    trigger->hullCount = json_trigger["hulls"].Size();
                    trigger->hulls = local_allocator.allocate_array<native::TriggerHull>(trigger->hullCount);
                    for (size_t i = 0; i < trigger->hullCount; i++)
                    {
                        trigger->hulls[i].bounds = utils::json::read_bounds(json_trigger["hulls"][i]["bounds"]);

                        trigger->hulls[i].contents = json_trigger["hulls"][i]["contents"].Get<int32_t>();
                        trigger->hulls[i].firstSlab = json_trigger["hulls"][i]["firstSlab"].Get<uint16_t>();
                        trigger->hulls[i].slabCount = json_trigger["hulls"][i]["slabCount"].Get<uint16_t>();
                    }

                    trigger->slabCount = json_trigger["slabs"].Size();
                    trigger->slabs = local_allocator.allocate_array<native::TriggerSlab>(trigger->slabCount);
                    for (size_t i = 0; i < trigger->slabCount; i++)
                    {
                        utils::json::copy_array(trigger->slabs[i].dir, json_trigger["slabs"][i]["dir"]);
                        trigger->slabs[i].midPoint = json_trigger["slabs"][i]["midPoint"].Get<float>();
                        trigger->slabs[i].halfSize = json_trigger["slabs"][i]["halfSize"].Get<float>();
                    }

                    // Stages
                    clipmap->mapEnts->stageCount = static_cast<char>(json_ents["stages"].Size());
                    clipmap->mapEnts->stages = local_allocator.allocate_array<native::Stage>(clipmap->mapEnts->stageCount);
                    for (auto i = 0; i < clipmap->mapEnts->stageCount; i++)
                    {
                        auto stage = &clipmap->mapEnts->stages[i];
                        auto& json_stage = json_ents["stages"][i];

                        stage->name = local_allocator.duplicate_string(json_stage["name"].GetString());
                        utils::json::copy_array(stage->origin, json_stage["origin"]);
                        stage->triggerIndex = json_stage["triggerIndex"].Get<uint16_t>();
                        stage->sunPrimaryLightIndex = json_stage["sunPrimaryLightIndex"].Get<char>();
                    }
                }
            }

            // SmodelNodes
            clipmap->smodelNodeCount = static_cast<uint16_t>(clipmap_json["smodelNodes"].Size());
            clipmap->smodelNodes =
                clipmap->smodelNodeCount == 0 ? nullptr : local_allocator.allocate_array<native::SModelAabbNode>(clipmap->smodelNodeCount);
            for (size_t i = 0; i < clipmap->smodelNodeCount; i++)
            {
                auto& json_node = clipmap_json["smodelNodes"][i];
                auto node = &clipmap->smodelNodes[i];

                node->bounds = utils::json::read_bounds(json_node["bounds"]);
                node->firstChild = json_node["firstChild"].Get<uint16_t>();
                node->childCount = json_node["childCount"].Get<uint16_t>();
            }

            for (size_t i = 0; i < 2; i++)
            {
                if (clipmap_json["dynEntities"][i].IsNull()) continue;

                clipmap->dynEntCount[i] = static_cast<uint16_t>(clipmap_json["dynEntities"][i].Size());

                auto& json_entities = clipmap_json["dynEntities"][i];
                clipmap->dynEntClientList[i] =
                    clipmap->dynEntCount[i] == 0 ? nullptr : local_allocator.allocate_array<native::DynEntityClient>(clipmap->dynEntCount[i]);
                clipmap->dynEntCollList[i] =
                    clipmap->dynEntCount[i] == 0 ? nullptr : local_allocator.allocate_array<native::DynEntityColl>(clipmap->dynEntCount[i]);
                clipmap->dynEntPoseList[i] =
                    clipmap->dynEntCount[i] == 0 ? nullptr : local_allocator.allocate_array<native::DynEntityPose>(clipmap->dynEntCount[i]);
                clipmap->dynEntDefList[i] =
                    clipmap->dynEntCount[i] == 0 ? nullptr : local_allocator.allocate_array<native::DynEntityDef>(clipmap->dynEntCount[i]);

                for (size_t j = 0; j < clipmap->dynEntCount[i]; j++)
                {
                    auto& json_entity = json_entities[j]["dynEntityDef"];
                    auto entity = &clipmap->dynEntDefList[i][j];

                    entity->type = static_cast<native::DynEntityType>(json_entity["type"].Get<int32_t>());
                    utils::json::copy_array(entity->pose.quat, json_entity["pose"]["quat"]);
                    utils::json::copy_array(entity->pose.origin, json_entity["pose"]["origin"]);

                    entity->xModel = json_entity["xModel"].IsString()
                                         ? find<native::XModel>(native::XAssetType::ASSET_TYPE_XMODEL, json_entity["xModel"].GetString())
                                         : nullptr;
                    entity->brushModel = json_entity["brushModel"].Get<uint16_t>();
                    entity->physicsBrushModel = json_entity["physicsBrushModel"].Get<uint16_t>();
                    entity->destroyFx = json_entity["destroyFx"].IsString()
                                            ? find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, json_entity["destroyFx"].GetString())
                                            : nullptr;
                    entity->physPreset = json_entity["physPreset"].IsString() ? find<native::PhysPreset>(native::XAssetType::ASSET_TYPE_PHYSPRESET,
                                                                                                         json_entity["physPreset"].GetString())
                                                                              : nullptr;
                    entity->health = json_entity["health"].Get<int32_t>();

                    utils::json::copy_array(entity->mass.centerOfMass, json_entity["mass"]["centerOfMass"]);
                    utils::json::copy_array(entity->mass.momentsOfInertia, json_entity["mass"]["momentsOfInertia"]);
                    utils::json::copy_array(entity->mass.productsOfInertia, json_entity["mass"]["productsOfInertia"]);

                    entity->contents = json_entity["contents"].Get<int32_t>();
                }
            }

            clipmap->checksum = clipmap_json["checksum"].Get<uint32_t>();
        }
        catch (const std::exception& e)
        {
            print_error("Malformed JSON for clipmap {}! {}", _name, e.what());
            return nullptr;
        }

        return clipmap;
    }

    bool iclipmap::write_internal(const native::XAssetHeader& header) const
    {
        return write_plaintext(header);
    }

    bool iclipmap::write_plaintext(const native::XAssetHeader& header) const
    {
        assert(header.clipMap);

        const auto clip_map = header.clipMap;

        if (!clip_map) return false;

        utils::memory::allocator mem_allocator{};

        std::unordered_map<native::cplane_s*, int> planes{}; // +++
        std::unordered_map<native::cbrushside_t*, int> brush_sides{}; // +
        std::unordered_map<uint8_t*, int> brush_edges{}; // +
        std::unordered_map<uint16_t*, int> leaf_brushes{}; // ++
        std::unordered_map<native::CollisionBorder*, int> borders{}; // +

        utils::memory::allocator str_duplicator;
        rapidjson::Document output(rapidjson::kObjectType);
        auto& allocator = output.GetAllocator();

        auto ushort_to_array = [&allocator](const uint16_t* array, int size)
        {
            return utils::json::make_json_array(array, size, allocator);
        };

        auto uchar_to_array = [&allocator](const uint8_t* array, int size)
        {
            return utils::json::make_json_array(array, size, allocator);
        };

        auto placement_to_json = [&allocator](const native::GfxPlacement& placement)
        {
            rapidjson::Value placement_json(rapidjson::kObjectType);

            placement_json.AddMember("quat", utils::json::make_json_array(placement.quat, 4, allocator), allocator);
            placement_json.AddMember("origin", utils::json::make_json_array(placement.origin, 3, allocator), allocator);

            return placement_json;
        };

        auto leaf_to_json = [&allocator](const native::cLeaf_t& leaf)
        {
            rapidjson::Value json_leave(rapidjson::kObjectType);

            json_leave.AddMember("firstCollAabbIndex", leaf.firstCollAabbIndex, allocator);
            json_leave.AddMember("collAabbCount", leaf.collAabbCount, allocator);
            json_leave.AddMember("brushContents", leaf.brushContents, allocator);
            json_leave.AddMember("terrainContents", leaf.terrainContents, allocator);
            json_leave.AddMember("leafBrushNode", leaf.leafBrushNode, allocator);
            json_leave.AddMember("bounds", utils::json::to_json(leaf.bounds, allocator), allocator);

            return json_leave;
        };

        output.AddMember("version", IW4X_CLIPMAP_VERSION, allocator);
        output.AddMember("name", RAPIDJSON_STR(clip_map->name), allocator);
        output.AddMember("isInUse", clip_map->isInUse, allocator);

        // Planes
        rapidjson::Value json_planes(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->planeCount; i++)
        {
            rapidjson::Value json_plane(rapidjson::kObjectType);
            auto plane = &clip_map->planes[i];

            json_plane.AddMember("normal", utils::json::make_json_array(plane->normal, ARRAYSIZE(plane->normal), allocator), allocator);

            json_plane.AddMember("dist", plane->dist, allocator);
            json_plane.AddMember("type", plane->type, allocator);

            json_planes.PushBack(json_plane, allocator);

            planes[plane] = i;
        }

        output.AddMember("planes", json_planes, allocator);

        // Static models
        rapidjson::Value json_static_models(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numStaticModels; i++)
        {
            rapidjson::Value json_static_model(rapidjson::kObjectType);
            auto static_model = &clip_map->staticModelList[i];

            json_static_model.AddMember(
                "xmodel",
                static_model->xmodel == nullptr ? rapidjson::Value(rapidjson::kNullType) : RAPIDJSON_STR(static_model->xmodel->name),
                allocator);
            json_static_model.AddMember("origin", utils::json::make_json_array(static_model->origin, ARRAYSIZE(static_model->origin), allocator), allocator);

            if (static_model->xmodel)
            {
                assets->write(native::ASSET_TYPE_XMODEL, static_model->xmodel);
            }

            rapidjson::Value inv_scaled_axis(rapidjson::kArrayType);
            for (size_t j = 0; j < ARRAYSIZE(static_model->invScaledAxis); j++)
            {
                inv_scaled_axis.PushBack(utils::json::make_json_array(static_model->invScaledAxis[j], ARRAYSIZE(static_model->invScaledAxis[j]), allocator),
                                         allocator);
            }

            json_static_model.AddMember("invScaledAxis", inv_scaled_axis, allocator);

            json_static_model.AddMember("absBounds", utils::json::to_json(static_model->absBounds, allocator), allocator);

            json_static_models.PushBack(json_static_model, allocator);
        }

        output.AddMember("staticModelList", json_static_models, allocator);

        // Materials
        rapidjson::Value json_materials(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numMaterials; i++)
        {
            rapidjson::Value json_material(rapidjson::kObjectType);
            auto material = &clip_map->materials[i];

            json_material.AddMember("name", RAPIDJSON_STR(material->name), allocator);

            json_material.AddMember("surfaceFlags", material->surfaceFlags, allocator);
            json_material.AddMember("contents", material->contents, allocator);

            json_materials.PushBack(json_material, allocator);
        }

        output.AddMember("materials", json_materials, allocator);

        // Brushsides
        rapidjson::Value json_brush_sides(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numBrushSides; i++)
        {
            rapidjson::Value json_brush_side(rapidjson::kObjectType);
            auto brush_side = &clip_map->brushsides[i];

            auto index = planes[brush_side->plane];
            auto plane_index = std::format("#{}", index);
            json_brush_side.AddMember("plane", RAPIDJSON_STR(str_duplicator.duplicate_string(plane_index)), allocator);

            json_brush_side.AddMember("materialNum", brush_side->materialNum, allocator);
            json_brush_side.AddMember("firstAdjacentSideOffset", brush_side->firstAdjacentSideOffset, allocator);
            json_brush_side.AddMember("edgeCount", brush_side->edgeCount, allocator);

            json_brush_sides.PushBack(json_brush_side, allocator);

            brush_sides[brush_side] = i;
        }

        output.AddMember("brushsides", json_brush_sides, allocator);

        // Brush edges
        rapidjson::Value json_brush_edges(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numBrushEdges; i++)
        {
            json_brush_edges.PushBack(clip_map->brushEdges[i], allocator);
            brush_edges[&clip_map->brushEdges[i]] = i;
        }

        output.AddMember("brushEdges", json_brush_edges, allocator);

        // Brush nodes
        rapidjson::Value json_nodes(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numNodes; i++)
        {
            auto node = &clip_map->nodes[i];

            rapidjson::Value json_node(rapidjson::kObjectType);

            auto index = planes[node->plane];
            auto plane_index = std::format("#{}", index);
            json_node.AddMember("plane", RAPIDJSON_STR(str_duplicator.duplicate_string(plane_index)), allocator);

            rapidjson::Value children(rapidjson::kArrayType);
            for (size_t j = 0; j < ARRAYSIZE(node->children); j++)
            {
                children.PushBack(node->children[j], allocator);
            }

            json_node.AddMember("children", children, allocator);

            json_nodes.PushBack(json_node, allocator);
        }

        output.AddMember("nodes", json_nodes, allocator);

        // Leaves
        rapidjson::Value json_leaves(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numLeafs; i++)
        {
            auto leaf = &clip_map->leafs[i];

            json_leaves.PushBack(leaf_to_json(*leaf), allocator);
        }

        output.AddMember("leafs", json_leaves, allocator);

        // Brush leafs
        rapidjson::Value json_brush_leafs(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numLeafBrushes; i++)
        {
            json_brush_leafs.PushBack(clip_map->leafbrushes[i], allocator);
            leaf_brushes[&clip_map->leafbrushes[i]] = i;
        }

        output.AddMember("leafbrushes", json_brush_leafs, allocator);

        // LeafBrushNodes
        rapidjson::Value json_leaf_brush_nodes(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->leafbrushNodesCount; i++)
        {
            rapidjson::Value json_leaf_brush_node(rapidjson::kObjectType);
            auto leaf_brush_node = &clip_map->leafbrushNodes[i];

            json_leaf_brush_node.AddMember("axis", leaf_brush_node->axis, allocator);
            json_leaf_brush_node.AddMember("leafBrushCount", leaf_brush_node->leafBrushCount, allocator);
            json_leaf_brush_node.AddMember("contents", leaf_brush_node->contents, allocator);

            if (leaf_brush_node->leafBrushCount > 0)
            {
                if (leaf_brushes.contains(leaf_brush_node->data.leaf.brushes))
                {
                    auto index_str = std::format("#{}", leaf_brushes[leaf_brush_node->data.leaf.brushes]);
                    json_leaf_brush_node.AddMember("data", RAPIDJSON_STR(mem_allocator.duplicate_string(index_str)), allocator);
                }
                else
                {
                    // This happens on iw3, i suppose it's okay
                    rapidjson::Value lbn_array(rapidjson::kArrayType);
                    for (short brushIndex = 0; brushIndex < leaf_brush_node->leafBrushCount; brushIndex++)
                    {
                        lbn_array.PushBack(leaf_brush_node->data.leaf.brushes[brushIndex], allocator);
                    }

                    json_leaf_brush_node.AddMember("data", lbn_array, allocator);
                }
            }
            else
            {
                rapidjson::Value data(rapidjson::kObjectType);

                data.AddMember("dist", leaf_brush_node->data.children.dist, allocator);
                data.AddMember("range", leaf_brush_node->data.children.range, allocator);

                rapidjson::Value child_offset(rapidjson::kArrayType);
                for (size_t x = 0; x < 2; x++)
                {
                    child_offset.PushBack(leaf_brush_node->data.children.childOffset[x], allocator);
                }

                data.AddMember("childOffset", child_offset, allocator);

                json_leaf_brush_node.AddMember("data", data, allocator);
            }

            json_leaf_brush_nodes.PushBack(json_leaf_brush_node, allocator);
        }

        output.AddMember("leafbrushNodes", json_leaf_brush_nodes, allocator);

        // leafsurfaces (unused)
        rapidjson::Value json_leaf_surfaces(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numLeafSurfaces; i++)
        {
            json_leaf_surfaces.PushBack(clip_map->leafsurfaces[i], allocator);
        }

        output.AddMember("leafsurfaces", json_leaf_surfaces, allocator);

        // vertices
        rapidjson::Value json_vertices(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->vertCount; i++)
        {
            json_vertices.PushBack(utils::json::make_json_array(clip_map->verts[i], 3, allocator), allocator);
        }

        output.AddMember("verts", json_vertices, allocator);

        // tris
        rapidjson::Value json_tris(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->triCount * 3; i += 3)
        {
            assert(clip_map->triIndices[i] != clip_map->triIndices[i + 1]);
            assert(clip_map->triIndices[i + 1] != clip_map->triIndices[i + 2]);
            assert(clip_map->triIndices[i + 2] != clip_map->triIndices[i]);
            json_tris.PushBack(ushort_to_array(&clip_map->triIndices[i], 3), allocator);
        }

        output.AddMember("triIndices", json_tris, allocator);

        rapidjson::Value json_tris_walkable(rapidjson::kArrayType);
        auto walkable_count = 4 * ((3 * clip_map->triCount + 31) >> 5);
        for (size_t i = 0; i < walkable_count * 3; i++)
        {
            json_tris_walkable.PushBack(static_cast<uint8_t>(clip_map->triEdgeIsWalkable[i]), allocator);
        }

        output.AddMember("triEdgeIsWalkable", json_tris_walkable, allocator);

        // Collision borders
        rapidjson::Value json_collision_borders(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->borderCount; i++)
        {
            auto collision_border = &clip_map->borders[i];

            rapidjson::Value json_collision_border(rapidjson::kObjectType);

            json_collision_border.AddMember("distEq", utils::json::make_json_array(collision_border->distEq, ARRAYSIZE(collision_border->distEq), allocator), allocator);
            json_collision_border.AddMember("zBase", collision_border->zBase, allocator);
            json_collision_border.AddMember("zSlope", collision_border->zSlope, allocator);
            json_collision_border.AddMember("start", collision_border->start, allocator);
            json_collision_border.AddMember("length", collision_border->length, allocator);

            json_collision_borders.PushBack(json_collision_border, allocator);

            borders[collision_border] = i;
        }

        output.AddMember("borders", json_collision_borders, allocator);

        // Collision partitions
        rapidjson::Value json_collision_partitions(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->partitionCount; i++)
        {
            auto collision_partition = &clip_map->partitions[i];

            rapidjson::Value json_collision_partition(rapidjson::kObjectType);

            json_collision_partition.AddMember("triCount", collision_partition->triCount, allocator);
            json_collision_partition.AddMember("firstVertSegment", collision_partition->firstVertSegment, allocator);
            json_collision_partition.AddMember("firstTri", collision_partition->firstTri, allocator);
            json_collision_partition.AddMember("borderCount", collision_partition->borderCount, allocator);

            if (collision_partition->borderCount)
            {
                auto index_str = str_duplicator.duplicate_string(std::format("#{}", borders[&collision_partition->borders[0]]));
                json_collision_partition.AddMember("firstBorder", RAPIDJSON_STR(index_str), allocator);
            }

            json_collision_partitions.PushBack(json_collision_partition, allocator);
        }

        output.AddMember("partitions", json_collision_partitions, allocator);

        // Trees
        rapidjson::Value json_aabbtrees(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->aabbTreeCount; i++)
        {
            auto aabbtree = &clip_map->aabbTrees[i];

            rapidjson::Value json_aabbtree(rapidjson::kObjectType);

            json_aabbtree.AddMember("midPoint", utils::json::make_json_array(aabbtree->midPoint, 3, allocator), allocator);
            json_aabbtree.AddMember("halfSize", utils::json::make_json_array(aabbtree->halfSize, 3, allocator), allocator);
            json_aabbtree.AddMember("materialIndex", aabbtree->materialIndex, allocator);
            json_aabbtree.AddMember("childCount", aabbtree->childCount, allocator);
            json_aabbtree.AddMember("u", aabbtree->u, allocator);

            json_aabbtrees.PushBack(json_aabbtree, allocator);
        }

        output.AddMember("aabbTrees", json_aabbtrees, allocator);

        // Cmodels
        rapidjson::Value json_cmodels(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numSubModels; i++)
        {
            auto cmodel = &clip_map->cmodels[i];

            rapidjson::Value json_cmodel(rapidjson::kObjectType);

            json_cmodel.AddMember("bounds", utils::json::to_json(cmodel->bounds, allocator), allocator);
            json_cmodel.AddMember("radius", cmodel->radius, allocator);
            json_cmodel.AddMember("leaf", leaf_to_json(cmodel->leaf), allocator);

            json_cmodels.PushBack(json_cmodel, allocator);
        }

        output.AddMember("cmodels", json_cmodels, allocator);

        // Brushes
        rapidjson::Value json_brushes(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numBrushes; i++)
        {
            auto brush = &clip_map->brushes[i];

            rapidjson::Value json_brush(rapidjson::kObjectType);

            json_brush.AddMember("glassPieceIndex", brush->glassPieceIndex, allocator);
            json_brush.AddMember("numsides", brush->numsides, allocator);

            // Sides
            if (brush->numsides > 0)
            {
                auto index_str = str_duplicator.duplicate_string(std::format("#{}", brush_sides[brush->sides]));
                json_brush.AddMember("firstSide", RAPIDJSON_STR(index_str), allocator);
            }

            if (brush->baseAdjacentSide)
            {
                auto index_str = str_duplicator.duplicate_string(std::format("#{}", brush_edges[brush->baseAdjacentSide]));
                json_brush.AddMember("baseAdjacentSide", RAPIDJSON_STR(index_str), allocator);
            }
            else
            {
                json_brush.AddMember("baseAdjacentSide", rapidjson::Value(rapidjson::kNullType), allocator);
            }

            rapidjson::Value axial_material_num(rapidjson::kArrayType);
            for (size_t x = 0; x < 2; x++)
            {
                axial_material_num.PushBack(ushort_to_array(brush->axialMaterialNum[x], 3), allocator);
            }
            json_brush.AddMember("axialMaterialNum", axial_material_num, allocator);

            rapidjson::Value first_adjacent_side_offsets(rapidjson::kArrayType);
            for (size_t x = 0; x < 2; x++)
            {
                first_adjacent_side_offsets.PushBack(uchar_to_array(brush->firstAdjacentSideOffsets[x], 3), allocator);
            }
            json_brush.AddMember("firstAdjacentSideOffsets", first_adjacent_side_offsets, allocator);

            rapidjson::Value edge_count(rapidjson::kArrayType);
            for (size_t x = 0; x < 2; x++)
            {
                edge_count.PushBack(uchar_to_array(brush->edgeCount[x], 3), allocator);
            }
            json_brush.AddMember("edgeCount", edge_count, allocator);

            json_brushes.PushBack(json_brush, allocator);
        }

        output.AddMember("brushes", json_brushes, allocator);

        // Brushbounds
        rapidjson::Value json_brush_bounds(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numBrushes; i++)
        {
            json_brush_bounds.PushBack(utils::json::to_json(clip_map->brushBounds[i], allocator), allocator);
        }

        output.AddMember("brushBounds", json_brush_bounds, allocator);

        // Brush contents
        rapidjson::Value json_brush_contents(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->numBrushes; i++)
        {
            json_brush_contents.PushBack(clip_map->brushContents[i], allocator);
        }

        output.AddMember("brushContents", json_brush_contents, allocator);

        // ENTITIES
        if (clip_map->mapEnts)
        {
            static_assert(sizeof native::TriggerSlab == 20);
            static_assert(sizeof native::TriggerModel == 8);
            static_assert(sizeof native::TriggerHull == 32);

            rapidjson::Value json_map_ents(rapidjson::kObjectType);
            const auto ents = clip_map->mapEnts;

            json_map_ents.AddMember("name", RAPIDJSON_STR(ents->name), allocator);
            assets->write(native::ASSET_TYPE_MAP_ENTS, clip_map->mapEnts);

            rapidjson::Value json_trigger(rapidjson::kObjectType);

            rapidjson::Value json_trigger_models(rapidjson::kArrayType);
            for (size_t i = 0; i < ents->trigger.count; i++)
            {
                rapidjson::Value json_trigger_model(rapidjson::kObjectType);
                json_trigger_model.AddMember("contents", ents->trigger.models[i].contents, allocator);
                json_trigger_model.AddMember("hullCount", ents->trigger.models[i].hullCount, allocator);
                json_trigger_model.AddMember("firstHull", ents->trigger.models[i].firstHull, allocator);

                json_trigger_models.PushBack(json_trigger_model, allocator);
            }

            json_trigger.AddMember("models", json_trigger_models, allocator);

            rapidjson::Value json_trigger_hulls(rapidjson::kArrayType);
            for (size_t i = 0; i < ents->trigger.hullCount; i++)
            {
                rapidjson::Value json_trigger_hull(rapidjson::kObjectType);
                json_trigger_hull.AddMember("bounds", utils::json::to_json(ents->trigger.hulls[i].bounds, allocator), allocator);
                json_trigger_hull.AddMember("contents", ents->trigger.hulls[i].contents, allocator);
                json_trigger_hull.AddMember("slabCount", ents->trigger.hulls[i].slabCount, allocator);
                json_trigger_hull.AddMember("firstSlab", ents->trigger.hulls[i].firstSlab, allocator);

                json_trigger_hulls.PushBack(json_trigger_hull, allocator);
            }

            json_trigger.AddMember("hulls", json_trigger_hulls, allocator);

            rapidjson::Value json_trigger_slabs(rapidjson::kArrayType);
            for (size_t i = 0; i < ents->trigger.slabCount; i++)
            {
                rapidjson::Value json_trigger_slab(rapidjson::kObjectType);
                json_trigger_slab.AddMember("dir", utils::json::make_json_array(ents->trigger.slabs[i].dir, 3, allocator), allocator);
                json_trigger_slab.AddMember("midPoint", ents->trigger.slabs[i].midPoint, allocator);
                json_trigger_slab.AddMember("halfSize", ents->trigger.slabs[i].halfSize, allocator);

                json_trigger_slabs.PushBack(json_trigger_slab, allocator);
            }

            json_trigger.AddMember("slabs", json_trigger_slabs, allocator);
            json_map_ents.AddMember("trigger", json_trigger, allocator);

            rapidjson::Value json_stages(rapidjson::kArrayType);
            for (auto i = 0; i < ents->stageCount; i++)
            {
                auto stage = &ents->stages[i];
                rapidjson::Value json_stage(rapidjson::kObjectType);
                json_stage.AddMember("name", RAPIDJSON_STR(stage->name), allocator);
                json_stage.AddMember("origin", utils::json::make_json_array(stage->origin, 3, allocator), allocator);
                json_stage.AddMember("triggerIndex", stage->triggerIndex, allocator);
                json_stage.AddMember("sunPrimaryLightIndex", stage->sunPrimaryLightIndex, allocator);

                json_stages.PushBack(json_stage, allocator);
            }

            json_map_ents.AddMember("stages", json_stages, allocator);

            output.AddMember("mapEnts", json_map_ents, allocator);
        }
        else
        {
            output.AddMember("mapEnts", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        // Smodel nodes
        rapidjson::Value json_smodelnodes(rapidjson::kArrayType);
        for (size_t i = 0; i < clip_map->smodelNodeCount; i++)
        {
            auto smodelNode = &clip_map->smodelNodes[i];

            rapidjson::Value json_smodelnode(rapidjson::kObjectType);

            json_smodelnode.AddMember("bounds", utils::json::to_json(smodelNode->bounds, allocator), allocator);
            json_smodelnode.AddMember("firstChild", smodelNode->firstChild, allocator);
            json_smodelnode.AddMember("childCount", smodelNode->childCount, allocator);

            json_smodelnodes.PushBack(json_smodelnode, allocator);
        }

        output.AddMember("smodelNodes", json_smodelnodes, allocator);

        // Dynent
        rapidjson::Value json_dyn_entities(rapidjson::kArrayType);
        for (size_t i = 0; i < ARRAYSIZE(clip_map->dynEntCount); i++)
        {
            auto def_list = clip_map->dynEntDefList[i];
            if (def_list)
            {
                rapidjson::Value json_dyn_entity_def_list(rapidjson::kArrayType);

                for (size_t j = 0; j < clip_map->dynEntCount[i]; j++)
                {
                    rapidjson::Value json_dyn_entity_def_pack(rapidjson::kObjectType);
                    rapidjson::Value json_dyn_entity_def(rapidjson::kObjectType);
                    auto def = &def_list[j];

                    json_dyn_entity_def.AddMember("type", def->type, allocator);
                    json_dyn_entity_def.AddMember("pose", placement_to_json(def->pose), allocator);
                    json_dyn_entity_def.AddMember(
                        "xModel", def->xModel ? RAPIDJSON_STR(def->xModel->name) : rapidjson::Value(rapidjson::kNullType), allocator);
                    json_dyn_entity_def.AddMember("brushModel", def->brushModel, allocator);
                    json_dyn_entity_def.AddMember("physicsBrushModel", def->physicsBrushModel, allocator);
                    json_dyn_entity_def.AddMember(
                        "destroyFx", def->destroyFx ? RAPIDJSON_STR(def->destroyFx->name) : rapidjson::Value(rapidjson::kNullType), allocator);
                    json_dyn_entity_def.AddMember(
                        "physPreset", def->physPreset ? RAPIDJSON_STR(def->physPreset->name) : rapidjson::Value(rapidjson::kNullType), allocator);
                    json_dyn_entity_def.AddMember("health", def->health, allocator);

                    rapidjson::Value json_mass(rapidjson::kObjectType);
                    json_mass.AddMember("centerOfMass", utils::json::make_json_array(def->mass.centerOfMass, 3, allocator), allocator);
                    json_mass.AddMember("momentsOfInertia", utils::json::make_json_array(def->mass.momentsOfInertia, 3, allocator), allocator);
                    json_mass.AddMember("productsOfInertia", utils::json::make_json_array(def->mass.productsOfInertia, 3, allocator), allocator);
                    json_dyn_entity_def.AddMember("mass", json_mass, allocator);

                    json_dyn_entity_def.AddMember("contents", def->contents, allocator);

                    json_dyn_entity_def_pack.AddMember("dynEntityDef", json_dyn_entity_def, allocator);

                    if (def->xModel)
                    {
                        assets->write(native::ASSET_TYPE_XMODEL, def->xModel);
                    }

                    if (def->destroyFx)
                    {
                        assets->write(native::ASSET_TYPE_FX, def->destroyFx);
                    }

                    if (def->physPreset)
                    {
                        assets->write(native::ASSET_TYPE_PHYSPRESET, def->physPreset);
                    }

                    json_dyn_entity_def_list.PushBack(json_dyn_entity_def_pack, allocator);
                }

                json_dyn_entities.PushBack(json_dyn_entity_def_list, allocator);
            }
            else
            {
                json_dyn_entities.PushBack(rapidjson::Value(rapidjson::kNullType), allocator);
            }
        }
        output.AddMember("dynEntities", json_dyn_entities, allocator);

        // Checksum
        output.AddMember("checksum", clip_map->checksum, allocator);

        // Write to disk
        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<
					/*typename OutputStream  */ rapidjson::StringBuffer,
					/*typename SourceEncoding*/ rapidjson::UTF8<>,
					/*typename TargetEncoding*/ rapidjson::UTF8<>,
					/*typename StackAllocator*/ rapidjson::CrtAllocator,
					/*unsigned writeFlags*/     rapidjson::kWriteNanAndInfFlag 
			>
			writer(buff);
        writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
        output.Accept(writer);

        utils::io::write_file(get_work_path(header).string(), buff.GetString());

        return true;
    }
} // namespace iw4of::interfaces
