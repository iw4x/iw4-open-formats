#include <std_include.hpp>

#include "iglassworld.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

#include <utils/json.hpp>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#define IW4X_GAMEWORLD_VERSION 1

namespace iw4of::interfaces
{

    bool iglassworld::write_internal(const native::XAssetHeader& header) const
    {
        auto asset = header.gameWorldMp;

        constexpr auto prefix = "maps/mp/";
        constexpr auto suffix = ".d3dbsp";

        std::string basename(header.gameWorldMp->name);
        basename = basename.substr(strlen(prefix), basename.size() - strlen(suffix) - strlen(prefix));

        rapidjson::Document output(rapidjson::kObjectType);
        auto& allocator = output.GetAllocator();
        utils::memory::allocator str_duplicator;

        output.AddMember("version", IW4X_GAMEWORLD_VERSION, allocator);

        output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

        rapidjson::Value glassData(rapidjson::kObjectType);

        rapidjson::Value pieces(rapidjson::kArrayType);
        for (size_t i = 0; i < asset->g_glassData->pieceCount; i++)
        {
            auto iwPiece = asset->g_glassData->glassPieces[i];

            rapidjson::Value piece(rapidjson::kObjectType);

            piece.AddMember("damageTaken", iwPiece.damageTaken, allocator);
            piece.AddMember("collapseTime", iwPiece.collapseTime, allocator);
            piece.AddMember("lastStateChangeTime", iwPiece.lastStateChangeTime, allocator);
            piece.AddMember("impactDir", iwPiece.impactDir, allocator);

            rapidjson::Value impactPos(rapidjson::kArrayType);
            impactPos.PushBack(iwPiece.impactPos[0], allocator);
            impactPos.PushBack(iwPiece.impactPos[1], allocator);

            piece.AddMember("impactPos", impactPos, allocator);

            pieces.PushBack(piece, allocator);
        }

        glassData.AddMember("glassPieces", pieces, allocator);
        glassData.AddMember("damageToWeaken", asset->g_glassData->damageToWeaken, allocator);
        glassData.AddMember("damageToDestroy", asset->g_glassData->damageToDestroy, allocator);

        rapidjson::Value glassNames(rapidjson::kArrayType);

        for (size_t i = 0; i < asset->g_glassData->glassNameCount; i++)
        {
            auto iwName = asset->g_glassData->glassNames[i];

            rapidjson::Value name(rapidjson::kObjectType);

            name.AddMember("nameStr", RAPIDJSON_STR(iwName.nameStr), allocator);
            name.AddMember("name", iwName.name, allocator);

            rapidjson::Value pieceIndices(rapidjson::kArrayType);

            for (size_t j = 0; j < iwName.pieceCount; j++)
            {
                pieceIndices.PushBack(iwName.pieceIndices[j], allocator);
            }

            name.AddMember("piecesIndices", pieceIndices, allocator);

            glassNames.PushBack(name, allocator);
        }

        glassData.AddMember("glassNames", glassNames, allocator);

        output.AddMember("glassData", glassData, allocator);

        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
        output.Accept(writer);

        const auto& dump = buff.GetString();

        return utils::io::write_file(get_work_path(header).string(), dump);
    }

