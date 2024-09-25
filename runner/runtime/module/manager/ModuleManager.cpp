#include "runner/runtime/module/manager/ModuleManager.h"

#include <unordered_map>

#include "runner/runtime/module/IModule.h"
#include "runner/runtime/module/modules/NativeModule.h"

namespace runner::runtime::moduleManager {

namespace {

std::unordered_map<std::string, std::unique_ptr<IModule>> g_KnownModules{};

}

Module loadModule(const std::string& moduleName)
{
	if(g_KnownModules.contains(moduleName)) {
		return Module(g_KnownModules[moduleName].get());
	}

	const auto module = LoadLibraryA(moduleName.c_str());
	if(INVALID_HANDLE_VALUE == module || nullptr == module){
		throw FailedToLoadNativeModuleException(moduleName);
	}

	return Module(std::make_unique<NativeModule>(module));
}

Module getModule(const std::string& moduleName)
{
	if(g_KnownModules.contains(moduleName)) {
		return Module(g_KnownModules[moduleName].get());
	}

	return Module(std::make_unique<NativeModule>(GetModuleHandleA(moduleName.c_str())));
}

IModule& registerModule(std::unique_ptr<IModule> module)
{
	auto moduleName = module->name();
	return *(g_KnownModules[std::move(moduleName)] = std::move(module));
}


}
