#pragma once

#include <memory>
#include <vector>

#include "framework/hook/FixedAddress.h"
#include "framework/hook/assembly/ExecutableAddress.h"


namespace framework::hook {

namespace detail {

template<typename ReturnT, typename... ArgTs>
using ForwardHookType = ReturnT (*)(ArgTs...);

template<typename ReturnT, typename ObjT, typename... ArgTs>
using ClassForwardHookType = ReturnT(ObjT::*)(ArgTs...);

struct ForwardHookRemover
{
	std::vector<std::byte> previousContent;

	void operator()(void *ptr) const;
};

}

class ForwardHook
{
public:
	/**
		Setups a forwarding hook, that works by replacing the first operand of an function
		to be a `jmp` instruction to a different address.

		@param hookAddress - The address to place the hook at.
		@param function - The function to call when the address is reached.
	 */
	explicit ForwardHook(FixedAddress hookAddress, assembly::ExecutableAddress function);

private:
	assembly::ExecutableAddress m_HookFunction;
	std::unique_ptr<void, detail::ForwardHookRemover> m_Hook;
};

}
