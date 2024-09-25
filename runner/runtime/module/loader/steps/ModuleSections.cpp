#include "runner/framework/pe/PeHeader.h"

#include "runner/runtime/module/loader/steps/ModuleSections.h"

namespace runner::runtime::moduleLoader {

namespace {

constexpr int PROTECTION_FLAGS[2][2][2] = {
	{
		// not executable
		{ PAGE_NOACCESS, PAGE_WRITECOPY },
		{ PAGE_READONLY, PAGE_READWRITE },
	},
	{
		// executable
		{ PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY },
		{ PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE },
	},
};

/**
	Returns the desired protections of a section.
 */
uint32_t getSectionProtections(const framework::pe::PeHeader::Section& section)
{
	const bool isExecutable = section.Characteristics & IMAGE_SCN_MEM_EXECUTE;
	const bool isReadable = section.Characteristics & IMAGE_SCN_MEM_READ;
	const bool isWritable = section.Characteristics & IMAGE_SCN_MEM_WRITE;

	auto protection = PROTECTION_FLAGS[isExecutable][isReadable][isWritable];
	if(IMAGE_SCN_MEM_NOT_CACHED & section.Characteristics) {
		protection |= PAGE_NOCACHE;
	}

	return protection;
}

/**
	Returns the appropriate size of a section.
 */
uint32_t getSectionSize(const framework::pe::PeHeader::Section& section, const framework::pe::PeHeader& header)
{
	const auto size = section.SizeOfRawData;
	if(size != 0) {
		return size;
	}

	if(section.Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA) {
		return header->OptionalHeader.SizeOfInitializedData;
	}

	if(section.Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) {
		return header->OptionalHeader.SizeOfUninitializedData;
	}

	return size;
}

}

void initializeImageSections(framework::VirtualMemoryView& moduleBase, const uint8_t *imageRef)
{
	const framework::pe::PeHeader header(moduleBase.address());
	const auto sectionAlignment = header->OptionalHeader.SectionAlignment;

	for(auto& section : header.sections()) {
		auto sectionStart = moduleBase.address() + section.VirtualAddress;
		if(0 == section.SizeOfRawData && sectionAlignment > 0) {
			std::memset(sectionStart, 0, sectionAlignment);
			section.Misc.PhysicalAddress = reinterpret_cast<DWORD>(sectionStart);

			continue;
		}

		std::memcpy(sectionStart, imageRef + section.PointerToRawData, section.SizeOfRawData);
		section.Misc.PhysicalAddress = reinterpret_cast<DWORD>(sectionStart);
	}
}

void finalizeImageSections(framework::VirtualMemoryView& moduleBase)
{
	const framework::pe::PeHeader header(moduleBase.address());
	for(const auto section : header.sections()) {
		if(IMAGE_SCN_MEM_DISCARDABLE & section.Characteristics) {
			continue;
		}

		const auto protection = getSectionProtections(section);
		if(const auto sectionSize = getSectionSize(section, header); sectionSize > 0) {
			auto sectionDataView = moduleBase.subView(section.VirtualAddress, section.SizeOfRawData);
			sectionDataView.setProtection(protection);
		}
	}
}

}
