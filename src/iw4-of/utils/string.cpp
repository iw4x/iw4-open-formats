#include <std_include.hpp>
#include "string.hpp"

namespace iw4of::utils::string
{
  const char* va(const char* fmt, ...)
  {
    static thread_local va_provider<8, 256> provider;

    va_list ap;
    va_start(ap, fmt);

    const char* result = provider.get(fmt, ap);

    va_end(ap);
    return result;
  }

  void replace(std::string& str, const std::string& from, const std::string& to)
  {
    std::size_t n_pos = 0;

    while ((n_pos = str.find(from, n_pos)) != std::string::npos)
    {
      str = str.replace(n_pos, from.length(), to);
      n_pos += to.length();
    }
  }

  std::string to_lower(std::string text)
  {
    std::transform(text.begin(),
                   text.end(),
                   text.begin(),
                   [](const uint8_t input)
                   {
                     return static_cast<char>(std::tolower(input));
                   });

    return text;
  }

  std::string to_upper(std::string text)
  {
    std::transform(text.begin(),
                   text.end(),
                   text.begin(),
                   [](const uint8_t input)
                   {
                     return static_cast<char>(std::toupper(input));
                   });

    return text;
  }

  std::wstring convert(const std::string& str)
  {
    std::wstring result;
    result.reserve(str.size());

    for (const auto& chr : str)
    {
      result.push_back(static_cast<wchar_t>(chr));
    }

    return result;
  }

  std::vector<std::string> split(const char& delimiter, const std::string& data)
  {
    std::vector<std::string> result;
    std::stringstream ss(data);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
      result.push_back(item);
    }

    return result;
  }

  std::string dump_hex(const std::string& data, const std::string& separator)
  {
    std::string result;

    for (uint32_t i = 0; i < data.size(); ++i)
    {
      if (i > 0)
      {
        result.append(separator);
      }

      result.append(va("%02X", data[i] & 0xFF));
    }

    return result;
  }
} // namespace iw4of::utils::string
