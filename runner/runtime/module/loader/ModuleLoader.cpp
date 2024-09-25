#include "runner/runtime/module/loader/FileModuleAllocator.h"
#include "runner/runtime/module/loader/steps/ModuleSections.h"
#include "runner/runtime/module/loader/steps/ModuleImports.h"
#include "runner/runtime/module/loader/steps/ModuleRelocations.h"

#include "runner/runtime/module/loader/ModuleLoader.h"

namespace runner::runtime::moduleLoader {

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
	auto module = allocateModuleFromFile(path, true);
	initializeImage(module);

	return std::make_unique<CustomLoadedModule>(std::move(module), path.filename().string());
}

}
