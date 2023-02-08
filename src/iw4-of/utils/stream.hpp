#pragma once

namespace utils
{
	class stream
	{
	private:
		std::string buffer;
		std::unordered_map<void*, size_t> dataPointers;

	public:
		stream();
		stream(size_t size);
		~stream();

		class offset
		{
		public:
			offset(stream* _stream, size_t _offset) : _offset(_offset), stream(_stream) {}

			template <typename T> T* as()
			{
				return reinterpret_cast<T*>(this->stream->data() + _offset);
			}

		private:
			size_t _offset;
			stream* stream;
		};

		size_t length();
		size_t capacity();

		char* save(const void * _str, size_t size, size_t count = 1);
		char* save(int value, size_t count = 1);
		template <typename T> inline char* saveObject(T value)
		{
			return saveArray(&value, 1);
		}
		template <typename T> inline char* saveArray(T* array, size_t count)
		{
			return save(array, sizeof(T), count);
		}
		template <typename T> inline void saveArrayIfNotExisting(T* data, size_t count)
		{

#define POINTER 255
#define FOLLOWING 254

			if (dataPointers.contains(data))
			{
				size_t filePosition = dataPointers.at(data);
				saveByte(POINTER);
				saveObject(filePosition);
			}
			else
			{
				saveByte(FOLLOWING);
				dataPointers.insert_or_assign(reinterpret_cast<void*>(data), length());
				saveArray(data, count);
			}
		}

		char* saveString(std::string string);
		char* saveString(const char* string);
		char* saveString(const char* string, size_t len);
		char* saveByte(unsigned char byte, size_t count = 1);
		char* saveNull(size_t count = 1);
		char* saveMax(size_t count = 1);

		char* saveText(std::string string);

		offset _offset();
		char* data();
		char* at();
		template <typename T> T* Dest()
		{
			return reinterpret_cast<T*>(this->at());
		}

		void toBuffer(std::string& outBuffer);
		std::string toBuffer();


	};
}
