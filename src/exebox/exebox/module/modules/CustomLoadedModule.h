#pragma once

#include <filesystem>

#include "framework/memory/VirtualMemory.h"

#include "exebox/api/moduleManager/IModule.h"


namespace exebox {

class CustomLoadedModule final : public IModule
{
public:
	/**
		Loads a module into memory.

		@param moduleMemory - The memory of the module.
		@param name - The name of the module.
	 */
	explicit CustomLoadedModule(framework::VirtualMemory moduleMemory, std::string name);

	void run();

	[[nodiscard]] void *address() override;

	[[nodiscard]] ApiString name() const override;
	[[nodiscard]] void *getFunction(ApiString name) const override;
	[[nodiscard]] void* getFunction(int ordinal) const override;

private:
	std::string m_Name;
	framework::VirtualMemory m_ModuleMemory;
};

}