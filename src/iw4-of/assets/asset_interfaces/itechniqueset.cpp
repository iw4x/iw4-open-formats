#include <std_include.hpp>

#include "itechniqueset.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <utils/memory.hpp>
#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/json.hpp>

#include <assets/assets.hpp>

#define IW4X_TECHSET_VERSION 1

namespace iw4of::interfaces
{
  bool itechniqueset::write_internal(const native::XAssetHeader& header) const
  {
    const auto techset = header.techniqueSet;

    rapidjson::Document output(rapidjson::kObjectType);
    auto& allocator = output.GetAllocator();

    output.AddMember("version", IW4X_TECHSET_VERSION, allocator);

    if (techset->name)
    {
      output.AddMember("name", RAPIDJSON_STR(techset->name), allocator);
    }

    if (techset->remappedTechniqueSet)
    {
      output.AddMember("remappedTechniqueSet",
                       RAPIDJSON_STR(techset->remappedTechniqueSet->name),
                       allocator);
    }

    output.AddMember("hasBeenUploaded", techset->hasBeenUploaded, allocator);
    output.AddMember("worldVertFormat", techset->worldVertFormat, allocator);

    // This could be an array but it's practical to have the enum index in front
    // Otherwise on a ~48 keys long array the mapping is not immediatly obvious
    rapidjson::Value technique_map(rapidjson::kObjectType);

    for (size_t i = 0; i < native::TECHNIQUE_COUNT; i++)
    {
      rapidjson::Value value = rapidjson::Value(rapidjson::kNullType);

      if (techset->techniques[i])
      {
        if (write(techset->techniques[i]))
        {
          value = rapidjson::Value(techset->techniques[i]->name, allocator);
        }
        else
        {
          print_error("Could not export technique {}",
                      techset->techniques[i]->name);
        }
      }

      technique_map.AddMember(
          rapidjson::Value(std::to_string(i).c_str(), allocator),
          value,
          allocator);
    }

    output.AddMember("techniques", technique_map, allocator);

    rapidjson::StringBuffer buff;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
    output.Accept(writer);

    utils::io::write_file(get_work_path(header).string(), buff.GetString());
    return true;
  }

  void* interfaces::itechniqueset::read_internal(const std::string& name) const
  {
    const auto& path = get_work_path(name);
    if (!utils::io::file_exists(path.string())) return nullptr;

    rapidjson::Document techset;

    try
    {
      auto contents = utils::io::read_file(path.string());
      techset.Parse(contents.data());
    }
    catch (std::exception& e)
    {
      print_error(
          "Reading techset '{}' failed, file is messed up! {}", name, e.what());
      return nullptr;
    }

    auto version = techset["version"].Get<int32_t>();
    if (version != IW4X_TECHSET_VERSION)
    {
      print_error(
          "Reading techset '{}' failed, expected version is{}, but it was{}!",
          name,
          IW4X_TECHSET_VERSION,
          version);
      return nullptr;
    }

    native::MaterialTechniqueSet* asset =
        local_allocator.allocate<native::MaterialTechniqueSet>();

    if (asset == nullptr)
    {
      print_error("Reading techset '{}' failed, allocation failed!", name);
      return nullptr;
    }

    if (techset["name"].IsString())
    {
      asset->name =
          local_allocator.duplicate_string(techset["name"].Get<std::string>());
    }

    asset->hasBeenUploaded = techset["hasBeenUploaded"].Get<bool>();
    asset->worldVertFormat = techset["worldVertFormat"].Get<char>();

    if (techset.HasMember("remappedTechniqueSet") && techset["remappedTechniqueSet"].IsString())
    {
      auto remapped = techset["remappedTechniqueSet"].Get<std::string>();

      if (remapped != asset->name)
      {
        asset->remappedTechniqueSet = find<native::MaterialTechniqueSet>(
            native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, remapped);
      }
    }

    if (techset["techniques"].IsObject())
    {
      for (int i = 0; i < native::TECHNIQUE_COUNT; i++)
      {
        const auto& key = std::to_string(i);
        const auto& technique = techset["techniques"][key.data()];

        if (technique.IsString())
        {
          this->read_technique(&asset->techniques[i],
                               technique.Get<std::string>());
        }
      }
    }

    return asset;
  }

