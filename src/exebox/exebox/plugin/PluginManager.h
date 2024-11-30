#pragma once

#include <unordered_map>

#include "exebox/ApiInterface.h"
#include "exebox/api/plugin/IPluginManager.h"
#include "exebox/plugin/Plugin.h"


namespace exebox::plugin {

namespace detail {

enum class PluginState
{
	Ready,	   // No export was called.
	Installed, // Install was called.
	Removed,   // Uninstall was called.

	InstallFailed,	 // The call to install failed.
	UninstallFailed, // The call to uninstall failed.
};

struct PluginInformation
{
	PluginState state;
	Plugin plugin;
};

}

class PluginManager final : public IPluginManager
{
public:
	/**
		Checks if a plugin is loaded.

		@param name - The name of the plugin.
	 */
	bool isLoaded(const ApiString& name) override;

	/**
		Loads a plugin using its path.
	 */
	bool loadPlugin(const ApiString& pluginPath) override;

	/**
		Adds a plugin from a path.
	 */
	Plugin& addPlugin(const std::filesystem::path& path);

private:
	std::unordered_map<std::string, Plugin> m_Plugins;
};

/**
	Returns the current plugin manager.
 */
inline PluginManager& manager()
{
	return dynamic_cast<PluginManager&>(g_Api->pluginManager());
}

}
