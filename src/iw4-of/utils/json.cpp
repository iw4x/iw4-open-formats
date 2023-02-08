#include <std_include.hpp>
#include "json.hpp"

namespace utils::json
{
	iw4of::native::Bounds read_bounds(const rapidjson::Value& value)
	{
		iw4of::native::Bounds bounds{};
		copy_array(bounds.midPoint, value["midPoint"]);
		copy_array(bounds.halfSize, value["halfSize"]);

		return bounds;
	}
}
