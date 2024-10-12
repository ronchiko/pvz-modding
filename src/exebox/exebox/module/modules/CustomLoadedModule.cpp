#include <fstream>

#include "framework/Exception.h"
#include "framework/pe/ImageImportDescriptors.h"
#include "framework/pe/PeHeader.h"

#include "exebox/module/modules/CustomLoadedModule.h"

namespace exebox {

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

	(*entry)();
}

void *CustomLoadedModule::address()
{
	return m_ModuleMemory.address();
}

ApiString CustomLoadedModule::name() const
{
	return m_Name;
}

void *CustomLoadedModule::getFunction(ApiString name) const
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
