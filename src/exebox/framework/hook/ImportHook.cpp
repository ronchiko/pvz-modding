#include "framework/log/Log.h"

#include "framework/memory/ScopedMemoryProtection.h"
#include "framework/pe/ImageImportDescriptors.h"

#include "framework/hook/ImportHook.h"

namespace framework::hook {

namespace detail {

ImportHookBuilder::ImportHookBuilder(void *moduleBase, std::string moduleName)
	: m_ModuleBase(moduleBase)
	, m_ModuleName(std::move(moduleName))
{}

ImportHookBuilder::~ImportHookBuilder()
{
	try {
		installHooks();
	} catch(const std::exception& ex) {
		framework::log::error() << "Failed install import hooks. Error: " << ex.what();
	}
}

void ImportHookBuilder::installHooks()
{
	for(const auto& import : framework::pe::ImageImportDescriptors(m_ModuleBase)) {
		if(m_ModuleName != import.function.importName) {
			continue;
		}

		if(!std::holds_alternative<std::string>(import.function.function)) {
			// We dont support ordinals yet.
			continue;
		}

		const auto& functionName = std::get<std::string>(import.function.function);
		const auto hookInformation = m_HookMapping.find(functionName);
		if(m_HookMapping.end() == hookInformation) {
			continue;
		}

		{
			auto guard = scopedWritableMemory(framework::VirtualMemoryView{ import.functionRef, 256 });

			*import.functionRef = reinterpret_cast<uintptr_t>(hookInformation->second);
			framework::log::info() << "Add import hook for " << m_ModuleName << "::" << functionName << " to address "
								   << hookInformation->second;
		}
	}
}

}

detail::ImportHookBuilder createImportHook(exebox::IModule& module, std::string hookedModuleName)
{
	return detail::ImportHookBuilder(const_cast<void *>(module.address()), std::move(hookedModuleName));
}

}
