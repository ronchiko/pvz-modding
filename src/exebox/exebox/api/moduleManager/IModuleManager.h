#pragma once

#include "exebox/api/moduleManager/IModule.h"
#include "exebox/api/types/ApiString.h"
#include "exebox/api/moduleManager/IMainModuleProvider.h"

namespace exebox {

class IModuleManager
{
public:
	virtual ~IModuleManager() = default;

	/**
		Registers a new exe box.

		@param module - The module to add.
	 */
	virtual void registerModule(IModule& module) = 0;

	/**
		Returns a module by its name if it exists.

		@param name - The name of the module.

		@return - Address to the IModule if the module is found, nullptr otherwise.
	 */
	virtual IModule *getRegisteredModule(const ApiString& name) = 0;

	/**
		Returns an existing module by its address.

		@param address - The address inside the module.

		@return - The address to the IModule if the module is found, nullptr otherwise.
	 */
	virtual IModule *getRegisteredModuleByAddress(const void *address) = 0;

	/**
		Returns the main module provider.
	 */
	virtual IMainModuleProvider& mainModuleProvider() = 0;
};

}
