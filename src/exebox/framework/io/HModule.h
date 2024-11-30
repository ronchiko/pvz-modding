#pragma once

#include <filesystem>
#include <memory>

#include <Windows.h>

namespace framework {

namespace detail {

struct HModuleDeleter
{
	void operator()(HMODULE module) const;
};

}

/**
	Represents a classic windows module.
 */
class HModule
{
public:
	using Storage_t = std::unique_ptr<std::remove_pointer_t<HMODULE>, detail::HModuleDeleter>;

	explicit HModule(HMODULE module);

	/**
		Loads a library using its path.
	 */
	static HModule loadLibrary(const std::filesystem::path& libraryPath);

	/**
		Returns the address of a function is it exists.
	 */
	template<typename FunctionT>
	std::optional<FunctionT *> getProcedure(const std::string& name);

private:
	Storage_t m_Storage;
};

template<typename FunctionT>
std::optional<FunctionT *> HModule::getProcedure(const std::string& name)
{
	auto procedure = GetProcAddress(m_Storage.get(), name.c_str());
	if(nullptr == procedure) {
		return {};
	}

	return reinterpret_cast<FunctionT *>(procedure);
}

}
