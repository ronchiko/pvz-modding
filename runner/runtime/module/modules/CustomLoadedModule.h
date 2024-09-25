#pragma once

#include <filesystem>

#include "runner/framework/memory/VirtualMemory.h"

#include "runner/runtime/module/IModule.h"


namespace runner::runtime {

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

	[[nodiscard]] uint8_t *address();
	[[nodiscard]] const uint8_t *address() const;

	[[nodiscard]] const std::string& name() const override;
	[[nodiscard]] void* getFunction(const std::string& name) const override;
	[[nodiscard]] void* getFunction(int ordinal) const override;

private:
	std::string m_Name;
	framework::VirtualMemory m_ModuleMemory;
};

}