#include "framework/memory/ScopedMemoryProtection.h"

namespace framework {

void detail::MemoryProtectionRestorer::operator()(void* address) const
{
	if(nullptr == address) {
		return;
	}

	VirtualMemoryView(address, length).setProtection(oldProtection);
}


detail::ScopedMemoryProtection scopedMemoryProtection(VirtualMemoryView view, const DWORD protection)
{
	const auto oldProtection = view.setProtection(protection);
	return detail::ScopedMemoryProtection{
		view.address(),
		detail::MemoryProtectionRestorer{ oldProtection, view.length() },
	};
}

detail::ScopedMemoryProtection scopedMemoryProtection(void *address, const size_t length, const DWORD protection)
{
	return scopedMemoryProtection(VirtualMemoryView(address, length), protection);
}

detail::ScopedMemoryProtection scopedWritableMemory(const VirtualMemoryView view)
{
	return scopedMemoryProtection(view, PAGE_READWRITE);
}


}