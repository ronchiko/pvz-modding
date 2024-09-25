#pragma once

#include <filesystem>
#include <memory>

#include "runner/runtime/module/modules/CustomLoadedModule.h"

namespace runner::runtime {

/**
	Creates the runtime in an halted state, allowing the user to do modifications to it.

	When this object is destroyed, the runtime begins.
 */
class HaltedExecutable
{
public:
	explicit HaltedExecutable(std::unique_ptr<CustomLoadedModule> module);

	HaltedExecutable(HaltedExecutable&&) = default;
	HaltedExecutable& operator=(HaltedExecutable&&) = default;

	HaltedExecutable(const HaltedExecutable&) = delete;
	HaltedExecutable& operator=(const HaltedExecutable&) = delete;

	~HaltedExecutable();

private:
	std::unique_ptr<CustomLoadedModule> m_ModuleObject;
};

/**
	Creates an halted runtime for an executable.

	@param executablePath - The path to the EXE of PvZ.
 */
HaltedExecutable createHaltedRuntime(const std::filesystem::path& executablePath);

}
