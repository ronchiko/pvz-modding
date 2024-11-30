#pragma once

#include "runner/runtime/module/Module.h"

namespace runner::steam {

/**
	Registers a fake version of steam_api.dll to the runtime.
 */
runtime::Module addFakeSteamApi();

}
