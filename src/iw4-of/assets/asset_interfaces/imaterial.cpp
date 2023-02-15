#include <std_include.hpp>

#include "imaterial.hpp"

#include "itechniqueset.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>
#include <utils/json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#define IW4X_MAT_VERSION 1

namespace iw4of::interfaces
{
  bool imaterial::write_internal(const native::XAssetHeader& header) const
  {
    auto asset = header.material;

    utils::memory::allocator str_duplicator;
    rapidjson::Document output(rapidjson::kObjectType);
    auto& allocator = output.GetAllocator();

    output.AddMember("version", IW4X_MAT_VERSION, allocator);

    output.AddMember("name", RAPIDJSON_STR(asset->info.name), allocator);

    const auto gameFlagsStr = std::format("{:08b}", asset->info.gameFlags);
    output.AddMember(
        "gameFlags", RAPIDJSON_STR(gameFlagsStr.c_str()), allocator);

    const auto stateFlags =
        std::format("{:08b}", static_cast<char>(asset->stateFlags));
    output.AddMember(
        "stateFlags", RAPIDJSON_STR(stateFlags.c_str()), allocator);

#define SAME_NAME_JSON_MEMBER(x) output.AddMember(#x, asset->info.x, allocator)

    SAME_NAME_JSON_MEMBER(sortKey);

    std::string techsetName;
    if (asset->techniqueSet)
    {
      output.AddMember(
          "techniqueSet", RAPIDJSON_STR(asset->techniqueSet->name), allocator);
    }

    SAME_NAME_JSON_MEMBER(textureAtlasRowCount);
    SAME_NAME_JSON_MEMBER(textureAtlasColumnCount);

    const auto surfaceTypeBits =
        std::format("{:032b}", asset->info.surfaceTypeBits);
    output.AddMember(
        "surfaceTypeBits", RAPIDJSON_STR(surfaceTypeBits.c_str()), allocator);

    rapidjson::Value textureTable(rapidjson::kArrayType);

    if (asset->textureTable)
    {
      for (char i = 0; i < asset->textureCount; ++i)
      {
        auto textureDef = &asset->textureTable[i];
        rapidjson::Value textureJson(rapidjson::kObjectType);

        textureJson.AddMember("nameStart", textureDef->nameStart, allocator);
        textureJson.AddMember("nameEnd", textureDef->nameEnd, allocator);
        textureJson.AddMember("nameHash", textureDef->nameHash, allocator);
        textureJson.AddMember("samplerState",
                              textureDef->samplerState,
                              allocator); // $6961E030A9677F7C86FC6FF9B5901495
        textureJson.AddMember("semantic", textureDef->semantic, allocator);

        if (textureDef->semantic == native::TS_WATER_MAP)
        {
          auto water = textureDef->u.water;

          if (water)
          {
            rapidjson::Value waterJson(rapidjson::kObjectType);

            if (water->image)
            {
              waterJson.AddMember(
                  "image", RAPIDJSON_STR(water->image->name), allocator);
              assets->write<native::GfxImage>(
                  native::XAssetType::ASSET_TYPE_IMAGE, water->image);
            }

            constexpr unsigned long BUFF_SIZE = 0xFFFF; // 65KB

            // Save_water_t
            if (water->H0)
            {
              auto ptr = reinterpret_cast<uint8_t*>(water->H0);
              auto buffer = std::vector<uint8_t>(
                  ptr, ptr + water->M * water->N * sizeof(native::complex_s));

              auto b64 = local_allocator.allocate_array<char>(BUFF_SIZE);
              unsigned long buffLength = BUFF_SIZE;

              [[maybe_unused]] auto result = base64_encode(
                  &buffer.front(), buffer.size(), b64, &buffLength);
              assert(result == CRYPT_OK);

              waterJson.AddMember(
                  "H0",
                  RAPIDJSON_STR(str_duplicator.duplicate_string(b64)),
                  allocator);
            }

            if (water->wTerm)
            {
              auto ptr = reinterpret_cast<uint8_t*>(water->wTerm);
              auto buffer = std::vector<uint8_t>(
                  ptr, ptr + water->M * water->N * sizeof(float));

              auto b64 = local_allocator.allocate_array<char>(BUFF_SIZE);
              unsigned long buffLength = BUFF_SIZE;

              [[maybe_unused]] auto result = base64_encode(
                  &buffer.front(), buffer.size(), b64, &buffLength);
              assert(result == CRYPT_OK);

              waterJson.AddMember(
                  "wTerm",
                  RAPIDJSON_STR(str_duplicator.duplicate_string(b64)),
                  allocator);
            }

#define SAME_NAME_WATER_MEMBER(x) waterJson.AddMember(#x, water->x, allocator)

            SAME_NAME_WATER_MEMBER(M);
            SAME_NAME_WATER_MEMBER(N);
            SAME_NAME_WATER_MEMBER(Lx);
            SAME_NAME_WATER_MEMBER(Lz);
            SAME_NAME_WATER_MEMBER(gravity);
            SAME_NAME_WATER_MEMBER(windvel);
            waterJson.AddMember(
                "winddir",
                utils::json::make_json_array(water->winddir, 2, allocator),
                allocator);

            SAME_NAME_WATER_MEMBER(amplitude);
            waterJson.AddMember(
                "codeConstant",
                utils::json::make_json_array(water->codeConstant, 4, allocator),
                allocator);

            textureJson.AddMember("water", waterJson, allocator);
          }
        }
        else
        {
          if (textureDef->u.image)
          {
            textureJson.AddMember(
                "image", RAPIDJSON_STR(textureDef->u.image->name), allocator);
            assets->write<native::GfxImage>(
                native::XAssetType::ASSET_TYPE_IMAGE, textureDef->u.image);
          }
          else
          {
            // This can't happen! It will crash the game
            assert(false);
            print_error("Null/missing image for material {}",
                        header.material->info.name);
            return false;
          }
        }

        textureTable.PushBack(textureJson, allocator);
      }
    }

    output.AddMember("textureTable", textureTable, allocator);
    rapidjson::Value gfxDrawSurface(rapidjson::kObjectType);

#define SAME_NAME_GFXDRAWSURF_MEMBER(x) \
  gfxDrawSurface.AddMember(#x, asset->info.drawSurf.fields.##x##, allocator)

    SAME_NAME_GFXDRAWSURF_MEMBER(objectId);
    SAME_NAME_GFXDRAWSURF_MEMBER(reflectionProbeIndex);
    SAME_NAME_GFXDRAWSURF_MEMBER(hasGfxEntIndex);
    SAME_NAME_GFXDRAWSURF_MEMBER(customIndex);
    SAME_NAME_GFXDRAWSURF_MEMBER(materialSortedIndex);
    SAME_NAME_GFXDRAWSURF_MEMBER(prepass);
    SAME_NAME_GFXDRAWSURF_MEMBER(useHeroLighting);
    SAME_NAME_GFXDRAWSURF_MEMBER(sceneLightIndex);
    SAME_NAME_GFXDRAWSURF_MEMBER(surfType);
    SAME_NAME_GFXDRAWSURF_MEMBER(primarySortKey);
    SAME_NAME_GFXDRAWSURF_MEMBER(unused);

    output.AddMember("gfxDrawSurface", gfxDrawSurface, allocator);

    output.AddMember("hashIndex", 0, allocator);

    rapidjson::Value stateBitsEntry(rapidjson::kArrayType);
    for (size_t i = 0; i < 48; i++)
    {
      stateBitsEntry.PushBack(asset->stateBitsEntry[i], allocator);
    }

    output.AddMember("stateBitsEntry", stateBitsEntry, allocator);
    output.AddMember("cameraRegion", asset->cameraRegion, allocator);

    if (asset->constantTable)
    {
      rapidjson::Value constantTable(rapidjson::kArrayType);

      for (char i = 0; i < asset->constantCount; ++i)
      {
        native::MaterialConstantDef constantDef;
        std::memcpy(&constantDef,
                    &asset->constantTable[i],
                    sizeof native::MaterialConstantDef);

        rapidjson::Value constantDefJson(rapidjson::kObjectType);

        constantDefJson.AddMember("nameHash", constantDef.nameHash, allocator);
        constantDefJson.AddMember(
            "literal",
            utils::json::make_json_array(constantDef.literal, 4, allocator),
            allocator);

        std::string constantDefName = constantDef.name;
        constantDefName = constantDefName.substr(0, 12);

        constantDefJson.AddMember("name",
                                  RAPIDJSON_STR(str_duplicator.duplicate_string(
                                      constantDefName.c_str())),
                                  allocator);

        constantTable.PushBack(constantDefJson, allocator);
      }

      output.AddMember("constantTable", constantTable, allocator);
    }

    if (asset->stateBitsTable)
    {
      output.AddMember(
          "stateBitsTable",
          statebits_to_json_array(
              asset->stateBitsTable, asset->stateBitsCount, allocator),
          allocator);
    }

    rapidjson::StringBuffer buff;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
    output.Accept(writer);

    utils::io::write_file(get_work_path(header).string(), buff.GetString());
    return true;
  }

  void* interfaces::imaterial::read_internal(const std::string& name) const
  {
    auto path = get_work_path(name).string();

    if (!utils::io::file_exists(path)) return nullptr;

    native::Material* asset = local_allocator.allocate<native::Material>();

    rapidjson::Document materialJson;
    try
    {
      auto contents = utils::io::read_file(path);
      materialJson.Parse(contents.data());
    }
    catch (const std::exception& e)
    {
      print_error(
          "Invalid material json for {} (broken json {})\n", name, e.what());
    }

    if (!materialJson.IsObject())
    {
      print_error("Invalid material json for {} (Is it zonebuilder format?)\n",
                  name);
      return nullptr;
    }

    if (materialJson["version"].Get<int32_t>() != IW4X_MAT_VERSION)
    {
      print_error(
          "Invalid material json version for {}, expected {} and got {}\n",
          name,
          IW4X_MAT_VERSION,
          materialJson["version"].Get<std::string>());
      return nullptr;
    }

    try
    {
      asset->info.name = local_allocator.duplicate_string(
          materialJson["name"].Get<std::string>());
      asset->info.gameFlags = static_cast<char>(utils::json::read_flags(
          materialJson["gameFlags"].Get<std::string>(), sizeof(char)));

      asset->info.sortKey = materialJson["sortKey"].Get<char>();
      // * We do techset later * //
      asset->info.textureAtlasRowCount =
          materialJson["textureAtlasRowCount"].Get<uint8_t>();
      asset->info.textureAtlasColumnCount =
          materialJson["textureAtlasColumnCount"].Get<uint8_t>();
      asset->info.surfaceTypeBits =
          static_cast<uint32_t>(utils::json::read_flags(
              materialJson["surfaceTypeBits"].Get<std::string>(), sizeof(int)));
      asset->info.hashIndex = materialJson["hashIndex"].Get<uint16_t>();
      asset->cameraRegion = materialJson["cameraRegion"].Get<char>();
    }
    catch (const std::exception& e)
    {
      print_error(
          "Invalid material json for {} (broken json {})\n", name, e.what());
      return nullptr;
    }

    if (materialJson["gfxDrawSurface"].IsObject())
    {
      asset->info.drawSurf.fields.customIndex =
          materialJson["gfxDrawSurface"]["customIndex"].Get<int64_t>();
      asset->info.drawSurf.fields.hasGfxEntIndex =
          materialJson["gfxDrawSurface"]["hasGfxEntIndex"].Get<int64_t>();
      asset->info.drawSurf.fields.materialSortedIndex =
          materialJson["gfxDrawSurface"]["materialSortedIndex"].Get<int64_t>();
      asset->info.drawSurf.fields.objectId =
          materialJson["gfxDrawSurface"]["objectId"].Get<int64_t>();
      asset->info.drawSurf.fields.prepass =
          materialJson["gfxDrawSurface"]["prepass"].Get<int64_t>();
      asset->info.drawSurf.fields.primarySortKey =
          materialJson["gfxDrawSurface"]["primarySortKey"].Get<int64_t>();
      asset->info.drawSurf.fields.reflectionProbeIndex =
          materialJson["gfxDrawSurface"]["reflectionProbeIndex"].Get<int64_t>();
      asset->info.drawSurf.fields.sceneLightIndex =
          materialJson["gfxDrawSurface"]["sceneLightIndex"].Get<int64_t>();
      asset->info.drawSurf.fields.surfType =
          materialJson["gfxDrawSurface"]["surfType"].Get<int64_t>();
      asset->info.drawSurf.fields.unused =
          materialJson["gfxDrawSurface"]["unused"].Get<int64_t>();
      asset->info.drawSurf.fields.useHeroLighting =
          materialJson["gfxDrawSurface"]["useHeroLighting"].Get<int64_t>();
    }

    asset->stateFlags = static_cast<char>(utils::json::read_flags(
        materialJson["stateFlags"].Get<std::string>(), sizeof(char)));

    if (materialJson["textureTable"].IsArray())
    {
      const auto& textureTable = materialJson["textureTable"];
      asset->textureCount = static_cast<uint8_t>(textureTable.Size());
      asset->textureTable =
          local_allocator.allocate_array<native::MaterialTextureDef>(
              asset->textureCount);

      for (size_t i = 0; i < textureTable.Size(); i++)
      {
        auto& textureJson = textureTable[i];
        if (textureJson.IsObject())
        {
          native::MaterialTextureDef* textureDef = &asset->textureTable[i];
          textureDef->semantic = textureJson["semantic"].Get<uint8_t>();
          textureDef->samplerState = textureJson["samplerState"].Get<char>();
          textureDef->nameStart = textureJson["nameStart"].Get<char>();
          textureDef->nameEnd = textureJson["nameEnd"].Get<char>();
          textureDef->nameHash = textureJson["nameHash"].Get<uint32_t>();

          if (textureDef->semantic == native::TextureSemantic::TS_WATER_MAP)
          {
            native::water_t* water =
                local_allocator.allocate<native::water_t>();

            if (textureJson["water"].IsObject())
            {
              auto& waterJson = textureJson["water"];

              if (waterJson["image"].IsString())
              {
                auto imageName = waterJson["image"].Get<std::string>();

                water->image = find<native::GfxImage>(
                    native::XAssetType::ASSET_TYPE_IMAGE, imageName.data());
              }

              water->amplitude = waterJson["amplitude"].Get<float>();
              water->M = waterJson["M"].Get<int32_t>();
              water->N = waterJson["N"].Get<int32_t>();
              water->Lx = waterJson["Lx"].Get<float>();
              water->Lz = waterJson["Lz"].Get<float>();
              water->gravity = waterJson["gravity"].Get<float>();
              water->windvel = waterJson["windvel"].Get<float>();

              utils::json::copy_array(water->winddir, waterJson["winddir"], 2);
              utils::json::copy_array(
                  water->codeConstant, waterJson["codeConstant"], 4);

              /// H0
              [[maybe_unused]] auto idealSize =
                  water->M * water->N * sizeof(native::complex_s);
              auto h064 = waterJson["H0"].Get<std::string>();
              auto predictedSize = static_cast<unsigned long>(
                  std::ceilf((h064.size() / 4.f) * 3.f));
              assert(predictedSize >= idealSize);

              auto h0 = reinterpret_cast<native::complex_s*>(
                  local_allocator.allocate(predictedSize));

              [[maybe_unused]] auto h0Result =
                  base64_decode(h064.data(),
                                h064.size(),
                                reinterpret_cast<uint8_t*>(h0),
                                &predictedSize);

              assert(h0Result == CRYPT_OK);
              water->H0 = h0;

              /// WTerm
              auto wTerm64 = waterJson["wTerm"].Get<std::string>();
              auto predictedWTermSize = static_cast<unsigned long>(
                  std::ceilf((wTerm64.size() / 4.f) * 3.f));

              auto wTerm = reinterpret_cast<float*>(
                  local_allocator.allocate(predictedWTermSize));

              [[maybe_unused]] auto wTermResult =
                  base64_decode(wTerm64.data(),
                                wTerm64.size(),
                                reinterpret_cast<uint8_t*>(wTerm),
                                &predictedWTermSize);

              assert(wTermResult == CRYPT_OK);
              water->wTerm = wTerm;
            }

            textureDef->u.water = water;
          }
          else
          {
            textureDef->u.image = nullptr;
            if (textureJson["image"].IsString())
            {
              textureDef->u.image = find<native::GfxImage>(
                  native::XAssetType::ASSET_TYPE_IMAGE,
                  textureJson["image"].Get<std::string>());

              assert(textureDef->u.image);
            }
            else
            {
              assert(false);
            }
          }
        }
      }
    }

    // Statebits
    if (materialJson["stateBitsEntry"].IsArray())
    {
      const auto& stateBitsEntry = materialJson["stateBitsEntry"];

      for (size_t i = 0; i < std::min(stateBitsEntry.Size(), 48u); i++)
      {
        asset->stateBitsEntry[i] = stateBitsEntry[i].Get<char>();
      }
    }

    if (materialJson["stateBitsTable"].IsArray())
    {
      const auto& array = materialJson["stateBitsTable"];
      asset->stateBitsCount = static_cast<uint8_t>(array.Size());

      asset->stateBitsTable =
          local_allocator.allocate_array<native::GfxStateBits>(array.Size());

      size_t statebitTableIndex = 0;
      for (size_t i = 0; i < array.Size(); i++)
      {
        const auto& jsonStateBitEntry = array[i];
        auto stateBit = &asset->stateBitsTable[statebitTableIndex++];

        uint32_t loadbits0 = 0;
        uint32_t loadbits1 = 0;

#define READ_INT_LB_FROM_JSON(x) \
  uint32_t x = jsonStateBitEntry[#x].Get<uint32_t>()
#define READ_BOOL_LB_FROM_JSON(x) bool x = jsonStateBitEntry[#x].Get<bool>()

        READ_INT_LB_FROM_JSON(srcBlendRgb);
        READ_INT_LB_FROM_JSON(dstBlendRgb);
        READ_INT_LB_FROM_JSON(blendOpRgb);
        READ_INT_LB_FROM_JSON(srcBlendAlpha);
        READ_INT_LB_FROM_JSON(dstBlendAlpha);
        READ_INT_LB_FROM_JSON(blendOpAlpha);
        READ_INT_LB_FROM_JSON(depthTest);
        READ_INT_LB_FROM_JSON(polygonOffset);

        const auto alphaTest =
            jsonStateBitEntry["alphaTest"].Get<std::string>();
        const auto cullFace = jsonStateBitEntry["cullFace"].Get<std::string>();

        READ_BOOL_LB_FROM_JSON(colorWriteRgb);
        READ_BOOL_LB_FROM_JSON(colorWriteAlpha);
        READ_BOOL_LB_FROM_JSON(polymodeLine);

        READ_BOOL_LB_FROM_JSON(gammaWrite);
        READ_BOOL_LB_FROM_JSON(depthWrite);
        READ_BOOL_LB_FROM_JSON(stencilFrontEnabled);
        READ_BOOL_LB_FROM_JSON(stencilBackEnabled);

        READ_INT_LB_FROM_JSON(stencilFrontPass);
        READ_INT_LB_FROM_JSON(stencilFrontFail);
        READ_INT_LB_FROM_JSON(stencilFrontZFail);
        READ_INT_LB_FROM_JSON(stencilFrontFunc);
        READ_INT_LB_FROM_JSON(stencilBackPass);
        READ_INT_LB_FROM_JSON(stencilBackFail);
        READ_INT_LB_FROM_JSON(stencilBackZFail);
        READ_INT_LB_FROM_JSON(stencilBackFunc);

        loadbits0 |= srcBlendRgb << native::GFXS0_SRCBLEND_RGB_SHIFT;
        loadbits0 |= dstBlendRgb << native::GFXS0_DSTBLEND_RGB_SHIFT;
        loadbits0 |= blendOpRgb << native::GFXS0_BLENDOP_RGB_SHIFT;
        loadbits0 |= srcBlendAlpha << native::GFXS0_SRCBLEND_ALPHA_SHIFT;
        loadbits0 |= dstBlendAlpha << native::GFXS0_DSTBLEND_ALPHA_SHIFT;
        loadbits0 |= blendOpAlpha << native::GFXS0_BLENDOP_ALPHA_SHIFT;

        if (depthTest == -1)
        {
          loadbits1 |= native::GFXS1_DEPTHTEST_DISABLE;
        }
        else
        {
          loadbits1 |= depthTest << native::GFXS1_DEPTHTEST_SHIFT;
        }

        loadbits1 |= polygonOffset << native::GFXS1_POLYGON_OFFSET_SHIFT;

        if (alphaTest == "disable")
        {
          loadbits0 |= native::GFXS0_ATEST_DISABLE;
        }
        else if (alphaTest == ">0")
        {
          loadbits0 |= native::GFXS0_ATEST_GT_0;
        }
        else if (alphaTest == "<128")
        {
          loadbits0 |= native::GFXS0_ATEST_LT_128;
        }
        else if (alphaTest == ">=128")
        {
          loadbits0 |= native::GFXS0_ATEST_GE_128;
        }
        else
        {
          print_error("Invalid alphatest loadbit0 '{}' in material {}\n",
                      alphaTest,
                      name);
          return nullptr;
        }

        if (cullFace == "none")
        {
          loadbits0 |= native::GFXS0_CULL_NONE;
        }
        else if (cullFace == "back")
        {
          loadbits0 |= native::GFXS0_CULL_BACK;
        }
        else if (cullFace == "front")
        {
          loadbits0 |= native::GFXS0_CULL_FRONT;
        }
        else
        {
          print_error("Invalid cullFace loadbit0 '{}' in material {}\n",
                      cullFace,
                      name);
          return nullptr;
        }

        if (gammaWrite)
        {
          loadbits0 |= native::GFXS0_GAMMAWRITE;
        }

        if (colorWriteAlpha)
        {
          loadbits0 |= native::GFXS0_COLORWRITE_ALPHA;
        }

        if (colorWriteRgb)
        {
          loadbits0 |= native::GFXS0_COLORWRITE_RGB;
        }

        if (polymodeLine)
        {
          loadbits0 |= native::GFXS0_POLYMODE_LINE;
        }

        if (depthWrite)
        {
          loadbits1 |= native::GFXS1_DEPTHWRITE;
        }
        if (stencilFrontEnabled)
        {
          loadbits1 |= native::GFXS1_STENCIL_FRONT_ENABLE;
        }
        if (stencilBackEnabled)
        {
          loadbits1 |= native::GFXS1_STENCIL_BACK_ENABLE;
        }

        loadbits1 |= stencilFrontPass << native::GFXS1_STENCIL_FRONT_PASS_SHIFT;
        loadbits1 |= stencilFrontFail << native::GFXS1_STENCIL_FRONT_FAIL_SHIFT;
        loadbits1 |= stencilFrontZFail
                     << native::GFXS1_STENCIL_FRONT_ZFAIL_SHIFT;
        loadbits1 |= stencilFrontFunc << native::GFXS1_STENCIL_FRONT_FUNC_SHIFT;
        loadbits1 |= stencilBackPass << native::GFXS1_STENCIL_BACK_PASS_SHIFT;
        loadbits1 |= stencilBackFail << native::GFXS1_STENCIL_BACK_FAIL_SHIFT;
        loadbits1 |= stencilBackZFail << native::GFXS1_STENCIL_BACK_ZFAIL_SHIFT;
        loadbits1 |= stencilBackFunc << native::GFXS1_STENCIL_BACK_FUNC_SHIFT;

        stateBit->loadBits[0] = loadbits0;
        stateBit->loadBits[1] = loadbits1;
      }

      // Constant table
      if (materialJson["constantTable"].IsArray())
      {
        const auto& constants = materialJson["constantTable"];
        asset->constantCount = static_cast<char>(constants.Size());
        auto table =
            local_allocator.allocate_array<native::MaterialConstantDef>(
                asset->constantCount);

        for (size_t constantIndex = 0; constantIndex < asset->constantCount;
             constantIndex++)
        {
          auto& constant = constants[constantIndex];
          auto entry = &table[constantIndex];

          utils::json::copy_array(entry->literal, constant["literal"], 4);

          auto constantName = constant["name"].Get<std::string>();
          std::copy(constantName.begin(), constantName.end(), entry->name);

          entry->nameHash = constant["nameHash"].Get<uint32_t>();
        }

        asset->constantTable = table;
      }

      if (materialJson["techniqueSet"].IsString())
      {
        const std::string techsetName =
            materialJson["techniqueSet"].Get<std::string>();
        asset->techniqueSet = find<native::MaterialTechniqueSet>(
            native::ASSET_TYPE_TECHNIQUE_SET, techsetName);

        if (asset->techniqueSet == nullptr)
        {
          assert(false);
          print_error("Could not find technique set {} for material {}",
                      techsetName,
                      name);
          return nullptr;
        }
      }

      return asset;
    }

    return nullptr;
  }

  std::filesystem::path interfaces::imaterial::get_file_name(
      const std::string& basename) const
  {
    return std::format("{}.iw4x.json", basename);
  }

  rapidjson::Value imaterial::statebits_to_json_array(
      native::GfxStateBits* stateBits, uint8_t count,
      rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const
  {
    rapidjson::Value arr(rapidjson::kArrayType);

    for (auto index = 0u; index < count; index++)
    {
      const auto& entry = stateBits[index];

      const auto srcBlendRgb =
          (entry.flags.loadbit0 & native::GFXS0_SRCBLEND_RGB_MASK) >>
          native::GFXS0_SRCBLEND_RGB_SHIFT;
      const auto dstBlendRgb =
          (entry.flags.loadbit0 & native::GFXS0_DSTBLEND_RGB_MASK) >>
          native::GFXS0_DSTBLEND_RGB_SHIFT;
      const auto blendOpRgb =
          (entry.flags.loadbit0 & native::GFXS0_BLENDOP_RGB_MASK) >>
          native::GFXS0_BLENDOP_RGB_SHIFT;
      const auto srcBlendAlpha =
          (entry.flags.loadbit0 & native::GFXS0_SRCBLEND_ALPHA_MASK) >>
          native::GFXS0_SRCBLEND_ALPHA_SHIFT;
      const auto dstBlendAlpha =
          (entry.flags.loadbit0 & native::GFXS0_DSTBLEND_ALPHA_MASK) >>
          native::GFXS0_DSTBLEND_ALPHA_SHIFT;
      const auto blendOpAlpha =
          (entry.flags.loadbit0 & native::GFXS0_BLENDOP_ALPHA_MASK) >>
          native::GFXS0_BLENDOP_ALPHA_SHIFT;
      const auto depthTest =
          (entry.flags.loadbit1 & native::GFXS1_DEPTHTEST_DISABLE)
              ? -1
              : (entry.flags.loadbit1 & native::GFXS1_DEPTHTEST_MASK) >>
                    native::GFXS1_DEPTHTEST_SHIFT;
      const auto polygonOffset =
          (entry.flags.loadbit1 & native::GFXS1_POLYGON_OFFSET_MASK) >>
          native::GFXS1_POLYGON_OFFSET_SHIFT;

      const auto* alphaTest = "disable";
      if ((entry.flags.loadbit0 & native::GFXS0_ATEST_MASK) ==
          native::GFXS0_ATEST_GE_128)
      {
        alphaTest = ">=128";
      }
      else if ((entry.flags.loadbit0 & native::GFXS0_ATEST_MASK) ==
               native::GFXS0_ATEST_GT_0)
      {
        alphaTest = ">0";
      }
      else if ((entry.flags.loadbit0 & native::GFXS0_ATEST_MASK) ==
               native::GFXS0_ATEST_LT_128)
      {
        alphaTest = "<128";
      }
      else
      {
        assert(entry.flags.loadbit0 & native::GFXS0_ATEST_DISABLE);
      }

      const auto* cullFace = "none";
      if ((entry.flags.loadbit0 & native::GFXS0_CULL_MASK) ==
          native::GFXS0_CULL_BACK)
      {
        cullFace = "back";
      }
      else if ((entry.flags.loadbit0 & native::GFXS0_CULL_MASK) ==
               native::GFXS0_CULL_FRONT)
      {
        cullFace = "front";
      }
      else
      {
        assert((entry.flags.loadbit0 & native::GFXS0_CULL_MASK) ==
               native::GFXS0_CULL_NONE);
      }

      rapidjson::Value stateBitEntry(rapidjson::kObjectType);

      const auto colorWriteRgb =
          entry.flags.loadbit0 & native::GFXS0_COLORWRITE_RGB ? true : false;
      const auto colorWriteAlpha =
          entry.flags.loadbit0 & native::GFXS0_COLORWRITE_ALPHA ? true : false;
      const auto polymodeLine =
          entry.flags.loadbit0 & native::GFXS0_POLYMODE_LINE ? true : false;
      const auto gammaWrite =
          entry.flags.loadbit0 & native::GFXS0_GAMMAWRITE ? true : false;
      const auto depthWrite =
          (entry.flags.loadbit1 & native::GFXS1_DEPTHWRITE) ? true : false;
      const auto stencilFrontEnabled =
          (entry.flags.loadbit1 & native::GFXS1_STENCIL_FRONT_ENABLE) ? true
                                                                      : false;
      const auto stencilBackEnabled =
          (entry.flags.loadbit1 & native::GFXS1_STENCIL_BACK_ENABLE) ? true
                                                                     : false;
      const auto stencilFrontPass =
          (entry.flags.loadbit1 >> native::GFXS1_STENCIL_FRONT_PASS_SHIFT) &
          native::GFXS_STENCILOP_MASK;
      const auto stencilFrontFail =
          (entry.flags.loadbit1 >> native::GFXS1_STENCIL_FRONT_FAIL_SHIFT) &
          native::GFXS_STENCILOP_MASK;
      const auto stencilFrontZFail =
          (entry.flags.loadbit1 >> native::GFXS1_STENCIL_FRONT_ZFAIL_SHIFT) &
          native::GFXS_STENCILOP_MASK;
      const auto stencilFrontFunc =
          (entry.flags.loadbit1 >> native::GFXS1_STENCIL_FRONT_FUNC_SHIFT) &
          native::GFXS_STENCILOP_MASK;
      const auto stencilBackPass =
          (entry.flags.loadbit1 >> native::GFXS1_STENCIL_BACK_PASS_SHIFT) &
          native::GFXS_STENCILOP_MASK;
      const auto stencilBackFail =
          (entry.flags.loadbit1 >> native::GFXS1_STENCIL_BACK_FAIL_SHIFT) &
          native::GFXS_STENCILOP_MASK;
      const auto stencilBackZFail =
          (entry.flags.loadbit1 >> native::GFXS1_STENCIL_BACK_ZFAIL_SHIFT) &
          native::GFXS_STENCILOP_MASK;
      const auto stencilBackFunc =
          (entry.flags.loadbit1 >> native::GFXS1_STENCIL_BACK_FUNC_SHIFT) &
          native::GFXS_STENCILOP_MASK;

#define ADD_TO_JSON(x) stateBitEntry.AddMember(#x, x, allocator)
#define ADD_TO_JSON_STR(x) \
  stateBitEntry.AddMember(#x, RAPIDJSON_STR(x), allocator)

      ADD_TO_JSON_STR(alphaTest);
      ADD_TO_JSON(blendOpAlpha);
      ADD_TO_JSON(blendOpRgb);
      ADD_TO_JSON(colorWriteAlpha);
      ADD_TO_JSON(colorWriteRgb);
      ADD_TO_JSON_STR(cullFace);
      ADD_TO_JSON(depthTest);
      ADD_TO_JSON(depthWrite);
      ADD_TO_JSON(dstBlendAlpha);
      ADD_TO_JSON(dstBlendRgb);
      ADD_TO_JSON(gammaWrite);
      ADD_TO_JSON(polygonOffset);
      ADD_TO_JSON(polymodeLine);
      ADD_TO_JSON(srcBlendRgb);
      ADD_TO_JSON(srcBlendAlpha);
      ADD_TO_JSON(stencilBackEnabled);
      ADD_TO_JSON(stencilBackFail);
      ADD_TO_JSON(stencilBackFunc);
      ADD_TO_JSON(stencilBackPass);
      ADD_TO_JSON(stencilBackZFail);
      ADD_TO_JSON(stencilFrontEnabled);
      ADD_TO_JSON(stencilFrontFail);
      ADD_TO_JSON(stencilFrontFunc);
      ADD_TO_JSON(stencilFrontPass);
      ADD_TO_JSON(stencilFrontZFail);

#if DEBUG
      stateBitEntry.AddMember("check0", entry.flags.loadbit0, allocator);
      stateBitEntry.AddMember("check1", entry.flags.loadbit1, allocator);
#endif

      arr.PushBack(stateBitEntry, allocator);
    }

    return arr;
  }
} // namespace iw4of::interfaces