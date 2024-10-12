#include "framework/Exception.h"

#include "framework/memory/VirtualMemory.h"

namespace framework {

void VirtualMemoryDeleter::operator()(void *address) const
{
	if(nullptr == address) {
		return;
	}

	VirtualFree(address, 0, MEM_RELEASE);
}

VirtualMemoryView::VirtualMemoryView(void *address, const size_t length)
	: m_Address(static_cast<uint8_t *>(address), length)
{}

DWORD VirtualMemoryView::setProtection(const DWORD protection)
{
	DWORD oldProtection = 0;
	if(!VirtualProtect(m_Address.data(),
					   m_Address.size_bytes(),
					   static_cast<DWORD>(protection),
					   &oldProtection)) {
		throw WindowsException("Failed to change page protection");
	}

	return oldProtection;
}

std::span<uint8_t> VirtualMemoryView::view()
{
	return m_Address;
}

std::span<const uint8_t> VirtualMemoryView::view() const
{
	return m_Address;
}

uint8_t *VirtualMemoryView::address()
{
	return m_Address.data();
}

const uint8_t *VirtualMemoryView::address() const
{
	return m_Address.data();
}

size_t VirtualMemoryView::length() const
{
	return m_Address.size_bytes();
}

VirtualMemoryView VirtualMemoryView::subView(const size_t offset, const size_t length)
{
	if(offset + length > m_Address.size_bytes()) {
		throw std::out_of_range("Attempted to access out of memory range");
	}

	return VirtualMemoryView(m_Address.data() + offset, length);
}

VirtualMemory::VirtualMemory(uint8_t *address, const size_t length)
	: VirtualMemoryView(address, length)
	, m_Owner(address, VirtualMemoryDeleter{})
{}

VirtualMemory allocate(const size_t length, const std::optional<uintptr_t> preferredAddress)
{
	const auto address = VirtualAlloc(reinterpret_cast<PVOID>(preferredAddress.value_or(0)),
									  length,
									  MEM_RESERVE,
									  PAGE_READWRITE);
	if(nullptr == address) {
		throw WindowsException("Failed to allocate memory");
	}

	VirtualAlloc(address, length, MEM_COMMIT, PAGE_READWRITE);
	return VirtualMemory(static_cast<uint8_t *>(address), length);
}


}