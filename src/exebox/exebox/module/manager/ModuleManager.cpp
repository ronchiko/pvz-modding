#include "framework/log/Log.h"

#include "exebox/module/manager/ModuleManager.h"

namespace exebox::module {

ModuleManager::ModuleManager()
	: m_MainModuleProvider(std::make_unique<MainModuleProvider>())
{}

void ModuleManager::registerModule(IModule& module)
{
	m_Modules[module.name()] = &module;
}

IModule *ModuleManager::getRegisteredModule(const ApiString& name)
{
	const auto module = m_Modules.find(name);
	if(m_Modules.end() == module) {
		return nullptr;
	}

	return module->second;
}

IModule *ModuleManager::getRegisteredModuleByAddress(const void *address)
{
	const auto iterator = std::ranges::find_if(m_Modules, [address](const auto& module) {
		return address == module.second->address();
	});
	if(m_Modules.end() == iterator) {
		return nullptr;
	}

	return iterator->second;
}

}
