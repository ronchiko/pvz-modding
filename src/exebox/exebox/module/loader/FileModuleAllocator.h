#pragma once

#include <filesystem>

#include "framework/memory/VirtualMemory.h"

namespace exebox::module::loader {

/**
	Allocates the memory required for a module specified in a path.

	@param path - The path to the module.
	@param forceDesiredBase - Should the allocation force the module to be loaded at its desired base address.
 */
framework::VirtualMemory allocateModuleFromFile(const std::filesystem::path& path, bool forceDesiredBase);

}
