#include <stdexcept>

#include "framework/pe/PeHeader.h"

#include "framework/pe/ImageImportDescriptors.h"


namespace framework::pe {

namespace
{

std::variant<int, std::string> getThunkImport(uint8_t* moduleBase, const DWORD* thunkRef)
{
	if(IMAGE_SNAP_BY_ORDINAL(*thunkRef)) {
		return static_cast<int>(IMAGE_ORDINAL32(*thunkRef));
	}

	const auto& importByName = *reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(moduleBase + *thunkRef);
	return std::string(importByName.Name);
}

}


detail::ThunkIterator& detail::ThunkIterator::operator++()
{
	++thunk;
	++function;

	return *this;
}

bool detail::ThunkIterator::isOver() const
{
	return !*thunk;
}

ImageImportDescriptors::Iterator::Iterator(
	Descriptor *descriptor,
	const detail::ThunkIterator thunkIterator,
	const ImageImportDescriptors *parent)
	: m_CurrentDescriptor(descriptor)
	, m_ThunkIterator(thunkIterator)
	, m_Parent(parent)
{}

ImageImportDescriptors::Iterator& ImageImportDescriptors::Iterator::operator++()
{
	++m_ThunkIterator;
	if(!m_ThunkIterator.isOver()) {

		// Advance the thunk iteration.
		return *this;
	}

	// If the thunk iteration is over, then move on to the next descriptor.

	++m_CurrentDescriptor;
	if(*this != Sentinel{}) {
		// Setup the next thunk iterator.
		m_ThunkIterator = m_Parent->iterateDescriptorThunk(*m_CurrentDescriptor);
	}

	return *this;
}

ImportThunk ImageImportDescriptors::Iterator::operator*() const
{
	const std::string moduleName(reinterpret_cast<char *>(m_Parent->m_Module + m_CurrentDescriptor->Name));
	return ImportThunk{
		ImportedFunction{
			moduleName,
			getThunkImport(m_Parent->m_Module, m_ThunkIterator.thunk)
		},
		reinterpret_cast<uintptr_t*>(m_ThunkIterator.function)
	};
}

bool ImageImportDescriptors::Iterator::operator==(const Sentinel&) const
{
	return IsBadReadPtr(m_CurrentDescriptor, sizeof(IMAGE_IMPORT_DESCRIPTOR)) || !m_CurrentDescriptor->Name;
}

bool ImageImportDescriptors::Iterator::operator!=(const Sentinel&) const
{
	return !operator==(Sentinel{});
}

ImageImportDescriptors::ImageImportDescriptors(void *module)
	: m_Module(static_cast<uint8_t *>(module))
	, m_InitialDescriptor(nullptr)
{
	const PeHeader header(m_Module);
	const auto directory = header.getDataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT);
	if(!directory.has_value()){
		throw std::runtime_error("Image has no import directory");
	}

	m_InitialDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(m_Module + directory->VirtualAddress);
}

ImageImportDescriptors::Iterator ImageImportDescriptors::begin() const
{
	return Iterator(m_InitialDescriptor, iterateDescriptorThunk(*m_InitialDescriptor), this);
}

ImageImportDescriptors::Sentinel ImageImportDescriptors::end()
{
	return Sentinel{};
}

detail::ThunkIterator ImageImportDescriptors::iterateDescriptorThunk(const Descriptor& descriptor) const
{
	if(descriptor.OriginalFirstThunk) {
		return detail::ThunkIterator{
			reinterpret_cast<PDWORD>(m_Module + descriptor.OriginalFirstThunk),
			reinterpret_cast<DWORD *>(m_Module + descriptor.FirstThunk),
		};
	}

	return detail::ThunkIterator{
		reinterpret_cast<PDWORD>(m_Module + descriptor.FirstThunk),
		reinterpret_cast<DWORD *>(m_Module + descriptor.FirstThunk),
	};
}

}
