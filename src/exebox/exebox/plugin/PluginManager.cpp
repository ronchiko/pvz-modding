#include "exebox/plugin/PluginManager.h"

#include "exebox/ApiInterface.h"
#include "framework/log/Log.h"

namespace exebox::plugin {

namespace {

std::string pluginNameFromPath(const std::filesystem::path& pluginPath)
{
	return pluginPath.filename().string();
}

}

bool PluginManager::isLoaded(const ApiString& name)
{
	return m_Plugins.contains(name);
}

bool PluginManager::loadPlugin(const ApiString& pluginPath)
{
	try {
		auto& registeredPlugin = addPlugin(std::string_view(pluginPath));
		registeredPlugin.install(g_Api.get());
	} catch(const std::exception& ex) {
		framework::log::error() << "Failed to load plugin '" << std::string_view(pluginPath) << "', reason: " << ex.what();
		return false;
	}

	return true;
}

Plugin& PluginManager::addPlugin(const std::filesystem::path& path)
{
	const auto& registeredPlugin = m_Plugins.emplace(pluginNameFromPath(path), Plugin{ path });
	return registeredPlugin.first->second;
}

}
