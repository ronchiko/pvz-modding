#include "runner/Log.h"
#include "runner/framework/pe/PeHeader.h"
#include "runner/framework/pe/ImageReLocations.h"

#include "runner/runtime/module/loader/steps/ModuleRelocations.h"

namespace runner::runtime::moduleLoader {

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
			log::error() << "Unknown relocation type encountered: " << static_cast<int>(relocation.mode) << std::endl;
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
		log::error() << "Failed to relocate image. Error: " << ex.what() << std::endl;
	}
}

}