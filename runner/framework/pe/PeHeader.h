#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <Windows.h>


namespace framework::pe {

class PeHeader
{
public:
	using Section = IMAGE_SECTION_HEADER;

	explicit PeHeader(const uint8_t *moduleBase);

	/**
		Returns the nt image header.
	 */
	[[nodiscard]] const IMAGE_NT_HEADERS& header() const;

	constexpr const IMAGE_NT_HEADERS *operator->() const;

	/**
		Gets a data directory if its valid.
	 */
	[[nodiscard]] std::optional<IMAGE_DATA_DIRECTORY> getDataDirectory(uint32_t directoryIndex) const;

	/**
		Returns a vector containing all the sections in the PE.
	 */
	std::span<Section> sections() const;

private:
	const uint8_t *m_ModuleBase;
	const IMAGE_NT_HEADERS* m_NtHeaders;
};

constexpr const IMAGE_NT_HEADERS* PeHeader::operator->() const
{
	return m_NtHeaders;
}

}
