#pragma once

#include <memory>

#include "exebox/api/IExeBoxApi.h"

namespace exebox::api {

class ExeBoxApi final : public IExeBoxApi
{
public:
	explicit ExeBoxApi();

	IModuleManager& moduleManager() override;

private:
	std::unique_ptr<IModuleManager> m_ModuleManager;
};

/**
	Initializes exebox's api object.
 */
void initializeApi();

}
