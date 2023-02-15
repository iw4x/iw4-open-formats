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
      throw std::runtime_error(std::format(
          "Flag {} has the wrong size compared to what was expected (expected {} bits)\n",
          binaryFlags,
          binarySize));
      return 0;
    }

    auto i = binarySize - 1;
    for (char bit : binaryFlags)
    {
      if (i < 0)
      {
        assert(false);
        throw std::runtime_error(std::format(
            "Flag {} might not be properly translated, it seems to contain an error (invalid length)\n",
            binaryFlags));
        break;
      }

      auto isOne = bit == '1';
      input.set(i--, isOne);
    }

    return input.to_ulong();
  }
} // namespace iw4of::utils::json
