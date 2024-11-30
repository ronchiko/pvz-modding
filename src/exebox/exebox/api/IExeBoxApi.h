#pragma once

#include "exebox/api/moduleManager/IModuleManager.h"
#include "exebox/api/plugin/IPluginManager.h"


namespace exebox {

class IExeBoxApi
{
public:
	virtual ~IExeBoxApi() = default;

	/**
		Returns ExeBox's module manager.
	 */
	virtual IModuleManager& moduleManager() = 0;

	/**
		Returns ExeBox's plugin manager.
	 */
	virtual IPluginManager& pluginManager() = 0;
};

}