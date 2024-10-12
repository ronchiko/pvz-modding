#pragma once

#include "exebox/ApiInterface.h"
#include "exebox/api/moduleManager/IMainModuleProvider.h"


namespace exebox::module {

class MainModuleProvider final : public IMainModuleProvider
{
public:
	/**
		Returns the main module.
	 */
	IModule* mainModule() override;

	/**
		Sets the main module.
	 */
	void setMainModule(IModule& module) override;

private:
	IModule *m_MainModule = nullptr;
};

/**
	Returns the main module provider.
 */
inline IMainModuleProvider& mainModuleProvider()
{
	return g_Api->moduleManager().mainModuleProvider();
}

}
