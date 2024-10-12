#pragma once

#include "exebox/api/moduleManager/IModuleManager.h"

namespace exebox {

class IExeBoxApi
{
public:
	virtual ~IExeBoxApi() = default;

	/**
		Returns ExeBox's module manager.
	 */
	virtual IModuleManager& moduleManager() = 0;
};

}