#pragma once

#include <memory>

#include "exebox/api/IExeBoxApi.h"

namespace exebox::api {

class ExeBoxApi final : public IExeBoxApi
{
public:
	explicit ExeBoxApi();

	IModuleManager& moduleManager() override;
	IPluginManager& pluginManager() override;

private:
	std::unique_ptr<IModuleManager> m_ModuleManager;
	std::unique_ptr<IPluginManager> m_PluginManager;
};

/**
	Initializes exebox's api object.
 */
void initializeApi();

}
