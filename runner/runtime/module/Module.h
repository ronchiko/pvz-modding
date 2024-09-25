#pragma once

#include <memory>
#include <variant>

#include "runner/runtime/module/IModule.h"


namespace runner::runtime
{

class Module
{
public:
	explicit Module(std::unique_ptr<IModule> module);
	explicit Module(IModule* module);

	template<typename FunctionT>
	FunctionT *getFunction(const std::string& name);

	template<typename FunctionT>
	FunctionT *getFunction(const int ordinal);

private:
	std::variant<std::unique_ptr<IModule>, IModule *> m_Module;
};

template <typename FunctionT>
FunctionT* Module::getFunction(const std::string& name)
{
	auto function = std::visit([&](auto& module) { return module->getFunction(name); }, m_Module);
	return static_cast<FunctionT *>(function);
}

template <typename FunctionT>
FunctionT* Module::getFunction(const int ordinal)
{
	auto function = std::visit([&](auto& module) { return module->getFunction(ordinal); }, m_Module);
	return static_cast<FunctionT *>(function);
}

}
