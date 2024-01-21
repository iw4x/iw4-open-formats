#include <std_include.hpp>

#include "assets/assets.hpp"
#include "ixmodel.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

#define IW4X_MODEL_VERSION 10

namespace iw4of::interfaces
{
    bool ixmodel::write_internal(const native::XAssetHeader& header) const
    {
        auto asset = header.model;
        assert(asset);

        utils::stream buffer;

        buffer.save_array("IW4xModl", 8);
        buffer.save_object(IW4X_MODEL_VERSION);

        buffer.save_object(*asset);

        if (asset->name)
        {
            buffer.save_string(asset->name);
        }

        if (asset->boneNames)
        {
            for (uint8_t i = 0; i < asset->numBones; ++i)
            {
                buffer.save_string(assets->read_from_stringtable(asset->boneNames[i]));
            }
        }

        if (asset->parentList)
        {
            buffer.save_array_if_not_existing(asset->parentList, asset->numBones - asset->numRootBones);
        }

        if (asset->quats)
        {
            buffer.save_array_if_not_existing(asset->quats, (asset->numBones - asset->numRootBones));
        }

        if (asset->trans)
        {
            buffer.save_array_if_not_existing(asset->trans, (asset->numBones - asset->numRootBones));
        }

        if (asset->partClassification)
        {
            buffer.save_array_if_not_existing(asset->partClassification, asset->numBones);
        }

        if (asset->baseMat)
        {
            buffer.save_array_if_not_existing(asset->baseMat, asset->numBones);
        }

        if (asset->materialHandles)
        {
            buffer.save_array(asset->materialHandles, asset->numsurfs);

            for (uint8_t i = 0; i < asset->numsurfs; ++i)
            {
                if (asset->materialHandles[i])
                {
                    buffer.save_string(asset->materialHandles[i]->info.name);
                    assets->write(native::ASSET_TYPE_MATERIAL, asset->materialHandles[i]);
                }
            }
        }

        // Save_XModelLodInfoArray
        {
            for (int i = 0; i < 4; ++i)
            {
                if (asset->lodInfo[i].modelSurfs)
                {
                    ixmodel::write(asset->lodInfo[i].modelSurfs, &buffer);
                }
            }
        }

        // Save_XModelCollSurfArray
        if (asset->collSurfs)
        {
            buffer.save_array(asset->collSurfs, asset->numCollSurfs);

            for (int i = 0; i < asset->numCollSurfs; ++i)
            {
                native::XModelCollSurf_s* collSurf = &asset->collSurfs[i];

                if (collSurf->collTris)
                {
                    buffer.save_array(collSurf->collTris, collSurf->numCollTris);
                }
            }
        }

        if (asset->boneInfo)
        {
            buffer.save_array(asset->boneInfo, asset->numBones);
        }

        if (asset->physPreset)
        {
            if (asset->physPreset->name)
            {
                buffer.save_string(asset->physPreset->name);
                assets->write(native::ASSET_TYPE_PHYSPRESET, asset->physPreset);
            }
        }

        if (asset->physCollmap)
        {
            auto collmap = asset->physCollmap;
            buffer.save_object(*collmap);

            if (collmap->name)
            {
                buffer.save_string(collmap->name);
            }

            if (collmap->geoms)
            {
                buffer.save_array(collmap->geoms, collmap->count);

                for (uint32_t i = 0; i < collmap->count; ++i)
                {
                    auto geom = &collmap->geoms[i];

                    if (geom->brushWrapper)
                    {
                        native::BrushWrapper* brush = geom->brushWrapper;
                        buffer.save_array_if_not_existing(brush, 1);
                        {
                            if (brush->brush.sides)
                            {
                                buffer.save_array_if_not_existing(brush->brush.sides, brush->brush.numsides);

                                // Save_cbrushside_tArray
                                for (uint16_t j = 0; j < brush->brush.numsides; ++j)
                                {
                                    native::cbrushside_t* side = &brush->brush.sides[j];

                                    // TODO: Add pointer support
                                    if (side->plane)
                                    {
                                        buffer.save_array_if_not_existing(side->plane, 1);
                                    }
                                }
                            }

                            if (brush->brush.baseAdjacentSide)
                            {
                                buffer.save_array_if_not_existing(brush->brush.baseAdjacentSide, brush->totalEdgeCount);
                            }
                        }

                        // TODO: Add pointer support
                        if (brush->planes)
                        {
                            buffer.save_array_if_not_existing(brush->planes, brush->brush.numsides);
                        }
                    }
                }
            }
        }

        return utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());
    }

    void ixmodel::write(const native::XSurfaceCollisionTree* entry, utils::stream* buffer) const
    {
        buffer->save_object(*entry);

        if (entry->nodes)
        {
            buffer->save_array_if_not_existing(entry->nodes, entry->nodeCount);
        }

        if (entry->leafs)
        {
            buffer->save_array_if_not_existing(entry->leafs, entry->leafCount);
        }
    }

    void ixmodel::write(const native::XSurface* surf, utils::stream* buffer) const
    {
        if (surf->vertInfo.vertsBlend)
        {
            buffer->save_array_if_not_existing(surf->vertInfo.vertsBlend,
                                               surf->vertInfo.vertCount[0] + (surf->vertInfo.vertCount[1] * 3) + (surf->vertInfo.vertCount[2] * 5) +
                                                   (surf->vertInfo.vertCount[3] * 7));
        }

        // Access vertex block
        if (surf->verts0)
        {
            buffer->save_array_if_not_existing(surf->verts0, surf->vertCount);
        }

        // Save_XRigidVertListArray
        if (surf->vertList)
        {
            buffer->save_array_if_not_existing(surf->vertList, surf->vertListCount);

            for (uint32_t i = 0; i < surf->vertListCount; ++i)
            {
                native::XRigidVertList* rigidVertList = &surf->vertList[i];

                if (rigidVertList->collisionTree)
                {
                    ixmodel::write(rigidVertList->collisionTree, buffer);
                }
            }
        }

        // Access index block
        if (surf->triIndices)
        {
            buffer->save_array_if_not_existing(surf->triIndices, surf->triCount * 3);
        }
    }

    void ixmodel::write(const native::XModelSurfs* asset, utils::stream* buffer) const
    {
        buffer->save_object(*asset);

        if (asset->name)
        {
            buffer->save_string(asset->name);
        }

        if (asset->surfaces)
        {
            buffer->save_array_if_not_existing(asset->surfaces, asset->numSurfaces);

            for (int i = 0; i < asset->numSurfaces; ++i)
            {
                ixmodel::write(&asset->surfaces[i], buffer);
            }
        }
    }

    void interfaces::ixmodel::read(native::XSurfaceCollisionTree* entry, utils::stream::reader* reader) const
    {
        if (entry->nodes)
        {
            entry->nodes = reader->read_array_once<native::XSurfaceCollisionNode>(entry->nodeCount);
        }

        if (entry->leafs)
        {
            entry->leafs = reader->read_array_once<native::XSurfaceCollisionLeaf>(entry->leafCount);
        }
    }

    void interfaces::ixmodel::read(native::XSurface* surf, utils::stream::reader* reader) const
    {
        if (surf->vertInfo.vertsBlend)
        {
            surf->vertInfo.vertsBlend = reader->read_array_once<uint16_t>(surf->vertInfo.vertCount[0] + (surf->vertInfo.vertCount[1] * 3) +
                                                                          (surf->vertInfo.vertCount[2] * 5) + (surf->vertInfo.vertCount[3] * 7));
        }

        // Access vertex block
        if (surf->verts0)
        {
            surf->verts0 = reader->read_array_once<native::GfxPackedVertex>(surf->vertCount);
        }

        // Save_XRigidVertListArray
        if (surf->vertList)
        {
            surf->vertList = reader->read_array_once<native::XRigidVertList>(surf->vertListCount);

            for (uint32_t i = 0; i < surf->vertListCount; ++i)
            {
                native::XRigidVertList* rigidVertList = &surf->vertList[i];

                if (rigidVertList->collisionTree)
                {
                    rigidVertList->collisionTree = reader->read_object<native::XSurfaceCollisionTree>();
                    read(rigidVertList->collisionTree, reader);
                }
            }
        }

        // Access index block
        if (surf->triIndices)
        {
            surf->triIndices = reader->read_array_once<uint16_t>(surf->triCount * 3);
        }
    }

    void interfaces::ixmodel::read(native::XModelSurfs* asset, utils::stream::reader* reader) const
    {
        if (asset->name)
        {
            asset->name = reader->read_cstring();
        }

        if (asset->surfaces)
        {
            asset->surfaces = reader->read_array_once<native::XSurface>(asset->numSurfaces);

            for (int i = 0; i < asset->numSurfaces; ++i)
            {
                read(&asset->surfaces[i], reader);
            }
        }
    }

    void* ixmodel::read_internal(const std::string& name) const
    {
        const auto& path = get_work_path(name).string();

        if (utils::io::file_exists(path))
        {
            auto contents = utils::io::read_file(path);

            utils::stream::reader reader(&local_allocator, contents);

            int64_t magic = reader.read<int64_t>();
            if (std::memcmp(&magic, "IW4xModl", 8))
            {
                print_error("Reading model '{}' failed, header is invalid!", name);
                return nullptr;
            }

            int32_t version = reader.read<int32_t>();

            constexpr auto BASE_COMPAT_VERSION = 9;

            if (version < BASE_COMPAT_VERSION)
            {
                print_error("Reading model '{}' failed, minimal supported version is {}, but it was {}!", name, IW4X_MODEL_VERSION, version);
                return nullptr;
            }

            if (version > IW4X_MODEL_VERSION)
            {
                print_error("Reading model '{}' failed, expected version is {}, but it was {}!", name, IW4X_MODEL_VERSION, version);
                return nullptr;
            }

            native::XModel* asset = reader.read_object<native::XModel>();

            if (asset->name)
            {
                asset->name = reader.read_cstring();
            }

            if (asset->boneNames)
            {
                asset->boneNames = local_allocator.allocate_array<uint16_t>(asset->numBones);

                for (uint8_t i = 0; i < asset->numBones; ++i)
                {
                    asset->boneNames[i] = static_cast<std::uint16_t>(assets->write_in_stringtable(reader.read_cstring()));
                }
            }

            if (asset->parentList)
            {
                asset->parentList = reader.read_array_once<uint8_t>(asset->numBones - asset->numRootBones);
            }

            if (asset->quats)
            {
                asset->quats = reinterpret_cast<short(*)[4]>(reader.read_array_once<int16_t>((asset->numBones - asset->numRootBones) * 4));
            }

            if (asset->trans)
            {
                asset->trans = reinterpret_cast<float(*)[3]>(reader.read_array_once<float>((asset->numBones - asset->numRootBones) * 3));
            }

            if (asset->partClassification)
            {
                asset->partClassification = reader.read_array_once<uint8_t>(asset->numBones);
            }

            if (asset->baseMat)
            {
                asset->baseMat = reader.read_array_once<native::DObjAnimMat>(asset->numBones);
            }

            if (asset->materialHandles)
            {
                asset->materialHandles = reader.read_array<native::Material*>(asset->numsurfs);

                for (uint8_t i = 0; i < asset->numsurfs; ++i)
                {
                    if (asset->materialHandles[i])
                    {
                        asset->materialHandles[i] = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, reader.read_string());
                    }
                }
            }

            // Save_XModelLodInfoArray
            {
                for (uint32_t i = 0; i < 4; ++i)
                {
                    if (asset->lodInfo[i].modelSurfs)
                    {
                        asset->lodInfo[i].modelSurfs = reader.read_object<native::XModelSurfs>();
                        read(asset->lodInfo[i].modelSurfs, &reader);

                        asset->lodInfo[i].surfs = asset->lodInfo[i].modelSurfs->surfaces;
                    }
                }
            }

            // Save_XModelCollSurfArray
            if (asset->collSurfs)
            {
                asset->collSurfs = reader.read_array<native::XModelCollSurf_s>(asset->numCollSurfs);

                for (int i = 0; i < asset->numCollSurfs; ++i)
                {
                    native::XModelCollSurf_s* collSurf = &asset->collSurfs[i];

                    if (collSurf->collTris)
                    {
                        collSurf->collTris = reader.read_array<native::XModelCollTri_s>(collSurf->numCollTris);
                    }
                }
            }

            if (asset->boneInfo)
            {
                asset->boneInfo = reader.read_array<native::XBoneInfo>(asset->numBones);
            }

            if (asset->physPreset)
            {
                if (version == 9)
                {
                    // Removed in later versions of XModel
                    asset->physPreset = reader.read_object<native::PhysPreset>();
                }

                const auto& phys_preset_name = reader.read_cstring();

                native::PhysPreset* preset = find<native::PhysPreset>(native::XAssetType::ASSET_TYPE_PHYSPRESET, phys_preset_name);
                assert(preset);

                if (preset)
                {
                    asset->physPreset = preset;
                }
            }

            if (asset->physCollmap)
            {
                native::PhysCollmap* collmap = reader.read_object<native::PhysCollmap>();
                asset->physCollmap = collmap;

                if (collmap->name)
                {
                    collmap->name = reader.read_cstring();
                }

                if (collmap->geoms)
                {
                    collmap->geoms = reader.read_array<native::PhysGeomInfo>(collmap->count);

                    for (uint32_t i = 0; i < collmap->count; ++i)
                    {
                        native::PhysGeomInfo* geom = &collmap->geoms[i];

                        if (geom->brushWrapper)
                        {
                            native::BrushWrapper* brush = reader.read_array_once<native::BrushWrapper>();
                            geom->brushWrapper = brush;
                            {
                                if (brush->brush.sides)
                                {
                                    brush->brush.sides = reader.read_array_once<native::cbrushside_t>(brush->brush.numsides);
                                    for (uint16_t j = 0; j < brush->brush.numsides; ++j)
                                    {
                                        native::cbrushside_t* side = &brush->brush.sides[j];

                                        if (side->plane)
                                        {
                                            side->plane = reader.read_array_once<native::cplane_s>();
                                        }
                                    }
                                }

                                if (brush->brush.baseAdjacentSide)
                                {
                                    brush->brush.baseAdjacentSide = reader.read_array_once<uint8_t>(brush->totalEdgeCount);
                                }
                            }

                            if (brush->planes)
                            {
                                brush->planes = reader.read_array_once<native::cplane_s>(brush->brush.numsides);
                            }
                        }
                    }
                }
            }

            if (!reader.end())
            {
                print_error("Reading model '{}' failed, remaining raw data found!", name);
                return nullptr;
            }

            return asset;
        }

        return nullptr;
    }

    std::filesystem::path ixmodel::get_file_name(const std::string& basename) const
    {
        return std::format("{}.iw4xModel", basename);
    }

    std::filesystem::path ixmodel::get_folder_name() const
    {
        return "xmodel";
    }

} // namespace iw4of::interfaces
