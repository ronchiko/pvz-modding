#include "framework/Exception.h"

#include "framework/io/HModule.h"

namespace framework {

namespace detail {

void HModuleDeleter::operator()(HMODULE module) const
{
	if(nullptr == module) {
		return;
	}

	FreeLibrary(module);
}

}

HModule::HModule(HMODULE module)
	: m_Storage(module, {})
{}

HModule HModule::loadLibrary(const std::filesystem::path& libraryPath)
{
	const auto module = LoadLibraryW(libraryPath.c_str());
	if(nullptr == module) {
		throw WindowsException("Failed to load library");
	}

	return HModule(module);
}


}