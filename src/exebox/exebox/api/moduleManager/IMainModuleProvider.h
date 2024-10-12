#pragma once

#include "exebox/api/moduleManager/IModule.h"

namespace exebox {

class IMainModuleProvider
{
public:
	virtual ~IMainModuleProvider() = default;

	/**
		Returns the current main module if its set, nullptr otherwise.
	 */
	virtual IModule *mainModule() = 0;

	/**
		Sets the main module.
	 */
	virtual void setMainModule(IModule& module) = 0;
};

}
