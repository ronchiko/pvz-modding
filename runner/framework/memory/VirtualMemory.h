#pragma once

#include <memory>
#include <optional>
#include <span>

#include <Windows.h>


namespace framework {

class VirtualMemoryDeleter
{
public:
	void operator()(void *address) const;
};

class VirtualMemoryView
{
public:
	explicit VirtualMemoryView(void *address, size_t length);

	/**
		Changes the protection of the memory region. Returns the previous protection of the memory.

		@param protection - The new protection to set to the view.
	 */
	DWORD setProtection(DWORD protection);

	/**
		Returns a view of the memory.
	 */
	[[nodiscard]] std::span<uint8_t> view();
	[[nodiscard]] std::span<const uint8_t> view() const;

	/**
		Returns the address of the memory.
	 */
	[[nodiscard]] uint8_t *address();
	[[nodiscard]] const uint8_t *address() const;

	/**
		Returns the length of the view.
	 */
	[[nodiscard]] size_t length() const;

	/**
		Creates a sub view of the current memory.
	 */
	VirtualMemoryView subView(size_t offset, size_t length);

private:
	std::span<uint8_t> m_Address;
};

/**
	An owned virtual memory section.
 */
class VirtualMemory : public VirtualMemoryView
{
public:
	explicit VirtualMemory(uint8_t *address, size_t length);

private:
	std::unique_ptr<uint8_t, VirtualMemoryDeleter> m_Owner;
};

/**
	Allocates a piece of RW memory with a given length.

	@param length - The length of the memory.
	@param preferredAddress - The preferred address of the memory.
 */
VirtualMemory allocate(size_t length, std::optional<uintptr_t> preferredAddress = std::nullopt);

}
