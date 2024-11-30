#include "framework/Exception.h"
#include "framework/log/Log.h"

#include "exebox/module/loader/ModuleInitialization.h"

namespace exebox::module::loader {

framework::VirtualMemory allocateMemoryForModule(const size_t length,
												 const uintptr_t desiredBase,
												 const bool forceDesiredBase)
{
	try {
		return framework::VirtualMemory::allocate(length, desiredBase);
	} catch(const framework::WindowsException& e) {
		if(forceDesiredBase) {
			framework::log::error() << "Failed to allocate module at desired address: 0x" << std::hex << desiredBase
									<< ". Reason: " << e.what();
			throw;
		}

		return framework::VirtualMemory::allocate(length);
	}
}

}
