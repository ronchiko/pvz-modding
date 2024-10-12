#pragma once

#include <Windows.h>

#include "framework/pe/ImportedFunction.h"


namespace framework::pe {

namespace detail {


struct ThunkIterator
{
	DWORD *thunk;
	DWORD *function;

	/**
		Advances the iteration over the thunks.
	 */
	ThunkIterator& operator++();

	/**
		Returns true if the iteration over the thunks is over.
	 */
	[[nodiscard]] bool isOver() const;
};

}

struct ImportThunk
{
	ImportedFunction function;
	uintptr_t *functionRef;
};

class ImageImportDescriptors
{
public:
	using Descriptor = IMAGE_IMPORT_DESCRIPTOR;

	class Sentinel
	{};

	class Iterator
	{
	public:
		explicit Iterator(Descriptor *descriptor, detail::ThunkIterator thunkIterator, const ImageImportDescriptors *parent);

		Iterator& operator++();

		ImportThunk operator*() const;

		bool operator==(const Sentinel&) const;
		bool operator!=(const Sentinel&) const;

	private:
		Descriptor *m_CurrentDescriptor = nullptr;
		detail::ThunkIterator m_ThunkIterator;

		const ImageImportDescriptors *m_Parent;
	};

	explicit ImageImportDescriptors(void *module);

	[[nodiscard]] Iterator begin() const;
	[[nodiscard]] static Sentinel end();

private:
	[[nodiscard]] detail::ThunkIterator iterateDescriptorThunk(const Descriptor& descriptor) const;

	uint8_t *m_Module;
	Descriptor *m_InitialDescriptor = nullptr;
};

}
