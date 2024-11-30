#include "exebox/module/manager/ModuleManager.h"
#include "exebox/plugin/PluginManager.h"

#include "exebox/cli/api/ExeBoxApi.h"


namespace exebox::api {

ExeBoxApi::ExeBoxApi()
	: m_ModuleManager(std::make_unique<module::ModuleManager>())
	, m_PluginManager(std::make_unique<plugin::PluginManager>())
{}

IModuleManager& ExeBoxApi::moduleManager()
{
	return *m_ModuleManager;
}

IPluginManager& ExeBoxApi::pluginManager()
{
	return *m_PluginManager;
}

void initializeApi()
{
	g_Api = std::make_unique<ExeBoxApi>();
}

}
