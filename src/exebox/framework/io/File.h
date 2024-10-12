#pragma once

#include <filesystem>
#include <vector>

namespace framework::io {

/**
	Fully reads the contents of a file to a buffer.

	@param path - The path to the file.
 */
std::vector<uint8_t> readFile(const std::filesystem::path& path);

}
