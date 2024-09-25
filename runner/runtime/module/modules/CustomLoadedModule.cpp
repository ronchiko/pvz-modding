#include <fstream>

#include "runner/framework/Exception.h"
#include "runner/framework/pe/ImageImportDescriptors.h"
#include "runner/framework/pe/PeHeader.h"

#include "CustomLoadedModule.h"

namespace runner::runtime {

namespace {

using ExeEntryProc = void(WINAPI *)();

}

CustomLoadedModule::CustomLoadedModule(framework::VirtualMemory moduleMemory, std::string name)
	: m_Name(std::move(name))
	, m_ModuleMemory(std::move(moduleMemory))
{}

void CustomLoadedModule::run()
{
	const framework::pe::PeHeader header(m_ModuleMemory.address());
	if(0 == header->OptionalHeader.AddressOfEntryPoint) {
		throw std::runtime_error("Not entry point to module");
	}

	const auto entry = reinterpret_cast<ExeEntryProc>(m_ModuleMemory.address() +
													  header->OptionalHeader.AddressOfEntryPoint);
	if(nullptr == entry) {
		throw std::runtime_error("Not entry point to module");
	}

	__debugbreak();
	(*entry)();
}

uint8_t *CustomLoadedModule::address()
{
	return m_ModuleMemory.address();
}

const uint8_t *CustomLoadedModule::address() const
{
	return m_ModuleMemory.address();
}

const std::string& CustomLoadedModule::name() const
{
	return m_Name;
}

void *CustomLoadedModule::getFunction(const std::string& name) const
{
	// TODO: Import function

	return nullptr;
}

void* CustomLoadedModule::getFunction(int ordinal) const
{
	// TODO: Import function

	return nullptr;
}

}
