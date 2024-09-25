#include "runner/runtime/module/Module.h"

namespace runner::runtime {

Module::Module(IModule *module)
	: m_Module(module)
{}

Module::Module(std::unique_ptr<IModule> module)
	: m_Module(std::move(module))
{}

}