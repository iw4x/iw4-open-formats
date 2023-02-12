#pragma once

namespace iw4of::utils::io
{
	bool file_exists(const std::string& file);
	bool write_file(const std::string& file, const std::string& data, bool append = false);
	bool read_file(const std::string& file, std::string* data);
	std::string read_file(const std::string& file);
	size_t file_size(const std::string& file);
	bool create_directory(const std::string& directory);
	bool directory_exists(const std::string& directory);
	bool directory_is_empty(const std::string& directory);
	std::vector<std::string> list_files(const std::string& directory);
}
