#pragma once

#include <memory>
#include <vector>

#include "runner/runtime/hook/FixedAddress.h"


namespace runner::runtime::hook
{

namespace detail {

template<typename ReturnT, typename... ArgTs>
using ForwardHookType = ReturnT (*)(ArgTs...);

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
	template<typename ReturnT, typename... ArgTs>
	explicit ForwardHook(FixedAddress hookAddress, detail::ForwardHookType<ReturnT, ArgTs...> function);

private:
	explicit ForwardHook(FixedAddress hookAddress, void *functionAddress);

	void *m_HookFunction;
	std::unique_ptr<void, detail::ForwardHookRemover> m_Hook;
};

template <typename ReturnT, typename ... ArgTs>
ForwardHook::ForwardHook(FixedAddress hookAddress, const detail::ForwardHookType<ReturnT, ArgTs...> function)
	: ForwardHook(hookAddress, reinterpret_cast<void*>(function))	
{}

}
