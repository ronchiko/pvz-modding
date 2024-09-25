#pragma once

#include <cstdint>

namespace runner::runtime::hook {

struct FixedAddress
{
	/**
		Creates a new fixed absolute address.

		@param address - The absolute address.
	 */
	consteval FixedAddress(uintptr_t address);

	/**
		Converts the fixed address into a opaque pointer.
	 */
	operator void *() const;

	/**
		Converts the address to a typed address.
	 */
	template<typename T>
	T *as();

	uintptr_t address;
};

consteval FixedAddress::FixedAddress(const uintptr_t address)
	: address(address)
{}

inline FixedAddress::operator void *() const
{
	return reinterpret_cast<void *>(address);
}

template<typename T>
T *FixedAddress::as()
{
	return reinterpret_cast<T *>(address);
}


}
