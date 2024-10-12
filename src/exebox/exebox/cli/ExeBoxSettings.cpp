#include "framework/cli/ArgumentParser.h"

#include "exebox/cli/ExeBoxSettings.h"

namespace exebox {

namespace {

enum class ExeBoxArgument
{
	MainExecutablePath,
	HookerPluginPath
};

/**
	Creates the argument parser for ExeBox.
 */
framework::cli::ArgumentParser<ExeBoxArgument> createParser()
{
	using namespace framework::cli;

	return ArgumentParser<ExeBoxArgument>()
		.addArgument<std::filesystem::path>(ExeBoxArgument::MainExecutablePath)
		.addArgument<std::filesystem::path>(ExeBoxArgument::HookerPluginPath, { "plugin", "p" }, GroupMode::Many);
}

/**
	Parses the command line arguments as arguments to ExeBox.

	@param arguments - The argument to use.
 */
framework::cli::ArgumentParseResult<ExeBoxArgument> parseArguments(const std::vector<std::string>& arguments)
{
	return createParser().parse(arguments);
}

}

ExeBoxSettings parseSettings(const std::vector<std::string>& arguments)
{
	ExeBoxSettings settings;
	const auto results = parseArguments(arguments);

	settings.mainPath = results.get<std::filesystem::path>(ExeBoxArgument::MainExecutablePath);
	settings.plugins = results.get<std::vector<std::filesystem::path>>(ExeBoxArgument::HookerPluginPath);

	return settings;
}


}