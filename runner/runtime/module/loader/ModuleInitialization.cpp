#include "runner/framework/Exception.h"
#include "runner/framework/pe/ImageImportDescriptors.h"
#include "runner/framework/pe/ImageReLocations.h"
#include "runner/framework/pe/PeHeader.h"
#include "runner/Log.h"

#include "runner/runtime/module/manager/ModuleManager.h"

#include "runner/runtime/module/loader/ModuleInitialization.h"

#include "runner/framework/Overloaded.h"

namespace runner::runtime::moduleLoader {



framework::VirtualMemory allocateMemoryForModule(const size_t length,
												 const uintptr_t desiredBase,
												 const bool forceDesiredBase)
{
	try {
		return framework::allocate(length, desiredBase);
	} catch(const framework::WindowsException& e) {
		if(forceDesiredBase) {
			log::error() << "Failed to allocate module at desired address: 0x" << std::hex << desiredBase
						 << ". Reason: " << e.what() << std::endl;
			throw;
		}

		return framework::allocate(length);
	}
}





}
