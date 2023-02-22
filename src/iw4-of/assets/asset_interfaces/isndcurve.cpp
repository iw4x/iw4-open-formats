#include <std_include.hpp>

#include "isndcurve.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>
#include <utils/json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <assets/assets.hpp>

#define IW4X_SND_CURVE 1

namespace iw4of::interfaces
{
    void* interfaces::isndcurve::read_internal(const std::string& name) const
    {
        auto path = get_work_path(name).string();

        if (!utils::io::file_exists(path))
        {
            return nullptr;
        }

        rapidjson::Document snd_curve_json;

        try
        {
            const auto& contents = utils::io::read_file(path);
            snd_curve_json.Parse(contents.data());
        }
        catch (const std::exception& e)
        {
            print_error("Invalid JSON for sndcurve {}! {}", name, e.what());
            return nullptr;
        }

        auto snd_curve = local_allocator.allocate<native::SndCurve>();

        try
        {
            snd_curve->filename = local_allocator.duplicate_string(snd_curve_json["filename"].GetString());
            snd_curve->knotCount = snd_curve_json["knotCount"].Get<uint16_t>();

            for (auto side = 0; side < 2; side++)
            {
                for (auto knot = 0; knot < 16; knot++)
                {
                    snd_curve->knots[knot][side] = snd_curve_json["knots"][knot][side].Get<float>();
                }
            }
        }
        catch (const std::exception& e)
        {
            print_error("Malformed JSON for sndcurve {}! {}", name, e.what());
            return nullptr;
        }

        return snd_curve;
    }

    bool isndcurve::write_internal(const native::XAssetHeader& header) const
    {
        auto asset = header.sndCurve;

        utils::memory::allocator str_duplicator;
        rapidjson::Document output(rapidjson::kObjectType);
        auto& allocator = output.GetAllocator();

        assert(asset->filename);

        output.AddMember("version", IW4X_SND_CURVE, allocator);
        output.AddMember("filename", RAPIDJSON_STR(asset->filename), allocator);
        output.AddMember("knotCount", asset->knotCount, allocator);

        rapidjson::Value knots_array(rapidjson::kArrayType);
        for (auto knot = 0; knot < 16; knot++)
        {
            rapidjson::Value knots_sides_array(rapidjson::kArrayType);
            for (auto side = 0; side < 2; side++)
            {
                knots_sides_array.PushBack(asset->knots[knot][side], allocator);
            }

            knots_array.PushBack(knots_sides_array, allocator);
        }

        output.AddMember("knots", knots_array, allocator);

        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
        writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);

        output.Accept(writer);

        utils::io::write_file(get_work_path(header).string(), buff.GetString());
        return true;
    }

    std::filesystem::path interfaces::isndcurve::get_file_name(const std::string& asset_name) const
    {
        return std::format("{}.iw4x.json", asset_name);
    }

    std::filesystem::path interfaces::isndcurve::get_folder_name() const
    {
        return "sndcurve";
    }
} // namespace iw4of::interfaces