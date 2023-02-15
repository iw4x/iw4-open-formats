#include <std_include.hpp>

#include "memory.hpp"
#include "stream.hpp"

namespace iw4of::utils
{
  std::string stream::reader::read_string()
  {
    std::string str;

    while (char byte = this->read_byte())
    {
      str.push_back(byte);
    }

    return str;
  }

  const char* stream::reader::read_cstring()
  {
    return this->allocator_->duplicate_string(this->read_string());
  }

  char stream::reader::read_byte()
  {
    if ((this->position_ + 1) <= this->buffer_.size())
    {
      return this->buffer_[this->position_++];
    }

    throw std::runtime_error("Reading past the buffer");
  }

  void* stream::reader::read(size_t size, std::size_t count)
  {
    auto bytes = size * count;

    if ((this->position_ + bytes) <= this->buffer_.size())
    {
      auto* read_buffer = this->allocator_->allocate(bytes);

      std::memcpy(read_buffer, this->buffer_.data() + this->position_, bytes);
      this->position_ += bytes;

      return read_buffer;
    }

    throw std::runtime_error("Reading past the buffer");
  }

  bool stream::reader::end() const
  {
    return (this->buffer_.size() == this->position_);
  }

  void stream::reader::seek(uint32_t position)
  {
    if (this->buffer_.size() >= position)
    {
      this->position_ = position;
    }
  }

  void stream::reader::seek_relative(uint32_t position)
  {
    return this->seek(position + this->position_);
  }

  void stream::reader::map_pointer(void* oldPointer, void* newPointer)
  {
    if (this->has_pointer(oldPointer))
    {
      this->pointer_map[oldPointer] = newPointer;
    }
  }

  bool stream::reader::has_pointer(void* pointer) const
  {
    return this->pointer_map.contains(pointer);
  }

  stream::stream() {}

  stream::stream(size_t size)
      : stream()
  {
    this->buffer.reserve(size);
  }

  stream::~stream()
  {
    this->buffer.clear();
  };

  size_t stream::length()
  {
    return this->buffer.length();
  }

  size_t stream::capacity()
  {
    return this->buffer.capacity();
  }

  char* stream::save(const void* _str, size_t size, size_t count)
  {
    this->buffer.append(static_cast<const char*>(_str), size * count);

    return this->at() - (size * count);
  }

  char* stream::save(int value, size_t count)
  {
    auto ret = this->length();

    for (size_t i = 0; i < count; ++i)
    {
      this->save(&value, 4, 1);
    }

    return this->data() + ret;
  }

  char* stream::save_string(std::string string)
  {
    return this->save_string(string.data() /*, string.size()*/);
  }

  char* stream::save_string(const char* string)
  {
    return this->save_string(string, strlen(string));
  }

  char* stream::save_string(const char* string, size_t len)
  {
    auto ret = this->length();

    if (string)
    {
      this->save(string, len);
    }

    this->save_null();

    return this->data() + ret;
  }

  char* stream::save_text(std::string string)
  {
    return this->save(string.data(), string.length());
  }

  char* stream::save_byte(uint8_t byte, size_t count)
  {
    auto ret = this->length();

    for (size_t i = 0; i < count; ++i)
    {
      this->save(&byte, 1);
    }

    return this->data() + ret;
  }

  char* stream::save_null(size_t count)
  {
    return this->save_byte(0, count);
  }

  char* stream::at()
  {
    return reinterpret_cast<char*>(this->data() + this->length());
  }

  stream::offset stream::_offset()
  {
    return stream::offset(this, this->length());
  }

  char* stream::data()
  {
    return const_cast<char*>(this->buffer.data());
  }

  void stream::to_buffer(std::string& outBuffer)
  {
    outBuffer.clear();
    outBuffer.append(this->data(), this->length());
  }

  std::string stream::to_buffer()
  {
    std::string outBuffer;
    this->to_buffer(outBuffer);
    return outBuffer;
  }
} // namespace iw4of::utils
