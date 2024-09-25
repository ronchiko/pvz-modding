#include "runner/runtime/module/modules/NativeModule.h"

#include <filesystem>

namespace runner::runtime {

namespace {

std::string getModuleName(const HMODULE module)
{
	CHAR fileName[MAX_PATH];
	const auto actualLength = GetModuleFileNameA(module, fileName, MAX_PATH);
	const std::filesystem::path path(std::string(fileName, actualLength));

	return path.stem().string();
}

}

FailedToLoadNativeModuleException::FailedToLoadNativeModuleException(const std::string& moduleName)
	: runtime_error(createMessage(moduleName))
{}

std::string FailedToLoadNativeModuleException::createMessage(const std::string& moduleName)
{
	std::stringstream stream;
	stream << "Failed to load module: " << moduleName;
	return stream.str();
}

NativeModule::NativeModule(const HMODULE module)
	: m_Module(module)
	, m_Name(getModuleName(module))
{}

const std::string& NativeModule::name() const
{
	return m_Name;
}

void *NativeModule::getFunction(const std::string& name) const
{
	return reinterpret_cast<void*>(GetProcAddress(m_Module, name.c_str()));
}

void* NativeModule::getFunction(const int ordinal) const
{
	return reinterpret_cast<void *>(GetProcAddress(m_Module, MAKEINTRESOURCEA(ordinal)));
}

}