  void interfaces::itechniqueset::read_technique(
      native::MaterialTechnique** tech, const std::string& name) const
  {
    AssertSize(native::MaterialPass, 20);

    const auto& path = get_technique_work_path(name).string();

    if (!utils::io::file_exists(path))
    {
      *tech = nullptr;

      print_error("Missing technique '{}'", name);
      assert(false);
      return;
    }

    rapidjson::Document technique;

    try
    {
      auto file = utils::io::read_file(path);
      technique.Parse(file.data());
    }
    catch (std::exception& e)
    {
      print_error(
          "Reading techset '{}' failed, file is messed up! {}", name, e.what());
    }

    int32_t version = technique["version"].Get<int32_t>();

    if (version != IW4X_TECHSET_VERSION)
    {
      print_error(
          "Reading technique '{}' failed, expected version is {}, but it was {}!",
          name,
          IW4X_TECHSET_VERSION,
          version);
    }

    uint16_t flags = static_cast<uint16_t>(utils::json::read_flags(
        technique["flags"].Get<std::string>(), sizeof(short)));

    if (technique["passArray"].IsArray())
    {
      const auto& passArray = technique["passArray"];

      native::MaterialTechnique* asset =
          reinterpret_cast<native::MaterialTechnique*>(
              local_allocator.allocate_array<uint8_t>(
                  sizeof(native::MaterialTechnique) +
                  (sizeof(native::MaterialPass) * (passArray.Size() - 1))));

      asset->name = local_allocator.duplicate_string(name);
      asset->flags = flags;
      asset->passCount = static_cast<uint16_t>(passArray.Size());

      native::MaterialPass* passes =
          local_allocator.allocate_array<native::MaterialPass>(
              asset->passCount);
      std::memcpy(asset->passArray,
                  passes,
                  sizeof(native::MaterialPass) * asset->passCount);

      for (uint16_t i = 0; i < asset->passCount; i++)
      {
        native::MaterialPass* pass = &asset->passArray[i];
        const auto& jsonPass = passArray[i];

        if (jsonPass["vertexDeclaration"].IsString())
        {
          auto declName = jsonPass["vertexDeclaration"].Get<std::string>();
          pass->vertexDecl = find<native::MaterialVertexDeclaration>(
              native::XAssetType::ASSET_TYPE_VERTEXDECL, declName);
        }

        if (jsonPass["vertexShader"].IsString())
        {
          auto vsName = jsonPass["vertexShader"].Get<std::string>();
          pass->vertexShader = find<native::MaterialVertexShader>(
              native::XAssetType::ASSET_TYPE_VERTEXSHADER, vsName);
        }

        if (jsonPass["pixelShader"].IsString())
        {
          auto psName = jsonPass["pixelShader"].Get<std::string>();
          pass->pixelShader = find<native::MaterialPixelShader>(
              native::XAssetType::ASSET_TYPE_PIXELSHADER, psName);
        }

        pass->perPrimArgCount = jsonPass["perPrimArgCount"].Get<char>();
        pass->perObjArgCount = jsonPass["perObjArgCount"].Get<char>();
        pass->stableArgCount = jsonPass["stableArgCount"].Get<char>();
        pass->customSamplerFlags = jsonPass["customSamplerFlags"].Get<char>();

        if (jsonPass["arguments"].IsArray())
        {
          const auto& jsonAguments = jsonPass["arguments"];

          pass->args =
              local_allocator.allocate_array<native::MaterialShaderArgument>(
                  jsonAguments.Size());

          for (size_t j = 0; j < jsonAguments.Size(); j++)
          {
            const auto& jsonArgument = jsonAguments[j];
            native::MaterialShaderArgument* argument = &pass->args[j];

            argument->type = jsonArgument["type"].Get<uint16_t>();
            argument->dest = jsonArgument["dest"].Get<uint16_t>();

            if (argument->type == native::MaterialShaderArgumentType::
                                      MTL_ARG_LITERAL_VERTEX_CONST ||
                argument->type == native::MaterialShaderArgumentType::
                                      MTL_ARG_LITERAL_PIXEL_CONST)
            {
              argument->u.literalConst = reinterpret_cast<float(*)[4]>(
                  local_allocator.allocate_array<float>(4));
              utils::json::copy_array(
                  reinterpret_cast<float*>(argument->u.literalConst),
                  jsonArgument["literals"],
                  4);
            }
            else if (argument->type == native::MaterialShaderArgumentType::
                                           MTL_ARG_CODE_VERTEX_CONST ||
                     argument->type == native::MaterialShaderArgumentType::
                                           MTL_ARG_CODE_PIXEL_CONST)
            {
              if (jsonArgument["codeConst"].IsObject())
              {
                const auto& codeConst = jsonArgument["codeConst"];

                argument->u.codeConst.index =
                    codeConst["index"].Get<uint16_t>();
                argument->u.codeConst.firstRow =
                    codeConst["firstRow"].Get<uint8_t>();
                argument->u.codeConst.rowCount =
                    codeConst["rowCount"].Get<uint8_t>();
              }
            }
            else if (argument->type == native::MaterialShaderArgumentType::
                                           MTL_ARG_MATERIAL_PIXEL_SAMPLER ||
                     argument->type == native::MaterialShaderArgumentType::
                                           MTL_ARG_MATERIAL_VERTEX_CONST ||
                     argument->type == native::MaterialShaderArgumentType::
                                           MTL_ARG_MATERIAL_PIXEL_CONST)
            {
              argument->u.nameHash = jsonArgument["nameHash"].Get<uint32_t>();
            }
            else if (argument->type == native::MaterialShaderArgumentType::
                                           MTL_ARG_CODE_PIXEL_SAMPLER)
            {
              argument->u.codeSampler =
                  jsonArgument["codeSampler"].Get<uint32_t>();
            }
          }
        }
      }

      *tech = asset;
    }
  }

