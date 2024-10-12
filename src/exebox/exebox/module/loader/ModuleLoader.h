#pragma once

#include <memory>
#include <filesystem>

#include "exebox/module/modules/CustomLoadedModule.h"

namespace exebox::module::loader {

/**
	Loads a custom module from a path.
 */
std::unique_ptr<CustomLoadedModule> loadModule(const std::filesystem::path& path);

}
