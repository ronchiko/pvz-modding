#pragma once

#include <concepts>
#include <memory>


namespace framework::hook::assembly {

namespace detail {

// clang-format off
template<typename T>
concept HasAddressFunction = requires(T instance) {
	 { instance.address() } -> std::same_as<void *>;
};
// clang-format on

class IAddressable // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	virtual ~IAddressable() = default;

	virtual void *address() = 0;
};

class Address final : public IAddressable
{
public:
	explicit Address(void *address)
		: m_Address(address)
	{}

	void *address() override { return m_Address; }
private:
	void *m_Address;
};

template<HasAddressFunction AddressableT>
class AddressableInstance final : public IAddressable
{
public:
	explicit AddressableInstance(AddressableT instance)
		: m_Addressable(std::move(instance))
	{}

	void* address() override { return m_Addressable.address(); }

private:
	AddressableT m_Addressable;
};

}

class ExecutableAddress
{
public:
	ExecutableAddress(void* address)
		: m_Addressable(new detail::Address(address))
	{}

	ExecutableAddress(detail::HasAddressFunction auto addressable)
		: m_Addressable(new detail::AddressableInstance(std::move(addressable)))
	{}

	[[nodiscard]] void *address() const { return m_Addressable->address(); }

private:
	std::unique_ptr<detail::IAddressable> m_Addressable;
};

}
