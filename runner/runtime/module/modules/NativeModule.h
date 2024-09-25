#pragma once

#include <stdexcept>
#include <Windows.h>

#include "runner/runtime/module/IModule.h"

namespace runner::runtime {

class FailedToLoadNativeModuleException : public std::runtime_error
{
public:
	explicit FailedToLoadNativeModuleException(const std::string& moduleName);

private:
	static std::string createMessage(const std::string& moduleName);
};

class NativeModule final : public IModule
{
public:
	explicit NativeModule(HMODULE module);

	[[nodiscard]] const std::string& name() const override;

	[[nodiscard]] void *getFunction(const std::string& name) const override;
	[[nodiscard]] void* getFunction(int ordinal) const override;

private:
	HMODULE m_Module;
	std::string m_Name;
};

}
