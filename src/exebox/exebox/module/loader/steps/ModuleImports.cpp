#include "framework/Exception.h"
#include "framework/Overloaded.h"
#include "framework/pe/ImageImportDescriptors.h"

#include "exebox/module/manager/ModuleManager.h"

#include "exebox/module/loader/steps/ModuleImports.h"

namespace exebox::module::loader {

namespace {

	
/**
	Returns a procedures address by its name.

	@param moduleName - The name of the module to get a procedure from.
	@param functionName - The name of the function.
 */
void *getProcedureAddress(const std::string& moduleName, const std::string& functionName)
{
	if(const auto module = manager().getRegisteredModule(moduleName)) {
		return module->getFunction(functionName);
	}

	const auto hModule = LoadLibraryA(moduleName.c_str());
	return GetProcAddress(hModule, functionName.c_str());
}

/**
	Returns a procedures address by an ordinal.

	@param moduleName - The name of the module to get a procedure from.
	@param ordinal - The ordinal of the function.
 */
void *getProcedureAddress(const std::string& moduleName, const int ordinal)
{
	if(const auto module = manager().getRegisteredModule(moduleName)) {
		return module->getFunction(ordinal);
	}

	const auto hModule = LoadLibraryA(moduleName.c_str());
	return GetProcAddress(hModule, MAKEINTRESOURCEA(ordinal));
}

}

void initializeImageImports(framework::VirtualMemoryView& image)
{
	for(const auto& import : framework::pe::ImageImportDescriptors(image.address())) {
		// clang-format off
		*import.functionRef = reinterpret_cast<uintptr_t>(std::visit(
		  [&](const auto& name) -> void* {
		  	  return getProcedureAddress(import.function.importName, name);
		  }, import.function.function));
		// clang-format on
	}
}

}
