#include <std_include.hpp>
#include "json.hpp"
#include <bitset>

namespace iw4of::utils::json
{
    iw4of::native::Bounds read_bounds(const rapidjson::Value& value)
    {
        iw4of::native::Bounds bounds{};
        copy_array(bounds.midPoint, value["midPoint"]);
        copy_array(bounds.halfSize, value["halfSize"]);

        return bounds;
    }

    unsigned long read_flags(const std::string binaryFlags, std::size_t size)
    {
        std::bitset<64> input;
        const auto binarySize = size * 8;

        if (binaryFlags.size() > binarySize)
        {
            assert(false);
            throw std::runtime_error(
                std::format("Flag {} has the wrong size compared to what was expected (expected {} bits)\n", binaryFlags, binarySize));
            return 0;
        }

        auto i = binarySize - 1;
        for (char bit : binaryFlags)
        {
            if (i < 0)
            {
                assert(false);
                throw std::runtime_error(
                    std::format("Flag {} might not be properly translated, it seems to contain an error (invalid length)\n", binaryFlags));
                break;
            }

            auto isOne = bit == '1';
            input.set(i--, isOne);
        }

        return input.to_ulong();
    }

    rapidjson::Value to_json(const native::FxFloatRange& floatRange, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
    {
        rapidjson::Value elemTable(rapidjson::kObjectType);
        elemTable.AddMember("base", floatRange.base, allocator);
        elemTable.AddMember("amplitude", floatRange.amplitude, allocator);
        return elemTable;
    }

    rapidjson::Value to_json(const native::FxIntRange& intRange, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
    {
        rapidjson::Value elemTable(rapidjson::kObjectType);
        elemTable.AddMember("base", intRange.base, allocator);
        elemTable.AddMember("amplitude", intRange.amplitude, allocator);
        return elemTable;
    }

    rapidjson::Value to_json(const native::FxElemVec3Range& vec3Range, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
    {
        rapidjson::Value elemTable(rapidjson::kObjectType);

        rapidjson::Value baseTable(rapidjson::kArrayType);
        rapidjson::Value amplitudeTable(rapidjson::kArrayType);

        for (size_t i = 0; i < 3; i++)
        {
            baseTable.PushBack(vec3Range.base[i], allocator);
            amplitudeTable.PushBack(vec3Range.amplitude[i], allocator);
        }

        elemTable.AddMember("base", baseTable, allocator);
        elemTable.AddMember("amplitude", amplitudeTable, allocator);

        return elemTable;
    }

	rapidjson::Value to_json(const native::cplane_s* planes, size_t count, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator, std::function<void(const native::cplane_s*, size_t)> const& callback_when_adding_plane)
    {
        rapidjson::Value json_planes(rapidjson::kArrayType);
        for (size_t i = 0; i < count; i++)
        {
            rapidjson::Value json_plane(rapidjson::kObjectType);
            auto plane = &planes[i];

            json_plane.AddMember("normal", utils::json::make_json_array(plane->normal, ARRAYSIZE(plane->normal), allocator), allocator);

            json_plane.AddMember("dist", plane->dist, allocator);
            json_plane.AddMember("type", plane->type, allocator);

            json_planes.PushBack(json_plane, allocator);

			if (callback_when_adding_plane)
			{
                callback_when_adding_plane(plane, i);
			}
        }

        return json_planes;
	}

    rapidjson::Value to_json(const native::Bounds& bounds, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
    {
        rapidjson::Value bounds_json(rapidjson::kObjectType);

        bounds_json.AddMember("midPoint", make_json_array(bounds.midPoint, 3, allocator), allocator);
        bounds_json.AddMember("halfSize", make_json_array(bounds.halfSize, 3, allocator), allocator);

        return bounds_json;
    }

    rapidjson::Value to_json(const native::FxElemVisualState& elemVisualState, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
    {
        rapidjson::Value output(rapidjson::kObjectType);

        output.AddMember("color", make_json_array(elemVisualState.color, 4, allocator), allocator);
        output.AddMember("rotationDelta", elemVisualState.rotationDelta, allocator);
        output.AddMember("rotationTotal", elemVisualState.rotationTotal, allocator);
        output.AddMember("size", make_json_array(elemVisualState.size, 2, allocator), allocator);
        output.AddMember("scale", elemVisualState.scale, allocator);

        return output;
    }

    native::FxFloatRange float_range_from_json(const rapidjson::Value& value)
    {
        native::FxFloatRange range{};

        range.base = value["base"].GetFloat();
        range.amplitude = value["amplitude"].GetFloat();

        return range;
    }

    native::FxIntRange int_range_from_json(const rapidjson::Value& value)
    {
        native::FxIntRange range{};

        range.base = value["base"].GetInt();
        range.amplitude = value["amplitude"].GetInt();

        return range;
    }

    native::FxElemVec3Range vec3_range_from_json(const rapidjson::Value& value)
    {
        native::FxElemVec3Range range{};

        for (size_t i = 0; i < 3; i++)
        {
            range.base[i] = value["base"][i].GetFloat();
            range.amplitude[i] = value["amplitude"][i].GetFloat();
        }

        return range;
    }

    native::FxElemVisualState visual_state_from_json(const rapidjson::Value& value)
    {
        native::FxElemVisualState state{};

		copy_array(state.color, value["color"], 4);
        state.rotationDelta = value["rotationDelta"].GetFloat();
        state.rotationTotal = value["rotationTotal"].GetFloat();

        copy_array(state.size, value["size"], 2);

        state.scale = value["scale"].GetFloat();

		return state;
    }

} // namespace iw4of::utils::json
