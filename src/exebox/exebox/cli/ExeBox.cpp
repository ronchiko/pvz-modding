#include "framework/log/Log.h"
#include "framework/log/LogPublisher.h"

#include "exebox/cli/api/ExeBoxApi.h"
#include "exebox/cli/ExeBoxSettings.h"
#include "exebox/cli/log/CliLogger.h"
#include "exebox/module/HaltedExecutable.h"
#include "exebox/plugin/PluginManager.h"

#include "exebox/cli/ExeBox.h"

namespace exebox {

namespace {

using Plugins_t = std::vector<std::pair<std::filesystem::path, plugin::Plugin *>>;

/**
	Adds the loggers to the framework context so exebox can write logs.
 */
void registerLoggers()
{
	static std::unique_ptr<framework::log::ILogger> s_CliLogger = std::make_unique<log::CliLogger>();

	framework::log::publisher::addLogger(*s_CliLogger);
}

Plugins_t addPlugins(const std::vector<std::filesystem::path>& pluginPaths)
{
	Plugins_t plugins;
	plugins.reserve(pluginPaths.size());
	for(const auto& pluginPath : pluginPaths) {
		try {
			plugins.emplace_back(pluginPath, &plugin::manager().addPlugin(pluginPath));
		} catch(const std::exception& ex) {
			framework::log::error() << "Failed to add plugin: " << pluginPath << ", reason: " << ex.what();
		}
	}

	return plugins;
}

void installPlugins(const std::vector<std::filesystem::path>& pluginPaths)
{
	const auto plugins = addPlugins(pluginPaths);
	for(auto& [path, plugin] : plugins) {
		try {
			plugin->install(g_Api.get());
			framework::log::info() << "Successfully installed plugin `" << path << "`";
		} catch(const std::exception& ex) {
			framework::log::error() << "Failed to installs plugin: " << path << ", reason: " << ex.what();
		}
	}
}

}

void main(const std::vector<std::string>& arguments)
{
	registerLoggers();
	api::initializeApi();

	framework::log::info() << "ExeBox is starting";
	const auto settings = parseSettings(arguments);

	try {
		auto runtime = createHaltedRuntime(settings.mainPath);
		installPlugins(settings.plugins);
	} catch(const std::exception& e) {
		framework::log::error() << e.what();
	}
}

}
