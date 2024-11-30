#include "exebox/plugin/Plugin.h"

#include "framework/log/ILogger.h"
#include "framework/log/LogPublisher.h"

namespace exebox::plugin {

namespace {

const std::string INSTALL_EXPORT_NAME = "install";
const std::string UNINSTALL_EXPORT_NAME = "uninstall";
const std::string ADD_LOGGER_EXPORT_NAME = "logPublisher_addLogger";

}

Plugin::Plugin(const std::filesystem::path& path)
	: m_PluginDll(framework::HModule::loadLibrary(path))
{}

void Plugin::install(IExeBoxApi *api)
{
	registerLoggers();
	installPlugin(api);
}

void Plugin::uninstall(IExeBoxApi *api)
{
	using UninstallFunc_t = void(__cdecl)(IExeBoxApi *);
	const auto uninstallProcedure = m_PluginDll.getProcedure<UninstallFunc_t>(UNINSTALL_EXPORT_NAME);
	if(!uninstallProcedure) {
		throw PluginInstallFailed("Missing `uninstall` export");
	}

	(*uninstallProcedure)(api);
}

void Plugin::installPlugin(IExeBoxApi *api)
{
	using InstallFunc_t = bool(__cdecl)(IExeBoxApi *);

	const auto installProcedure = m_PluginDll.getProcedure<InstallFunc_t>(INSTALL_EXPORT_NAME);
	if(!installProcedure) {
		throw PluginInstallFailed("Missing `install` export");
	}

	if(!(*installProcedure)(api)) {
		throw PluginInstallFailed("`install` exited with failure code");
	}
}

void Plugin::registerLoggers()
{
	using AddLoggerFunc_t = void(__cdecl)(framework::log::ILogger *);

	const auto registerLoggerProcedure = m_PluginDll.getProcedure<AddLoggerFunc_t>(ADD_LOGGER_EXPORT_NAME);
	if(!registerLoggerProcedure) {
		// No export? fine.
		return;
	}

	for(const auto& logger : framework::log::publisher::getAllRegisteredLoggers()) {
		(*registerLoggerProcedure)(&logger.get());
	}
}
}
