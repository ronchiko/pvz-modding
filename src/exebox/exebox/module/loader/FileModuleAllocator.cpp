#include "framework/io/File.h"
#include "framework/pe/PeHeader.h"

#include "exebox/module/loader/ModuleInitialization.h"
#include "exebox/module/loader/steps/ModuleSections.h"

#include "exebox/module/loader/FileModuleAllocator.h"


namespace exebox::module::loader {

framework::VirtualMemory allocateModuleFromFile(const std::filesystem::path& path, const bool forceDesiredBase)
{
	const auto moduleRef = framework::io::readFile(path);
	const framework::pe::PeHeader headerRef(moduleRef.data());

	// Allocate memory for the entire module, and allocate the module.
	auto module = allocateMemoryForModule(headerRef->OptionalHeader.SizeOfImage, headerRef->OptionalHeader.ImageBase, forceDesiredBase);
	std::memcpy(module.address(), moduleRef.data(), headerRef->OptionalHeader.SizeOfHeaders);

	initializeImageSections(module, moduleRef.data());

	return module;
}

}
