#include <std_include.hpp>

#include <assets\assets.hpp>

#include "igfxworld.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

#define IW4X_GFXMAP_VERSION 1

namespace iw4of::interfaces
{
    bool interfaces::igfxworld::write_internal(const native::XAssetHeader& header) const
    {
        const auto asset = header.gfxWorld;

        utils::stream buffer;
        buffer.save_array("IW4xGfxW", 8);
        buffer.save_object(IW4X_GFXMAP_VERSION);

        buffer.save_object(*asset);

        if (asset->name)
        {
            buffer.save_string(asset->name);
        }

        if (asset->baseName)
        {
            buffer.save_string(asset->baseName);
        }

        if (asset->skies)
        {
            buffer.save_array(asset->skies, asset->skyCount);

            for (int i = 0; i < asset->skyCount; ++i)
            {
                native::GfxSky* sky = &asset->skies[i];

                if (sky->skyStartSurfs)
                {
                    buffer.save_array(sky->skyStartSurfs, sky->skySurfCount);
                }

                if (sky->skyImage)
                {
                    buffer.save_string(sky->skyImage->name);
                    assets->write(native::ASSET_TYPE_IMAGE, sky->skyImage);
                }
            }
        }

        igfxworld::write(asset, &asset->dpvsPlanes, &buffer);

        int32_t cellCount = asset->dpvsPlanes.cellCount;

        if (asset->aabbTreeCounts)
        {
            buffer.save_array(asset->aabbTreeCounts, cellCount);
        }

        if (asset->aabbTrees)
        {
            buffer.save_array(asset->aabbTrees, cellCount);

            for (int i = 0; i < cellCount; ++i)
            {
                native::GfxCellTree* cellTree = &asset->aabbTrees[i];

                if (cellTree->aabbTree)
                {
                    AssertSize(native::GfxAabbTree, 44);
                    buffer.save_array(cellTree->aabbTree, asset->aabbTreeCounts[i].aabbTreeCount);

                    for (int j = 0; j < asset->aabbTreeCounts[i].aabbTreeCount; ++j)
                    {
                        native::GfxAabbTree* aabbTree = &cellTree->aabbTree[j];

                        if (aabbTree->smodelIndexes)
                        {
                            for (uint16_t k = 0; k < aabbTree->smodelIndexCount; ++k)
                            {
                                buffer.save_object(aabbTree->smodelIndexes[k]);
                            }
                        }
                    }
                }
            }
        }

        if (asset->cells)
        {
            AssertSize(native::GfxCell, 40);
            buffer.save_array(asset->cells, cellCount);

            for (int i = 0; i < cellCount; ++i)
            {
                native::GfxCell* cell = &asset->cells[i];

                if (cell->portals)
                {
                    AssertSize(native::GfxPortal, 60);
                    buffer.save_array(cell->portals, cell->portalCount);

                    for (int j = 0; j < cell->portalCount; ++j)
                    {
                        native::GfxPortal* portal = &cell->portals[j];

                        if (portal->vertices)
                        {
                            buffer.save_array(portal->vertices, portal->vertexCount);
                        }
                    }
                }

                if (cell->reflectionProbes)
                {
                    // no align for char
                    buffer.save_array(cell->reflectionProbes, cell->reflectionProbeCount);
                }
            }
        }

        igfxworld::write(&asset->draw, &buffer);
        igfxworld::write(&asset->lightGrid, &buffer);

        if (asset->models)
        {
            AssertSize(native::GfxBrushModel, 60);

            buffer.save_array(asset->models, asset->modelCount);
        }

        if (asset->materialMemory)
        {
            AssertSize(native::MaterialMemory, 8);
            buffer.save_array(asset->materialMemory, asset->materialMemoryCount);

            for (int i = 0; i < asset->materialMemoryCount; ++i)
            {
                native::MaterialMemory* materialMemory = &asset->materialMemory[i];

                if (materialMemory->material)
                {
                    buffer.save_string(materialMemory->material->info.name);
                    assets->write(native::ASSET_TYPE_MATERIAL, materialMemory->material);
                }
            }
        }

        if (asset->sun.spriteMaterial)
        {
            buffer.save_string(asset->sun.spriteMaterial->info.name);
            assets->write(native::ASSET_TYPE_MATERIAL, asset->sun.spriteMaterial);
        }

        if (asset->sun.flareMaterial)
        {
            buffer.save_string(asset->sun.flareMaterial->info.name);
            assets->write(native::ASSET_TYPE_MATERIAL, asset->sun.flareMaterial);
        }

        if (asset->outdoorImage)
        {
            buffer.save_string(asset->outdoorImage->name);
            assets->write(native::ASSET_TYPE_IMAGE, asset->outdoorImage);
        }

        if (asset->shadowGeom)
        {
            AssertSize(native::GfxShadowGeometry, 12);
            buffer.save_array(asset->shadowGeom, asset->primaryLightCount);

            for (uint32_t i = 0; i < asset->primaryLightCount; ++i)
            {
                native::GfxShadowGeometry* shadowGeometry = &asset->shadowGeom[i];

                if (shadowGeometry->sortedSurfIndex)
                {
                    buffer.save_array(shadowGeometry->sortedSurfIndex, shadowGeometry->surfaceCount);
                }

                if (shadowGeometry->smodelIndex)
                {
                    buffer.save_array(shadowGeometry->smodelIndex, shadowGeometry->smodelCount);
                }
            }
        }

        if (asset->lightRegion)
        {
            AssertSize(native::GfxLightRegion, 8);
            buffer.save_array(asset->lightRegion, asset->primaryLightCount);

            for (uint32_t i = 0; i < asset->primaryLightCount; ++i)
            {
                native::GfxLightRegion* lightRegion = &asset->lightRegion[i];

                if (lightRegion->hulls)
                {
                    AssertSize(native::GfxLightRegionHull, 80);
                    buffer.save_array(lightRegion->hulls, lightRegion->hullCount);

                    for (uint32_t j = 0; j < lightRegion->hullCount; ++j)
                    {
                        native::GfxLightRegionHull* lightRegionHull = &lightRegion->hulls[j];

                        if (lightRegionHull->axis)
                        {
                            AssertSize(native::GfxLightRegionAxis, 20);
                            buffer.save_array(lightRegionHull->axis, lightRegionHull->axisCount);
                        }
                    }
                }
            }
        }

        igfxworld::write(asset, &asset->dpvs, &buffer);

        if (asset->heroOnlyLights)
        {
            AssertSize(native::GfxHeroOnlyLight, 56);
            buffer.save_array(asset->heroOnlyLights, asset->heroOnlyLightCount);
        }

        utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());

