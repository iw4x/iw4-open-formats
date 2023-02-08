#pragma once

#define CHUNK 16384

namespace utils::compression
{
	class zlib final
	{
	public:
		static std::string compress(const std::string& data);
		static std::string decompress(const std::string& data);
	};

	class zstd final
	{
	public:
		static std::string compress(const std::string& data);
		static std::string decompress(const std::string& data);
	};
};
