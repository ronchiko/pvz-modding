#pragma once

#include <Windows.h>

#include "framework/pe/PeHeader.h"

namespace framework::pe {

enum class ReLocationMode
{
	Absolute,
	HighLow,
};

struct ReLocation
{
	ReLocationMode mode;
	uint32_t *destination;
};

namespace detail {

struct ReLocationBlock final : IMAGE_BASE_RELOCATION
{
	/**
		Returns the amount of relocations in a block.
	 */
	[[nodiscard]] size_t relocationsCount() const;

	/**
		Returns a relocation from this block at an offset.
	 */
	std::pair<ReLocationMode, uint16_t> at(size_t offset) const;
};

}

class ImageReLocations
{
public:
	class Sentinel {};

	class Iterator
	{
	public:
		explicit Iterator(detail::ReLocationBlock *block, size_t offset, const ImageReLocations* parent);

		Iterator& operator++();

		ReLocation operator*() const;

		bool operator==(const Sentinel& iterator) const;
		bool operator!=(const Sentinel& iterator) const;

	private:
		const ImageReLocations *m_Parent;
		detail::ReLocationBlock *m_CurrentBlock;
		size_t m_CurrentReLocation;
	};

	explicit ImageReLocations(uint8_t *imageBase);

	[[nodiscard]] Iterator begin() const;
	[[nodiscard]] static Sentinel end();

private:
	uint8_t *m_ImageBase;
	detail::ReLocationBlock *m_FirstRelocation;
};

}
