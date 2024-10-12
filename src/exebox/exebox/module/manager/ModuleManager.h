#pragma once

#include <string>
#include <unordered_map>

#include "exebox/ApiInterface.h"
#include "exebox/module/manager/MainModule.h"
#include "exebox/api/moduleManager/IModuleManager.h"


namespace exebox::module {

class ModuleManager final : public IModuleManager
{
public:
	explicit ModuleManager();

	/**
		Adds a module to the known modules list.

		@param module - The module to add.
	 */
	void registerModule(IModule& module) override;

	/**
		Gets a module if its already registered.

		@param name - The name of the module.
	 */
	IModule *getRegisteredModule(const ApiString& name) override;

	/**
		Returns a registered module by its address.
	 */
	IModule *getRegisteredModuleByAddress(const void* address) override;

	/**
		Returns the main module provider.
	 */
	IMainModuleProvider& mainModuleProvider() override
	{
		return *m_MainModuleProvider;
	}

private:
	std::unique_ptr<IMainModuleProvider> m_MainModuleProvider;
	std::unordered_map<std::string, IModule *> m_Modules;
};

/**
	Returns the current module manager.
 */
inline IModuleManager& manager()
{
	return g_Api->moduleManager();
}

}
