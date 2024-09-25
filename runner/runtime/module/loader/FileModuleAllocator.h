#pragma once

#include <filesystem>

#include "runner/framework/pe/PeHeader.h"
#include "runner/framework/memory/VirtualMemory.h"

namespace runner::runtime::moduleLoader {

/**
	Allocates the memory required for a module specified in a path.

	@param path - The path to the module.
	@param forceDesiredBase - Should the allocation force the module to be loaded at its desired base address.
 */
framework::VirtualMemory allocateModuleFromFile(const std::filesystem::path& path, bool forceDesiredBase);

}
