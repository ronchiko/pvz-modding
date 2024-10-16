#pragma once

#include "framework/memory/VirtualMemory.h"

namespace exebox::module::loader {


/**
	Allocates the memory required for the for a module.

	@param length - The length of the memory to allocate.
	@param desiredBase - The desired base address of the module.
	@param forceDesiredBase - Should fail if the desired address if occupied.
 */
framework::VirtualMemory allocateMemoryForModule(size_t length, uintptr_t desiredBase, bool forceDesiredBase);

}