    void* interfaces::iglassworld::read_internal(const std::string& name) const
    {
        const auto& path = get_work_path(name).string();

        if (utils::io::file_exists(path))
        {
            rapidjson::Document gameWorldJson;

            try
            {
                const auto& contents = utils::io::read_file(path);
                gameWorldJson.Parse(contents.data());
            }
            catch (const std::exception& e)
            {
                print_error("Invalid JSON for gameworld {}! {}", name, e.what());
                return nullptr;
            }

            auto* asset = local_allocator.allocate<native::GameWorldMp>();

            if (!gameWorldJson.IsObject())
            {
                print_error("Invalid GameWorldMp json for {}\n", name);
                return nullptr;
            }

            auto version = gameWorldJson["version"].IsNumber() ? gameWorldJson["version"].Get<int32_t>() : 0;
            if (version != IW4X_GAMEWORLD_VERSION)
            {
                print_error("Invalid GameWorld json version for {}, expected {} and got {}\n", name, IW4X_GAMEWORLD_VERSION, version);
                return nullptr;
            }

            if (!gameWorldJson["name"].IsString())
            {
                print_error("Missing gameworld name! on {}\n", name);
                return nullptr;
            }

            asset->name = local_allocator.duplicate_string(gameWorldJson["name"].GetString());
            auto glassData = local_allocator.allocate<native::G_GlassData>();

            if (gameWorldJson["glassData"].IsObject())
            {
                const auto& jsonGlassData = gameWorldJson["glassData"];

                try
                {
                    glassData->damageToDestroy = jsonGlassData["damageToDestroy"].Get<uint16_t>();
                    glassData->damageToWeaken = jsonGlassData["damageToWeaken"].Get<uint16_t>();

                    if (jsonGlassData["glassNames"].IsArray())
                    {
                        const auto& glassNames = jsonGlassData["glassNames"];
                        glassData->glassNameCount = glassNames.Size();
                        glassData->glassNames = local_allocator.allocate_array<native::G_GlassName>(glassData->glassNameCount);

                        for (size_t i = 0; i < glassData->glassNameCount; i++)
                        {
                            const auto& jsonGlassName = glassNames[i];
                            glassData->glassNames[i].nameStr = local_allocator.duplicate_string(jsonGlassName["nameStr"].GetString());

                            glassData->glassNames[i].name = jsonGlassName["name"].Get<uint16_t>();

                            if (jsonGlassName["piecesIndices"].IsArray())
                            {
                                const auto& jsonPiecesIndices = jsonGlassName["piecesIndices"];
                                glassData->glassNames[i].pieceCount = static_cast<uint16_t>(jsonPiecesIndices.Size());
                                glassData->glassNames[i].pieceIndices = local_allocator.allocate_array<uint16_t>(glassData->glassNames[i].pieceCount);
                                for (size_t j = 0; j < glassData->glassNames[i].pieceCount; j++)
                                {
                                    glassData->glassNames[i].pieceIndices[j] = jsonPiecesIndices[j].Get<uint16_t>();
                                }
                            }
                        }
                    }

                    if (jsonGlassData["glassPieces"].IsArray())
                    {
                        const auto& glassPieces = jsonGlassData["glassPieces"];
                        glassData->pieceCount = glassPieces.Size();
                        glassData->glassPieces = local_allocator.allocate_array<native::G_GlassPiece>(glassData->pieceCount);

                        for (size_t i = 0; i < glassData->pieceCount; i++)
                        {
                            glassData->glassPieces[i].collapseTime = glassPieces[i]["collapseTime"].Get<uint16_t>();
                            glassData->glassPieces[i].damageTaken = glassPieces[i]["damageTaken"].Get<uint16_t>();
                            glassData->glassPieces[i].lastStateChangeTime = glassPieces[i]["lastStateChangeTime"].Get<int32_t>();
                            glassData->glassPieces[i].impactDir = glassPieces[i]["impactDir"].Get<char>();

                            const auto& jsonPos = glassPieces[i]["impactPos"];
                            glassData->glassPieces[i].impactPos[0] = jsonPos[0].Get<char>();
                            glassData->glassPieces[i].impactPos[1] = jsonPos[1].Get<char>();
                        }
                    }
                }
                catch (const std::exception& e)
                {
                    print_error("Malformed GameWorldMp json for {} ({})\n", name, e.what());
                    return nullptr;
                }
            }

            asset->g_glassData = glassData;
            return asset;
        }

        return nullptr;
    }

    std::filesystem::path interfaces::iglassworld::get_file_name(const std::string& name) const
    {
        constexpr auto prefix = "maps/mp/";
        constexpr auto suffix = ".d3dbsp";

        std::string basename = name;
        utils::string::replace(basename, prefix, "");
        utils::string::replace(basename, suffix, "");

        return std::format("{}.iw4x.json", basename);
    }
} // namespace iw4of::interfaces