#include "exebox/module/loader/FileModuleAllocator.h"
#include "exebox/module/loader/steps/ModuleSections.h"
#include "exebox/module/loader/steps/ModuleImports.h"
#include "exebox/module/loader/steps/ModuleRelocations.h"

#include "exebox/module/manager/ModuleManagerHooks.h"

#include "exebox/module/loader/ModuleLoader.h"

namespace exebox::module::loader {

namespace {

/**
	Given an uninitialized image, initializes it.
 */
void initializeImage(framework::VirtualMemoryView& module)
{
	relocateImage(module);
	initializeImageImports(module);
	finalizeImageSections(module);
}

}

std::unique_ptr<CustomLoadedModule> loadModule(const std::filesystem::path& path)
{
	auto moduleAddress = allocateModuleFromFile(path, true);
	initializeImage(moduleAddress);
	auto module = std::make_unique<CustomLoadedModule>(std::move(moduleAddress), path.filename().string());

	module::hooks::installManagerHooks(*module);

	return module;
}

}
