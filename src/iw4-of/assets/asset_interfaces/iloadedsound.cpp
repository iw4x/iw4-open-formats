#include <std_include.hpp>

#include "iloadedsound.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

namespace iw4of::interfaces
{
  void* interfaces::iloadedsound::read_internal(const std::string& name) const
  {
    const auto& path = get_work_path(name).string();

    if (!utils::io::file_exists(path))
    {
      return nullptr;
    }

    native::LoadedSound* sound = local_allocator.allocate<native::LoadedSound>();
    if (!sound)
    {
      print("Error allocating memory for sound structure!\n");
      return nullptr;
    }

    sound->sound.data = nullptr;

    auto contents = utils::io::read_file(path);
    utils::stream::reader reader(&local_allocator, contents);

    uint32_t chunkIDBuffer = reader.read<uint32_t>();
    if (chunkIDBuffer != 0x46464952) // RIFF
    {
      print_error("Reading sound '{}' failed, header is invalid!", name);
      return nullptr;
    }

    uint32_t chunkSize = reader.read<uint32_t>();

    uint32_t format = reader.read<uint32_t>();
    if (format != 0x45564157) // WAVE
    {
      print_error("Reading sound '{}' failed, header is invalid!", name);
      return nullptr;
    }

    while (!sound->sound.data && !reader.end())
    {
      chunkIDBuffer = reader.read<uint32_t>();
      chunkSize = reader.read<uint32_t>();

      switch (chunkIDBuffer)
      {
        case 0x20746D66: // fmt
          if (chunkSize >= 16)
          {
            sound->sound.info.format = reader.read<int16_t>();
            if (sound->sound.info.format != 1 && sound->sound.info.format != 17)
            {
              print_error("Reading sound '{}' failed, invalid format!", name);
              return nullptr;
            }

            sound->sound.info.channels = reader.read<int16_t>();
            sound->sound.info.rate = reader.read<int32_t>();

            // We read samples later, this is byterate we don't need it
            reader.read<int32_t>();

            sound->sound.info.block_size = reader.read<int16_t>();
            sound->sound.info.bits = reader.read<int16_t>();

            // skip any extra parameters
            if (chunkSize > 16)
            {
              reader.seek_relative(chunkSize - 16);
            }
          }
          break;

        case 0x61746164: // data
          sound->sound.info.data_len = chunkSize;
          sound->sound.info.samples = chunkSize / (sound->sound.info.bits / 8);
          sound->sound.data = reader.read_array<char>(chunkSize);
          sound->sound.info.data_ptr = sound->sound.data;
          break;

        default:
          if (chunkSize > 0)
          {
            reader.seek_relative(chunkSize);
          }
          break;
      }
    }

    if (!sound->sound.info.data_ptr)
    {
      print_error("Reading sound '{}' failed, missing sound ptr !", name);
      return nullptr;
    }

    sound->name = local_allocator.duplicate_string(name.data());
    return sound;
  }
  bool iloadedsound::write_internal(const native::XAssetHeader& header) const
  {
    assert(header.loadSnd);

    auto loaded_sound = header.loadSnd;
    if (!loaded_sound->sound.data)
    {
      print_error("Tried to save sound {} which was never loaded before!", loaded_sound->name);
      return false;
    }

    char* sound_data = loaded_sound->sound.data;

    utils::stream buffer;
    // --- RIF HEADER
    // ChunkID
    const char* chunk_id = "RIFF";
    buffer.save_array(chunk_id, 4);

    // ChunkSize
    int32_t sub_chunk_1_size = 16;
    int32_t sub_chunk_2_size = loaded_sound->sound.info.data_len;
    int32_t chunk_size = 4 + (8 + sub_chunk_1_size) + (8 + sub_chunk_2_size);
    buffer.save(chunk_size);

    // Format
    const char* format = "WAVE";
    buffer.save_array(format, 4);

    // --- FMT SUBCHUNK
    // Subchunk1ID
    const char* sub_chunk_1_id = "fmt "; // Mind the space
    buffer.save_array(sub_chunk_1_id, 4);

    // Subchunk1Size
    buffer.save(sub_chunk_1_size);

    // AudioFormat
    short audio_formt = static_cast<int16_t>(loaded_sound->sound.info.format);
    buffer.save_object(audio_formt);

    // NumChannels
    short num_channels = static_cast<int16_t>(loaded_sound->sound.info.channels);
    buffer.save_object(num_channels);

    // SampleRate
    int32_t sampleRate = loaded_sound->sound.info.rate;
    buffer.save(sampleRate);

    auto data_length = loaded_sound->sound.info.data_len;
    assert(data_length / (loaded_sound->sound.info.bits / 8) == loaded_sound->sound.info.samples);

    // ByteRate
    int32_t byteRate = loaded_sound->sound.info.rate * loaded_sound->sound.info.channels * loaded_sound->sound.info.bits / 8;
    buffer.save(byteRate);

    // BlockAlign
    short block_align = static_cast<int16_t>(loaded_sound->sound.info.block_size);
    buffer.save_object(block_align);

    // BitsPerSample
    short bits_per_sample = static_cast<int16_t>(loaded_sound->sound.info.bits);
    buffer.save_object(bits_per_sample);

    // --- DATA SUBCHUNK
    const char* sub_chunk_2_id = "data";
    buffer.save_array(sub_chunk_2_id, 4);

    // Data
    buffer.save(data_length);
    buffer.save(sound_data, data_length);

    utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());
    return true;
  }

  std::filesystem::path interfaces::iloadedsound::get_file_name(const std::string& asset_name) const
  {
    return asset_name;
  }

  std::filesystem::path interfaces::iloadedsound::get_folder_name() const
  {
    return "loaded_sound";
  }
} // namespace iw4of::interfaces