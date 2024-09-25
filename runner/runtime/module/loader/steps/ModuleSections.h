#pragma once

#include "runner/framework/memory/VirtualMemory.h"

namespace runner::runtime::moduleLoader {

/**
	Initializes the sections of an image from its reference.

	@param moduleBase - The memory that was allocated for the image.
	@param imageRef - The reference for the image.
*/
void initializeImageSections(framework::VirtualMemoryView& moduleBase, const uint8_t *imageRef);

/**
	Finalizes the sections of a module.

	@param moduleBase - The address of the module.
 */
void finalizeImageSections(framework::VirtualMemoryView& moduleBase);

}