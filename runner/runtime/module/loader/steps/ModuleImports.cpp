#include "runner/framework/Exception.h"
#include "runner/framework/Overloaded.h"
#include "runner/framework/pe/ImageImportDescriptors.h"
#include "runner/runtime/module/manager/ModuleManager.h"

#include "runner/runtime/module/loader/steps/ModuleImports.h"


namespace runner::runtime::moduleLoader {

void initializeImageImports(framework::VirtualMemoryView& image)
{
	for(const auto& import : framework::pe::ImageImportDescriptors(image.address())) {
		auto importModule = moduleManager::loadModule(import.function.importName);

		// clang-format off
		*import.functionRef = reinterpret_cast<uintptr_t>(std::visit(framework::Overloaded{
		  [&](const int ordinal) {
		  	  return importModule.getFunction<uint32_t>(IMAGE_ORDINAL32(ordinal));
		  },
		  [&](const std::string& functionName) {
			  return importModule.getFunction<uint32_t>(functionName);
		  }
		}, import.function.function));
		// clang-format on
	}
}

}
