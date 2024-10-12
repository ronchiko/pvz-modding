#pragma once

#include <string>
#include <unordered_map>

#include "exebox/module/modules/CustomLoadedModule.h"


namespace exebox::hook {

namespace detail {

class ImportHookBuilder
{
public:
	explicit ImportHookBuilder(void *moduleBase, std::string moduleName);

	ImportHookBuilder(ImportHookBuilder&&) = default;
	ImportHookBuilder(const ImportHookBuilder&) = delete;

	ImportHookBuilder& operator=(ImportHookBuilder&&) = default;
	ImportHookBuilder& operator=(const ImportHookBuilder&) = delete;

	~ImportHookBuilder();

	/**
		Binds an import function call to a different function.

		@param functionName - The name of the import to hook.
		@param hook - The hook to use.
	 */
	template<typename FunctionT>
	ImportHookBuilder& hook(std::string functionName, FunctionT *hook);

private:
	/**
		Actually installs the function hooks on the module.
	 */
	void installHooks();

	void *m_ModuleBase;
	std::string m_ModuleName;
	std::unordered_map<std::string, void *> m_HookMapping;
};

template <typename FunctionT>
ImportHookBuilder& ImportHookBuilder::hook(std::string functionName, FunctionT* hook)
{
	m_HookMapping.emplace(std::move(functionName), reinterpret_cast<void *>(hook));
	return *this;
}

}

/**
	Creates a new import hook builder, this hooks hijacks address on the import table of the module.

	@param module - The address of the module we want to add import hooks for.
	@param hookedModuleName - The name of the module we want to hook.
 */
detail::ImportHookBuilder createImportHook(IModule& module, std::string hookedModuleName);

struct ImportHookDefinition
{
	std::string functionName;
	void *function;
};

class ImportHookGuard
{
public:
	using memory_t = std::vector<int>;

};

/**
	Installs IAT hooks on a specific loaded module.

	@param module - The module to install the hooks on.
	@param moduleName - The name of the imported module to place the hooks on.
	@param functions - The list of functions to hook.
 */
void installImportHooks(IModule *module, const std::string& moduleName, std::initializer_list<ImportHookDefinition> functions);

}