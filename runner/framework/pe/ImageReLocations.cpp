#include "runner/framework/pe/ImageReLocations.h"

#include <stdexcept>

namespace framework::pe {

ImageReLocations::Iterator::Iterator(detail::ReLocationBlock *block, const size_t offset, const ImageReLocations *parent)
	: m_Parent(parent)
	, m_CurrentBlock(block)
	, m_CurrentReLocation(offset)
{}

size_t detail::ReLocationBlock::relocationsCount() const
{
	return (SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(short);
}

std::pair<ReLocationMode, uint16_t> detail::ReLocationBlock::at(const size_t offset) const
{
	const auto *relocations = reinterpret_cast<const uint8_t *>(this) + sizeof(IMAGE_BASE_RELOCATION);
	const auto relocationInfo = relocations[offset];

	return std::make_pair(static_cast<ReLocationMode>(relocationInfo >> 12), relocationInfo & 0xFFF);
}

ImageReLocations::Iterator& ImageReLocations::Iterator::operator++()
{
	if(m_CurrentBlock->relocationsCount() > m_CurrentReLocation) {
		++m_CurrentReLocation;
		return *this;
	}

	++m_CurrentBlock;
	m_CurrentReLocation = 0;
	return *this;
}

ReLocation ImageReLocations::Iterator::operator*() const
{
	const auto [type, offset] = m_CurrentBlock->at(m_CurrentReLocation);
	return ReLocation{
		static_cast<ReLocationMode>(type),
		reinterpret_cast<uint32_t *>(m_Parent->m_ImageBase + m_CurrentBlock->VirtualAddress + offset),
	};
}

bool ImageReLocations::Iterator::operator==(const Sentinel&) const
{
	return m_CurrentBlock->VirtualAddress <= 0;
}

bool ImageReLocations::Iterator::operator!=(const Sentinel&) const
{
	return m_CurrentBlock->VirtualAddress > 0;
}

ImageReLocations::ImageReLocations(uint8_t *imageBase)
	: m_ImageBase(imageBase)
{
	const PeHeader header(imageBase);
	const auto directory = header.getDataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC);
	if(!directory.has_value()) {
		throw std::runtime_error("Image has no re-location directory");
	}

	m_FirstRelocation = reinterpret_cast<detail::ReLocationBlock*>(imageBase + directory->VirtualAddress);
}

ImageReLocations::Iterator ImageReLocations::begin() const
{
	return Iterator(m_FirstRelocation, 0, this);
}

ImageReLocations::Sentinel ImageReLocations::end()
{
	return Sentinel{};
}

}
