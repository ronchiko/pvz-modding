#pragma once

#include "exebox/api/IExeBoxApi.h"
#include "framework/io/HModule.h"

namespace exebox::plugin {

class PluginInstallFailed final : public std::runtime_error
{
public:
	explicit PluginInstallFailed(const std::string& reason)
		: runtime_error("Plugin install failed: " + reason)
	{}
};

class Plugin
{
public:
	/**
		Loads a plugin from a path.
	 */
	explicit Plugin(const std::filesystem::path& path);

	/**
		Installs the plugin.

		@param api - The api instance to the install the plugin with.

		@throws PluginInstallFailed - If the installation failed for some reason.
	 */
	void install(IExeBoxApi *api);

	/**
		Uninstall the plugin.
	 */
	void uninstall(IExeBoxApi *api);

private:
	/**
		Invokes the `install` function of the function, or raises an extension.
	 */
	void installPlugin(IExeBoxApi *api);

	/**
		Adds all the current context loggers to the plugins context.
	 */
	void registerLoggers();

	framework::HModule m_PluginDll;
};

}
