#pragma once

#include "framework/memory/VirtualMemory.h"


namespace exebox::module::loader {

/**
	Initializes the imports of the image.
 */
void initializeImageImports(framework::VirtualMemoryView& image);

}
