#include "exebox/module/manager/MainModule.h"

namespace exebox::module {

IModule *MainModuleProvider::mainModule()
{
	return m_MainModule;
}

void MainModuleProvider::setMainModule(IModule& module)
{
	m_MainModule = &module;
}

}
