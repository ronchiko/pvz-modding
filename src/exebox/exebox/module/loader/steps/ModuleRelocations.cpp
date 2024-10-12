#include "framework/log/Log.h"
#include "framework/pe/ImageReLocations.h"
#include "framework/pe/PeHeader.h"


#include "exebox/module/loader/steps/ModuleRelocations.h"

namespace exebox::module::loader {

namespace {

void relocateImage(uint8_t *imageBase, const int32_t delta)
{
	for(const auto relocation : framework::pe::ImageReLocations(imageBase)) {
		switch(relocation.mode) {
		case framework::pe::ReLocationMode::Absolute:
			break;
		case framework::pe::ReLocationMode::HighLow:
			*relocation.destination += delta;

		default:
			framework::log::error() << "Unknown relocation type encountered: " << static_cast<int>(relocation.mode);
			throw std::runtime_error("Unknown relocation type");
		}
	}
}

}

void relocateImage(framework::VirtualMemoryView& image)
{
	const framework::pe::PeHeader header(image.address());
	const auto delta = reinterpret_cast<intptr_t>(image.address()) - header->OptionalHeader.ImageBase;
	if(0 == delta) {
		return;
	}

	try {
		relocateImage(image.address(), delta);
	} catch(const std::exception& ex) {
		framework::log::error() << "Failed to relocate image. Error: " << ex.what();
		throw;
	}
}

}