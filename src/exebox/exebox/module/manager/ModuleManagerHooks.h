#pragma once

#include <cstdint>

#include "exebox/api/moduleManager/IModule.h"


namespace exebox::module::hooks {

/**
	Install the manager hooks on a specific module.

	This is required for modules that are loaded dynamically in order to make the modules work.
 */
void installManagerHooks(IModule& module);

}