        return true;
    }

    void igfxworld::write(const native::GfxWorld* world, const native::GfxWorldDpvsStatic* asset, utils::stream* buffer) const
    {
        AssertSize(native::GfxWorldDpvsStatic, 108);

        if (asset->sortedSurfIndex)
        {
            buffer->save_array(asset->sortedSurfIndex, asset->staticSurfaceCount + asset->staticSurfaceCountNoDecal);
        }

        if (asset->smodelInsts)
        {
            AssertSize(native::GfxStaticModelInst, 36);
            buffer->save_array(asset->smodelInsts, asset->smodelCount);
        }

        if (asset->surfaces)
        {
            AssertSize(native::GfxSurface, 24);
            buffer->save_array(asset->surfaces, world->surfaceCount);

            for (uint32_t i = 0; i < world->surfaceCount; ++i)
            {
                native::GfxSurface* surface = &asset->surfaces[i];

                if (surface->material)
                {
                    buffer->save_string(world->dpvs.surfaces[i].material->info.name); // Redundant, but too lazy to implement
                                                                                      // pointer support

                    assets->write(native::ASSET_TYPE_MATERIAL, surface->material);
                }
            }
        }

        if (asset->surfacesBounds)
        {
            AssertSize(native::GfxSurfaceBounds, 24);
            buffer->save_array(asset->surfacesBounds, world->surfaceCount);
        }

        if (asset->smodelDrawInsts)
        {
            AssertSize(native::GfxStaticModelDrawInst, 76);

            buffer->save_array(asset->smodelDrawInsts, asset->smodelCount);

            for (uint32_t i = 0; i < asset->smodelCount; ++i)
            {
                native::GfxStaticModelDrawInst* model = &asset->smodelDrawInsts[i];

                if (model->model)
                {
                    buffer->save_string(model->model->name);
                    assets->write(native::ASSET_TYPE_XMODEL, model->model);
                }
            }
        }
    }

    void igfxworld::write(const native::GfxLightGrid* asset, utils::stream* buffer) const
    {
        AssertSize(native::GfxLightGrid, 56);

        if (asset->rowDataStart)
        {
            buffer->save_array(asset->rowDataStart, (asset->maxs[asset->rowAxis] - asset->mins[asset->rowAxis]) + 1);
        }

        if (asset->rawRowData)
        {
            // no align for char
            buffer->save_array(asset->rawRowData, asset->rawRowDataSize);
        }

        if (asset->entries)
        {
            AssertSize(native::GfxLightGridEntry, 4);
            buffer->save_array(asset->entries, asset->entryCount);
        }

        if (asset->colors)
        {
            AssertSize(native::GfxLightGridColors, 168);
            buffer->save_array(asset->colors, asset->colorCount);
        }
    }

    bool igfxworld::read_dpvs_static(const native::GfxWorld* world, native::GfxWorldDpvsStatic* asset, utils::stream::reader* reader) const
    {
        if (asset->sortedSurfIndex)
        {
            asset->sortedSurfIndex = reader->read_array<uint16_t>(asset->staticSurfaceCount + asset->staticSurfaceCountNoDecal);
        }

        if (asset->smodelInsts)
        {
            asset->smodelInsts = reader->read_array<native::GfxStaticModelInst>(asset->smodelCount);
        }

        if (asset->surfaces)
        {
            asset->surfaces = reader->read_array<native::GfxSurface>(world->surfaceCount);

            for (uint32_t i = 0; i < world->surfaceCount; ++i)
            {
                native::GfxSurface* surface = &asset->surfaces[i];

                if (surface->material)
                {
                    auto materialName = reader->read_string();
                    world->dpvs.surfaces[i].material = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, materialName.data());
                    RETURN_IF_NULL(world->dpvs.surfaces[i].material);
                }
            }
        }

        if (asset->surfacesBounds)
        {
            asset->surfacesBounds = reader->read_array<native::GfxSurfaceBounds>(world->surfaceCount);
        }

        if (asset->smodelDrawInsts)
        {
            asset->smodelDrawInsts = reader->read_array<native::GfxStaticModelDrawInst>(asset->smodelCount);

            for (uint32_t i = 0; i < asset->smodelCount; ++i)
            {
                native::GfxStaticModelDrawInst* model = &asset->smodelDrawInsts[i];

                if (model->model)
                {
                    auto name = reader->read_string();

                    model->model = find<native::XModel>(native::XAssetType::ASSET_TYPE_XMODEL, name.data());

                    RETURN_IF_NULL(model->model);
                }
            }
        }

        return true;
    }

    bool igfxworld::read_gfx_world_draw(native::GfxWorldDraw* asset, utils::stream::reader* reader) const
    {
        if (asset->reflectionProbes)
        {
            asset->reflectionProbes = reader->read_array<native::GfxImage*>(asset->reflectionProbeCount);

            for (uint32_t i = 0; i < asset->reflectionProbeCount; ++i)
            {
                const auto& imgName = reader->read_string();
                asset->reflectionProbes[i] = find<native::GfxImage>(native::XAssetType::ASSET_TYPE_IMAGE, imgName.data());

                assert(asset->reflectionProbes[i]);
            }
        }

        if (asset->reflectionProbeOrigins)
        {
            asset->reflectionProbeOrigins = reader->read_array<native::GfxReflectionProbe>(asset->reflectionProbeCount);
        }

        if (asset->lightmaps)
        {
            asset->lightmaps = reader->read_array<native::GfxLightmapArray>(asset->lightmapCount);

            for (int i = 0; i < asset->lightmapCount; ++i)
            {
                native::GfxLightmapArray* lightmapArray = &asset->lightmaps[i];

                if (lightmapArray->primary)
                {
                    auto primaryLightmapName = reader->read_string();
                    lightmapArray->primary = find<native::GfxImage>(native::XAssetType::ASSET_TYPE_IMAGE, primaryLightmapName.data());
                    RETURN_IF_NULL(lightmapArray->primary);
                }

                if (lightmapArray->secondary)
                {
                    lightmapArray->secondary = find<native::GfxImage>(native::XAssetType::ASSET_TYPE_IMAGE, reader->read_string().data());
                    RETURN_IF_NULL(lightmapArray->secondary);
                }
            }
        }

        if (asset->lightmapOverridePrimary)
        {
            asset->lightmapOverridePrimary = find<native::GfxImage>(native::XAssetType::ASSET_TYPE_IMAGE, reader->read_string().data());
            RETURN_IF_NULL(asset->lightmapOverridePrimary);
        }

        if (asset->lightmapOverrideSecondary)
        {
            asset->lightmapOverrideSecondary = find<native::GfxImage>(native::XAssetType::ASSET_TYPE_IMAGE, reader->read_string().data());
            RETURN_IF_NULL(asset->lightmapOverrideSecondary);
        }

        {
            if (asset->vd.vertices)
            {
                asset->vd.vertices = reader->read_array<native::GfxWorldVertex>(asset->vertexCount);
            }
        }

        {
            if (asset->vld.data)
            {
                // no align for char
                asset->vld.data = reader->read_array<uint8_t>(asset->vertexLayerDataSize);
            }
        }

        if (asset->indices)
        {
            asset->indices = reader->read_array<uint16_t>(asset->indexCount);
        }

        return true;
    }

    void* interfaces::igfxworld::read_internal(const std::string& _name) const
    {
        std::string name = _name;

        utils::string::replace(name, "maps/mp/", "");
        utils::string::replace(name, ".d3dbsp", "");

        const auto& path = get_work_path(_name).string();

        if (utils::io::file_exists(path))
        {
            auto contents = utils::io::read_file(path);
            utils::stream::reader reader(&local_allocator, contents);

            int64_t magic = reader.read<int64_t>();
            if (std::memcmp(&magic, "IW4xGfxW", 8))
            {
                print_error("Reading gfxworld '{}' failed, header is invalid!", name);
            }

            int32_t version = reader.read<int32_t>();
            if (version > IW4X_GFXMAP_VERSION)
            {
                print_error("Reading gfxworld '{}' failed, expected version is {}, but it was {}!", name, IW4X_GFXMAP_VERSION, version);
            }

            native::GfxWorld* asset = reader.read_object<native::GfxWorld>();

            if (asset->name)
            {
                asset->name = reader.read_cstring();
            }

            if (asset->baseName)
            {
                asset->baseName = reader.read_cstring();
            }

            if (asset->skies)
            {
                asset->skies = reader.read_array<native::GfxSky>(asset->skyCount);

                for (int i = 0; i < asset->skyCount; ++i)
                {
                    native::GfxSky* sky = &asset->skies[i];

                    if (sky->skyStartSurfs)
                    {
                        sky->skyStartSurfs = reader.read_array<int32_t>(sky->skySurfCount);
                    }

                    if (sky->skyImage)
                    {
                        sky->skyImage = find<native::GfxImage>(native::XAssetType::ASSET_TYPE_IMAGE, reader.read_string().data());
                    }
                }
            }

            // GfxWorldDpvsPlanes
            {
                if (asset->dpvsPlanes.planes)
                {
                    asset->dpvsPlanes.planes = reader.read_array<native::cplane_s>(asset->planeCount);

                    auto clip = find<native::clipMap_t>(native::XAssetType::ASSET_TYPE_CLIPMAP_MP, asset->name);

                    if (clip)
                    {
                        assert(clip->planeCount == asset->planeCount);
                        for (size_t i = 0; i < clip->planeCount; i++)
                        {
                            assert(0 == memcmp(&clip->planes[i], &asset->dpvsPlanes.planes[i], sizeof(native::cplane_s) - sizeof(native::cplane_s::pad)));
                        }

                        asset->dpvsPlanes.planes = clip->planes;
                    }
                    else
                    {
                        print_error(
                            "GfxWorld dpvs planes not mapped. This shouldn't happen. Make sure to load the ClipMap first! They are co-dependant\n");
                    }
                }

                if (asset->dpvsPlanes.nodes)
                {
                    asset->dpvsPlanes.nodes = reader.read_array<uint16_t>(asset->nodeCount);
                }
            }

            int32_t cell_count = asset->dpvsPlanes.cellCount;

            if (asset->aabbTreeCounts)
            {
                asset->aabbTreeCounts = reader.read_array<native::GfxCellTreeCount>(cell_count);
            }

            if (asset->aabbTrees)
            {
                asset->aabbTrees = reader.read_array<native::GfxCellTree>(cell_count);

                for (int i = 0; i < cell_count; ++i)
                {
                    native::GfxCellTree* cell_tree = &asset->aabbTrees[i];

                    if (cell_tree->aabbTree)
                    {
                        cell_tree->aabbTree = reader.read_array<native::GfxAabbTree>(asset->aabbTreeCounts[i].aabbTreeCount);

                        for (int j = 0; j < asset->aabbTreeCounts[i].aabbTreeCount; ++j)
                        {
                            native::GfxAabbTree* aabbTree = &cell_tree->aabbTree[j];

                            if (aabbTree->smodelIndexes)
                            {
                                uint16_t* oldPointer = aabbTree->smodelIndexes;
                                if (local_allocator.is_pointer_mapped(oldPointer))
                                {
                                    // We still have to read it
                                    reader.read_array<uint16_t>(aabbTree->smodelIndexCount);

                                    aabbTree->smodelIndexes = local_allocator.get_pointer<uint16_t>(oldPointer);
                                }
                                else
                                {
                                    aabbTree->smodelIndexes = reader.read_array<uint16_t>(aabbTree->smodelIndexCount);

                                    for (uint16_t k = 0; k < aabbTree->smodelIndexCount; ++k)
                                    {
                                        local_allocator.map_pointer(&oldPointer[k], &aabbTree->smodelIndexes[k]);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (asset->cells)
            {
                asset->cells = reader.read_array<native::GfxCell>(cell_count);

                for (int i = 0; i < cell_count; ++i)
                {
                    native::GfxCell* cell = &asset->cells[i];

                    if (cell->portals)
                    {
                        cell->portals = reader.read_array<native::GfxPortal>(cell->portalCount);

                        for (int j = 0; j < cell->portalCount; ++j)
                        {
                            native::GfxPortal* portal = &cell->portals[j];

                            if (portal->vertices)
                            {
                                portal->vertices = reader.read_array<native::vec3_t>(portal->vertexCount);
                            }
                        }
                    }

                    if (cell->reflectionProbes)
                    {
                        cell->reflectionProbes = reader.read_array<uint8_t>(cell->reflectionProbeCount);
                    }
                }
            }

            if (!read_gfx_world_draw(&asset->draw, &reader))
            {
                print_error("Could not read world draw for {}", name);
                return nullptr;
            }

            // GfxLightGrid
            {
                if (asset->lightGrid.rowDataStart)
                {
                    asset->lightGrid.rowDataStart = reader.read_array<uint16_t>(
                        (asset->lightGrid.maxs[asset->lightGrid.rowAxis] - asset->lightGrid.mins[asset->lightGrid.rowAxis]) + 1);
                }

                if (asset->lightGrid.rawRowData)
                {
                    asset->lightGrid.rawRowData = reader.read_array<uint8_t>(asset->lightGrid.rawRowDataSize);
                }

                if (asset->lightGrid.entries)
                {
                    asset->lightGrid.entries = reader.read_array<native::GfxLightGridEntry>(asset->lightGrid.entryCount);
                }

                if (asset->lightGrid.colors)
                {
                    asset->lightGrid.colors = reader.read_array<native::GfxLightGridColors>(asset->lightGrid.colorCount);
                }
            }

            if (asset->models)
            {
                asset->models = reader.read_array<native::GfxBrushModel>(asset->modelCount);
            }

            if (asset->materialMemory)
            {
                asset->materialMemory = reader.read_array<native::MaterialMemory>(asset->materialMemoryCount);

                for (int i = 0; i < asset->materialMemoryCount; ++i)
                {
                    native::MaterialMemory* material_memory = &asset->materialMemory[i];

                    if (material_memory->material)
                    {
                        auto materialName = reader.read_string();
                        material_memory->material = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, materialName.data());
                        assert(material_memory->material);
                    }
                }
            }

            if (asset->sun.spriteMaterial)
            {
                auto material_name = reader.read_string();
                asset->sun.spriteMaterial = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, material_name.data());
                assert(asset->sun.spriteMaterial);
            }

            if (asset->sun.flareMaterial)
            {
                auto material_name = reader.read_string();
                asset->sun.flareMaterial = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, material_name.data());
                assert(asset->sun.flareMaterial);
            }

            if (asset->outdoorImage)
            {
                auto material_name = reader.read_string();
                asset->outdoorImage = find<native::GfxImage>(native::XAssetType::ASSET_TYPE_IMAGE, material_name.data());
                assert(asset->outdoorImage);
            }

            if (asset->primaryLightCount > 0)
            {
                asset->primaryLightEntityShadowVis = reinterpret_cast<uint32_t*>(-1);
            }

            // Not sure why this is here - todo investigate
            if (asset->dpvsDyn.dynEntClientCount[0] > 0)
            {
                asset->sceneDynModel = reinterpret_cast<native::GfxSceneDynModel*>(-1);
                asset->primaryLightDynEntShadowVis[0] = reinterpret_cast<uint32_t*>(-1);
                asset->nonSunPrimaryLightForModelDynEnt = reinterpret_cast<uint8_t*>(-1);
            }

            if (asset->dpvsDyn.dynEntClientCount[1] > 0)
            {
                asset->sceneDynBrush = reinterpret_cast<native::GfxSceneDynBrush*>(-1);
                asset->primaryLightDynEntShadowVis[1] = reinterpret_cast<uint32_t*>(-1);
            }
            //

            if (asset->shadowGeom)
            {
                asset->shadowGeom = reader.read_array<native::GfxShadowGeometry>(asset->primaryLightCount);

                for (uint32_t i = 0; i < asset->primaryLightCount; ++i)
                {
                    native::GfxShadowGeometry* shadow_geometry = &asset->shadowGeom[i];

                    if (shadow_geometry->sortedSurfIndex)
                    {
                        shadow_geometry->sortedSurfIndex = reader.read_array<uint16_t>(shadow_geometry->surfaceCount);
                    }

                    if (shadow_geometry->smodelIndex)
                    {
                        shadow_geometry->smodelIndex = reader.read_array<uint16_t>(shadow_geometry->smodelCount);
                    }
                }
            }

            if (asset->lightRegion)
            {
                asset->lightRegion = reader.read_array<native::GfxLightRegion>(asset->primaryLightCount);

                for (uint32_t i = 0; i < asset->primaryLightCount; ++i)
                {
                    native::GfxLightRegion* light_region = &asset->lightRegion[i];

                    if (light_region->hulls)
                    {
                        light_region->hulls = reader.read_array<native::GfxLightRegionHull>(light_region->hullCount);

                        for (uint32_t j = 0; j < light_region->hullCount; ++j)
                        {
                            native::GfxLightRegionHull* lightRegionHull = &light_region->hulls[j];

                            if (lightRegionHull->axis)
                            {
                                lightRegionHull->axis = reader.read_array<native::GfxLightRegionAxis>(lightRegionHull->axisCount);
                            }
                        }
                    }
                }
            }

            if (!this->read_dpvs_static(asset, &asset->dpvs, &reader))
            {
                print_error("Could not read DPVS static for {}", name);
                return nullptr;
            }

            if (asset->heroOnlyLights)
            {
                asset->heroOnlyLights = reader.read_array<native::GfxHeroOnlyLight>(asset->heroOnlyLightCount);
            }

            return asset;
        }

        return nullptr;
    }

    std::filesystem::path interfaces::igfxworld::get_file_name(const std::string& name) const
    {
        constexpr auto prefix = "maps/mp/";
        constexpr auto suffix = ".d3dbsp";

        std::string basename = name;
        utils::string::replace(basename, prefix, "");
        utils::string::replace(basename, suffix, "");

        return std::format("{}.iw4xGfxWorld", basename);
    }

    std::filesystem::path interfaces::igfxworld::get_folder_name() const
    {
        return "gfxworld";
    }

    void igfxworld::write(const native::GfxWorldDraw* asset, utils::stream* buffer) const
    {
        AssertSize(native::GfxWorldDraw, 72);

        if (asset->reflectionProbes)
        {
            buffer->save_array(asset->reflectionProbes, asset->reflectionProbeCount);

            for (uint32_t i = 0; i < asset->reflectionProbeCount; ++i)
            {
                if (asset->reflectionProbes[i])
                {
                    buffer->save_string(asset->reflectionProbes[i]->name);
                    assets->write(native::ASSET_TYPE_IMAGE, asset->reflectionProbes[i]);
                }
            }
        }

        if (asset->reflectionProbeOrigins)
        {
            AssertSize(native::GfxReflectionProbe, 12);
            buffer->save_array(asset->reflectionProbeOrigins, asset->reflectionProbeCount);
        }

        if (asset->lightmaps)
        {
            AssertSize(native::GfxLightmapArray, 8);
            buffer->save_array(asset->lightmaps, asset->lightmapCount);

            for (int i = 0; i < asset->lightmapCount; ++i)
            {
                native::GfxLightmapArray* lightmapArray = &asset->lightmaps[i];

                if (lightmapArray->primary)
                {
                    buffer->save_string(lightmapArray->primary->name);
                    assets->write(native::ASSET_TYPE_IMAGE, lightmapArray->primary);
                }

                if (lightmapArray->secondary)
                {
                    buffer->save_string(lightmapArray->secondary->name);
                    assets->write(native::ASSET_TYPE_IMAGE, lightmapArray->secondary);
                }
            }
        }

        if (asset->lightmapOverridePrimary)
        {
            buffer->save_string(asset->lightmapOverridePrimary->name);
            assets->write(native::ASSET_TYPE_IMAGE, asset->lightmapOverridePrimary);
        }

        if (asset->lightmapOverrideSecondary)
        {
            buffer->save_string(asset->lightmapOverrideSecondary->name);
            assets->write(native::ASSET_TYPE_IMAGE, asset->lightmapOverrideSecondary);
        }

        // saveGfxWorldVertexData
        {
            if (asset->vd.vertices)
            {
                buffer->save_array(asset->vd.vertices, asset->vertexCount);
            }
        }

        // saveGfxWorldVertexLayerData
        {
            if (asset->vld.data)
            {
                // no align for char
                buffer->save_array(asset->vld.data, asset->vertexLayerDataSize);
            }
        }

        if (asset->indices)
        {
            buffer->save_array(asset->indices, asset->indexCount);
        }
    }

    void igfxworld::write(const native::GfxWorld* world, const native::GfxWorldDpvsPlanes* asset, utils::stream* buffer) const
    {
        AssertSize(native::GfxWorldDpvsPlanes, 16);

        if (asset->planes)
        {
            for (size_t i = 0; i < world->planeCount; ++i)
            {
                buffer->save_object(asset->planes[i]);
            }
        }

        if (asset->nodes)
        {
            buffer->save_array(asset->nodes, world->nodeCount);
        }
    }
} // namespace iw4of::interfaces