  std::filesystem::path interfaces::itechniqueset::get_technique_work_path(
      const std::string& name) const
  {
    return std::format(
        "{}/techniques/{}.iw4x.json", assets->get_work_directory(), name);
  }

  std::filesystem::path interfaces::itechniqueset::get_file_name(
      const std::string& basename) const
  {
    return std::format("{}.iw4x.json", basename);
  }

  bool itechniqueset::write(
      const native::MaterialTechnique* iw4_technique) const
  {
    if (!iw4_technique) return false;

    utils::memory::allocator str_duplicator;
    rapidjson::Document output(rapidjson::kObjectType);
    auto& allocator = output.GetAllocator();

    output.AddMember("version", IW4X_TECHSET_VERSION, allocator);
    output.AddMember(
        "name", rapidjson::Value(iw4_technique->name, allocator), allocator);

    // We complete these later
    rapidjson::Value pass_array(rapidjson::kArrayType);

    for (int i = 0; i < iw4_technique->passCount; i++)
    {
      auto iw4_pass = &iw4_technique->passArray[i];

      rapidjson::Value json_pass(rapidjson::kObjectType);

      if (iw4_pass->vertexDecl)
      {
        json_pass.AddMember(
            "vertexDeclaration",
            rapidjson::Value(iw4_pass->vertexDecl->name, allocator),
            allocator);
      }

      if (iw4_pass->vertexShader)
      {
        json_pass.AddMember(
            "vertexShader",
            rapidjson::Value(iw4_pass->vertexShader->name, allocator),
            allocator);
      }

      if (iw4_pass->pixelShader)
      {
        json_pass.AddMember(
            "pixelShader",
            rapidjson::Value(iw4_pass->pixelShader->name, allocator),
            allocator);
      }

      json_pass.AddMember(
          "perPrimArgCount", iw4_pass->perPrimArgCount, allocator);
      json_pass.AddMember(
          "perObjArgCount", iw4_pass->perObjArgCount, allocator);
      json_pass.AddMember(
          "stableArgCount", iw4_pass->stableArgCount, allocator);
      json_pass.AddMember(
          "customSamplerFlags", iw4_pass->customSamplerFlags, allocator);

      rapidjson::Value arguments_array(rapidjson::kArrayType);

      for (int k = 0; k < iw4_pass->perPrimArgCount + iw4_pass->perObjArgCount +
                              iw4_pass->stableArgCount;
           ++k)
      {
        const native::MaterialShaderArgument* iw4_arg = &iw4_pass->args[k];

        rapidjson::Value arg_json(rapidjson::kObjectType);

        arg_json.AddMember("type", iw4_arg->type, allocator);

#if DEBUG
        arg_json.AddMember(
            "type_debug",
            RAPIDJSON_STR(
                native::Debug_MaterialShaderArgumentTypeName[iw4_arg->type]),
            allocator);
#endif

        arg_json.AddMember(
            "dest", iw4_arg->dest, allocator); // Does not need conversion

        if (iw4_arg->type == native::MaterialShaderArgumentType::
                                 MTL_ARG_LITERAL_VERTEX_CONST ||
            iw4_arg->type ==
                native::MaterialShaderArgumentType::MTL_ARG_LITERAL_PIXEL_CONST)
        {
          rapidjson::Value literalsArray(rapidjson::kArrayType);

          // always four
          for (size_t j = 0; j < 4; j++)
          {
            auto cons = (*iw4_arg->u.literalConst)[j];
            literalsArray.PushBack(cons, allocator);
          }

          arg_json.AddMember("literals", literalsArray, allocator);
        }
        else if (iw4_arg->type == native::MaterialShaderArgumentType::
                                      MTL_ARG_CODE_VERTEX_CONST ||
                 iw4_arg->type == native::MaterialShaderArgumentType::
                                      MTL_ARG_CODE_PIXEL_CONST)
        {
          rapidjson::Value code_const(rapidjson::kObjectType);
#if DEBUG
          assert(iw4_arg->u.codeConst.index <
                 ARRAYSIZE(native::Debug_ShaderCodeConstantsNames));
          code_const.AddMember(
              "index_debug",
              RAPIDJSON_STR(
                  native::Debug_ShaderCodeConstantsNames[iw4_arg->u.codeConst
                                                             .index]),
              allocator);
#endif

          code_const.AddMember("index", iw4_arg->u.codeConst.index, allocator);
          code_const.AddMember(
              "firstRow", iw4_arg->u.codeConst.firstRow, allocator);
          code_const.AddMember(
              "rowCount", iw4_arg->u.codeConst.rowCount, allocator);

          arg_json.AddMember("codeConst", code_const, allocator);
        }
        else if (iw4_arg->type == native::MaterialShaderArgumentType::
                                      MTL_ARG_MATERIAL_PIXEL_SAMPLER ||
                 iw4_arg->type == native::MaterialShaderArgumentType::
                                      MTL_ARG_MATERIAL_VERTEX_CONST ||
                 iw4_arg->type == native::MaterialShaderArgumentType::
                                      MTL_ARG_MATERIAL_PIXEL_CONST)
        {
          arg_json.AddMember("nameHash", iw4_arg->u.nameHash, allocator);
        }
        else if (iw4_arg->type ==
                 native::MaterialShaderArgumentType::MTL_ARG_CODE_PIXEL_SAMPLER)
        {
#if DEBUG
          arg_json.AddMember(
              "codeSampler_debug",
              RAPIDJSON_STR(
                  native::Debug_MaterialTextureSourceNames[iw4_arg->u
                                                               .codeSampler]),
              allocator);
#endif

          arg_json.AddMember("codeSampler", iw4_arg->u.codeSampler, allocator);
        }
        else
        {
          print_error(
              "Unknown arg type {} in {}!", iw4_arg->type, iw4_technique->name);
          return false;
        }

        arguments_array.PushBack(arg_json, allocator);
      }

      json_pass.AddMember("arguments", arguments_array, allocator);

      pass_array.PushBack(json_pass, allocator);
    }

    const auto flags =
        std::format("{:016b}", iw4_technique->flags); // no conversion?
    output.AddMember("flags", RAPIDJSON_STR(flags.c_str()), allocator);

    output.AddMember("passArray", pass_array, allocator);

    rapidjson::StringBuffer buff;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
    output.Accept(writer);

    utils::io::write_file(get_technique_work_path(iw4_technique->name).string(),
                          buff.GetString());

    return true;
  }
} // namespace iw4of::interfaces