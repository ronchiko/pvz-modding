#include "framework/log/Log.h"


#include "exebox/module/loader/ModuleLoader.h"
#include "exebox/module/manager/MainModule.h"
#include "exebox/module/manager/ModuleManager.h"

#include "exebox/module/HaltedExecutable.h"


namespace exebox {

HaltedExecutable::HaltedExecutable(std::unique_ptr<CustomLoadedModule> module)
	: m_ModuleObject(std::move(module))
{}

HaltedExecutable::~HaltedExecutable()
{
	if(nullptr == m_ModuleObject) {
		return;
	}

	try {
		module::manager().registerModule(*m_ModuleObject);
		m_ModuleObject->run();
	} catch(const std::exception& ex) {
		framework::log::error() << "Executable crashed: " << ex.what();
	} catch(...) {
		framework::log::error() << "Executable crashed: Unknown reason :(";
	}
}

IModule& HaltedExecutable::module() const
{
	return *m_ModuleObject;
}

HaltedExecutable createHaltedRuntime(const std::filesystem::path& executablePath)
{
	auto module = module::loader::loadModule(executablePath);
	module::mainModuleProvider().setMainModule(*module);

	return HaltedExecutable(std::move(module));
}

}
