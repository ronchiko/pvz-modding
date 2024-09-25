#pragma once

#include <memory>
#include <string>

#include "runner/runtime/module/Module.h"


namespace runner::runtime::moduleManager {

/**
	Attempts to load a module using its name.
 */
Module loadModule(const std::string& moduleName);

/**
	Attempts to get an already loaded module.
 */
Module getModule(const std::string& moduleName);

/**
	Registers a new module to the known modules pool.
 */
IModule& registerModule(std::unique_ptr<IModule> module);

template<typename T, typename = std::enable_if_t<std::is_base_of_v<IModule, T>>>
T& registerModule(std::unique_ptr<T> module)
{
	return dynamic_cast<T&>(registerModule(std::unique_ptr<IModule>(module.release())));
}

}
