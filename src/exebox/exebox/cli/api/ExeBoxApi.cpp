#include "exebox/module/manager/ModuleManager.h"

#include "exebox/cli/api/ExeBoxApi.h"

namespace exebox::api {

ExeBoxApi::ExeBoxApi()
	: m_ModuleManager(std::make_unique<module::ModuleManager>())
{}

IModuleManager& ExeBoxApi::moduleManager()
{
	return *m_ModuleManager;
}

void initializeApi()
{
	g_Api = std::make_unique<ExeBoxApi>();
}

}
