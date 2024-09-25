#include <algorithm>
#include <ranges>

#include "runner/framework/memory/ScopedMemoryProtection.h"
#include "runner/Log.h"

#include "runner/runtime/hook/ForwardHook.h"

namespace runner::runtime::hook {

namespace {

const std::vector<uint8_t> JUMP_INSTRUCTIONS = {
	0xCC,						  // INT 3
	0xB8, 0xFF, 0xFF, 0xFF, 0xFF, // MOVE EAX, 0xFFFFFFFF
	0xFF, 0x20,					  // JMP DWORD PTR [EAX]
};

std::vector<uint8_t> createAbsoluteJumpIntrinsic(void *targetAddress)
{
	std::vector<uint8_t> instructions = JUMP_INSTRUCTIONS;
	std::span view(reinterpret_cast<uint8_t *>(&targetAddress), sizeof(void *));

	std::ranges::copy(view, instructions.data() + 2);

	return instructions;
}

}

void detail::ForwardHookRemover::operator()(void *ptr) const
{
	if(nullptr == ptr) {
		return;
	}

	std::memcpy(ptr, previousContent.data(), previousContent.size());
}

ForwardHook::ForwardHook(FixedAddress hookAddress, void *functionAddress)
	: m_HookFunction(functionAddress)
{
	std::vector<std::byte> previousContent;
	{
		auto memoryGuard = scopedWritableMemory(framework::VirtualMemoryView{ hookAddress, 256 });

		auto hookInstructions = createAbsoluteJumpIntrinsic(&m_HookFunction);
		previousContent.reserve(hookInstructions.size());
		std::ranges::copy_n(hookAddress.as<std::byte>(), hookInstructions.size(), previousContent.data());

		std::ranges::copy(hookInstructions, hookAddress.as<uint8_t>());
	}

	log::info() << "Placed forward hook at: 0x" << std::hex << hookAddress.address << std::endl;
	m_Hook = { hookAddress, detail::ForwardHookRemover{ std::move(previousContent) } };
}


}
