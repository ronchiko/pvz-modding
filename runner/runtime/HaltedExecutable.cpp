#include "runner/Log.h"

#include "runner/runtime/module/loader/ModuleLoader.h"
#include "runner/runtime/module/manager/ModuleManager.h"

#include "runner/runtime/HaltedExecutable.h"

namespace runner::runtime {

HaltedExecutable::HaltedExecutable(std::unique_ptr<CustomLoadedModule> module)
	: m_ModuleObject(std::move(module))
{}

HaltedExecutable::~HaltedExecutable()
{
	if(nullptr == m_ModuleObject) {
		return;
	}

	try {
		auto& module = moduleManager::registerModule(std::move(m_ModuleObject));
		module.run();
	} catch(const std::exception& ex) {
		log::error() << "The Plants Vs Zombies executable with error: " << ex.what() << std::endl;
	} catch(...) {
		log::error() << "The Plants Vs Zombies executable failed unexpectedly" << std::endl;
	}
}

HaltedExecutable createHaltedRuntime(const std::filesystem::path& executablePath)
{
	auto module = moduleLoader::loadModule(executablePath);
	return HaltedExecutable(std::move(module));
}

}
