#pragma once
#include <filesystem>

namespace exebox {

struct ExeBoxSettings
{
	std::filesystem::path mainPath;
	std::vector<std::filesystem::path> plugins;
};

/**
	Parses the ExeBox settings from the command line arguments.

	@param arguments - The command line arguments.
 */
ExeBoxSettings parseSettings(const std::vector<std::string>& arguments);

}
