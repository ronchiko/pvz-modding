#include <filesystem>

#include "framework/log/Log.h"

#include "exebox/hook/ImportHook.h"
#include "exebox/module/manager/ModuleManager.h"
#include "exebox/module/modules/NativeModule.h"


#include "exebox/module/manager/ModuleManagerHooks.h"

namespace exebox::module::hooks {

namespace {

void *WINAPI loadLibraryHook(const char *libraryName)
{
	framework::log::info() << "Loading module: " << libraryName;
	if(const auto module = module::manager().getRegisteredModule(std::string_view(libraryName))) {
		return module->address();
	}

	return LoadLibraryA(libraryName);
}

void *WINAPI getModuleHandle(const char *libraryName)
{
	if(nullptr == libraryName) {
		return mainModuleProvider().mainModule()->address();	
	}

	if(const auto module = module::manager().getRegisteredModule(std::string_view(libraryName))) {
		return module->address();
	}

	return GetModuleHandleA(libraryName);
}

void WINAPI freeLibraryHook(const void *module)
{
	framework::log::info() << "Releasing: " << module;
}

void *WINAPI getProcAddressHook(void *moduleAddress, const char *procedureName)
{
	if(const auto module = module::manager().getRegisteredModuleByAddress(moduleAddress)) {
		return module->getFunction(std::string_view(procedureName));
	}

	return GetProcAddress(static_cast<HMODULE>(moduleAddress), procedureName);
}

DWORD WINAPI getModuleFileNameHook(const void *moduleAddress, const LPSTR result, const DWORD resultLength)
{
	if(nullptr == moduleAddress) {
		return getModuleFileNameHook(mainModuleProvider().mainModule()->address(), result, resultLength);
	}

	const auto module = manager().getRegisteredModuleByAddress(moduleAddress);
	if(nullptr == module) {
		framework::log::error() << "Module at " << moduleAddress << " is not loaded";
		return 0;
	}

	const auto path = (std::filesystem::current_path() / std::string_view(module->name())).string();
	const auto length = std::min<uint32_t>(path.length(), resultLength);

	result[length] = 0;
	memcpy(result, path.c_str(), length);

	framework::log::info() << "Returned file path " << result;
	return length;
}

}

void installManagerHooks(IModule &module)
{
	hook::createImportHook(module, "KERNEL32.dll")
		.hook("LoadLibraryA", loadLibraryHook)
		.hook("GetModuleHandleA", getModuleHandle)
		.hook("FreeLibrary", freeLibraryHook)
		.hook("GetProcAddress", getProcAddressHook)
		.hook("GetModuleFileNameA", getModuleFileNameHook);
}

}
