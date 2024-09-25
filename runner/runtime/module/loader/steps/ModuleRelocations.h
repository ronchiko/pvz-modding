#pragma once

#include "runner/framework/memory/VirtualMemory.h"


namespace runner::runtime::moduleLoader {

/**
Preforms re-locations on the image according to the image directory.

@param image - The image to re-locate.
*/
void relocateImage(framework::VirtualMemoryView& image);

}
