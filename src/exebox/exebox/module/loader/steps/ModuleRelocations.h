#pragma once

#include "framework/memory/VirtualMemory.h"


namespace exebox::module::loader {

/**
Preforms re-locations on the image according to the image directory.

@param image - The image to re-locate.
*/
void relocateImage(framework::VirtualMemoryView& image);

}
