#include <fstream>

#include "runner/framework/io/File.h"

namespace framework::io {

std::vector<uint8_t> readFile(const std::filesystem::path& path)
{
	std::ifstream stream(path, std::ios::in | std::ios::binary);
	const auto size = std::filesystem::file_size(path);

	std::vector<uint8_t> content;
	content.resize(size);

	stream.read(reinterpret_cast<char*>(content.data()), size);

	return content;
}

}
