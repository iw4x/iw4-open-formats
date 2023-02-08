#pragma once

namespace utils::cryptography
{
	class ecc final
	{
	public:
		class key final
		{
		public:
			key();
			~key();

			bool is_valid() const;

			ecc_key* get();

			std::string get_public_key() const;

			void set(const std::string& pub_key_buffer);

			void deserialize(const std::string& key);

			std::string serialize(int type = PK_PRIVATE) const;

			void free();

			bool operator==(key& key) const;

		private:
			ecc_key key_storage_{};
		};

		static key generate_key(int bits);
		static std::string sign_message(key key, const std::string& message);
		static bool verify_message(key key, const std::string& message, const std::string& signature);
	};

	class rsa final
	{
	public:
		static std::string encrypt(const std::string& data, const std::string& hash, const std::string& key);

	private:
		static void initialize();
	};

	class des3 final
	{
	public:
		static std::string encrypt(const std::string& data, const std::string& iv, const std::string& key);
		static std::string decrypt(const std::string& data, const std::string& iv, const std::string& key);

	private:
		static void initialize();
	};

	class tiger final
	{
	public:
		static std::string compute(const std::string& data, bool hex = false);
		static std::string compute(const uint8_t* data, size_t length, bool hex = false);
	};

	class sha1 final
	{
	public:
		static std::string compute(const std::string& data, bool hex = false);
		static std::string compute(const uint8_t* data, size_t length, bool hex = false);
	};

	class sha256 final
	{
	public:
		static std::string compute(const std::string& data, bool hex = false);
		static std::string compute(const uint8_t* data, size_t length, bool hex = false);
	};

	class sha512 final
	{
	public:
		static std::string compute(const std::string& data, bool hex = false);
		static std::string compute(const uint8_t* data, size_t length, bool hex = false);
	};

	class jenkins_one_at_a_time final
	{
	public:
		static unsigned int compute(const std::string& data);
		static unsigned int compute(const char* key, size_t len);
	};
}
