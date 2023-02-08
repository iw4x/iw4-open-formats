#include <std_include.hpp>
#include "stream.hpp"

namespace utils
{
	stream::stream()
	{

	}

	stream::stream(size_t size) : stream()
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

	char* stream::save(const void * _str, size_t size, size_t count)
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

	char* stream::saveString(std::string string)
	{
		return this->saveString(string.data()/*, string.size()*/);
	}

	char* stream::saveString(const char* string)
	{
		return this->saveString(string, strlen(string));
	}

	char* stream::saveString(const char* string, size_t len)
	{
		auto ret = this->length();

		if (string)
		{
			this->save(string, len);
		}

		this->saveNull();

		return this->data() + ret;
	}

	char* stream::saveText(std::string string)
	{
		return this->save(string.data(), string.length());
	}

	char* stream::saveByte(unsigned char byte, size_t count)
	{
		auto ret = this->length();

		for (size_t i = 0; i < count; ++i)
		{
			this->save(&byte, 1);
		}

		return this->data() + ret;
	}

	char* stream::saveNull(size_t count)
	{
		return this->saveByte(0, count);
	}

	char* stream::saveMax(size_t count)
	{
		return this->saveByte(static_cast<unsigned char>(-1), count);
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

	void stream::toBuffer(std::string& outBuffer)
	{
		outBuffer.clear();
		outBuffer.append(this->data(), this->length());
	}

	std::string stream::toBuffer()
	{
		std::string outBuffer;
		this->toBuffer(outBuffer);
		return outBuffer;
	}
}
