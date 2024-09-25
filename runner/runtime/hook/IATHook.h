#pragma once

#include <string>

#include "runner/runtime/loader/Module.h"


namespace runner::runtime::hook
{

class IATHook
{
public:

	explicit IATHook(CustomModule module, const std::string& functionName);

private:
	void *m_HookFunction;

};

}