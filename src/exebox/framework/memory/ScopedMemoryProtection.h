#pragma once

#include "framework/memory/VirtualMemory.h"


namespace framework
{
namespace detail
{

struct MemoryProtectionRestorer
{
	DWORD oldProtection;
	size_t length;

	void operator()(void *address) const;
};

using ScopedMemoryProtection = std::unique_ptr<void, MemoryProtectionRestorer>;

}

/**
	Sets the protection of a range of memory for the current scope.
 */
detail::ScopedMemoryProtection scopedMemoryProtection(void *address, size_t length, DWORD protection);
detail::ScopedMemoryProtection scopedMemoryProtection(VirtualMemoryView view, DWORD protection);

/**
	Sets the memory view to be RW for the current scope.
 */
detail::ScopedMemoryProtection scopedWritableMemory(VirtualMemoryView view);

}
