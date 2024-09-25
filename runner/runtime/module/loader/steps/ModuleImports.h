#pragma once

#include "runner/framework/memory/VirtualMemory.h"


namespace runner::runtime::moduleLoader
{

/**
	Initializes the imports of the image.
 */
void initializeImageImports(framework::VirtualMemoryView& image);

}
