#include <stdexcept>

#include "runner/framework/pe/PeHeader.h"

namespace framework::pe {

namespace {
/**
	Returns the nt headers of an image.

	@param content - The content of the image.
*/
const IMAGE_NT_HEADERS* getImageNtHeaders(const void *content)
{
	const auto& dosHeader = *static_cast<const IMAGE_DOS_HEADER *>(content);
	if(IMAGE_DOS_SIGNATURE != dosHeader.e_magic) {
		throw std::runtime_error("Module has bad DOS signature");
	}

	const auto ntHeaders = reinterpret_cast<const IMAGE_NT_HEADERS32*>(static_cast<const char *>(content) + dosHeader.e_lfanew);
	if(IMAGE_NT_SIGNATURE != ntHeaders->Signature) {
		throw std::runtime_error("Module has bad NT signature");
	}

	return ntHeaders;
}

}

PeHeader::PeHeader(const uint8_t *moduleBase)
	: m_ModuleBase(moduleBase)
	, m_NtHeaders(getImageNtHeaders(moduleBase))
{}

const IMAGE_NT_HEADERS& PeHeader::header() const
{
	return *m_NtHeaders;
}

std::optional<IMAGE_DATA_DIRECTORY> PeHeader::getDataDirectory(const uint32_t directoryIndex) const
{
	const auto& directory = m_NtHeaders->OptionalHeader.DataDirectory[directoryIndex];
	if(directory.Size <= 0) {
		return {};
	}

	return directory;
}

std::span<PeHeader::Section> PeHeader::sections() const
{
	const auto section = IMAGE_FIRST_SECTION(m_NtHeaders);
	return { section, section + m_NtHeaders->FileHeader.NumberOfSections };
}

}
