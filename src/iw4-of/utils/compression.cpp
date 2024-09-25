#include <std_include.hpp>
#include "memory.hpp"
#include "compression.hpp"

namespace iw4of::utils::compression
{
    std::string zlib::compress(const std::string& data)
    {
        memory::allocator allocator;
        unsigned long length = compressBound(data.size());
        const auto buffer = allocator.allocate_array<char>(length);
        if (compress2(reinterpret_cast<Bytef*>(buffer),
                      &length,
                      reinterpret_cast<Bytef*>(const_cast<char*>(data.data())),
                      data.size(),
                      Z_BEST_COMPRESSION) != Z_OK)
        {
            return {};
        }

        return std::string(buffer, length);
    }

    std::string zlib::decompress(const std::string& data)
    {
        z_stream stream;
        std::memset(&stream, 0, sizeof(stream));
        std::string buffer;

        if (inflateInit(&stream) != Z_OK)
        {
            return {};
        }

        int32_t ret;
        memory::allocator allocator;

        const auto dest = allocator.allocate_array<uint8_t>(CHUNK);
        auto data_ptr = data.data();

        do
        {
            stream.avail_in = std::min(static_cast<size_t>(CHUNK), data.size() - (data_ptr - data.data()));
            stream.next_in = reinterpret_cast<const uint8_t*>(data_ptr);
            data_ptr += stream.avail_in;

            do
            {
                stream.avail_out = CHUNK;
                stream.next_out = dest;

                ret = inflate(&stream, Z_NO_FLUSH);
                if (ret != Z_OK && ret != Z_STREAM_END)
                {
                    inflateEnd(&stream);
                    return {};
                }

                buffer.append(reinterpret_cast<const char*>(dest), CHUNK - stream.avail_out);
            } while (stream.avail_out == 0);
        } while (ret != Z_STREAM_END);

        inflateEnd(&stream);
        return buffer;
    }
} // namespace iw4of::utils::compression
