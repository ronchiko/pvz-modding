#pragma once

#include <memory>
#include <filesystem>

#include "..\modules\CustomLoadedModule.h"

namespace runner::runtime::moduleLoader {

/**
	Loads a custom module from a path.
 */
std::unique_ptr<CustomLoadedModule> loadModule(const std::filesystem::path& path);

}
