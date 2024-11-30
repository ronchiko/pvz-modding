#pragma once

#include "exebox/api/types/ApiString.h"

namespace exebox {

class IPluginManager
{
public:
	virtual ~IPluginManager() = default;

	/**
		Checks if a plugin is loaded.

		@param name - The name of the plugin.
	 */
	virtual bool isLoaded(const ApiString& name) = 0;

	/**
		Loads a plugin into the current context.
		Returns true if the plugin was loaded successfully.

		@param pluginPath - The path of the plugin.
	 */
	virtual bool loadPlugin(const ApiString& pluginPath) = 0;
};

}
