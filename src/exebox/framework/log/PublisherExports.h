#pragma once


#include "framework/log/LogPublisher.h"

/**
	Automatic export for every DLL that uses the logger, to allow exebox to pass in its active loggers when
	loading the plugin.

	@param logger - The logger to add.
 */
extern "C" __declspec(dllexport)
inline void __cdecl logPublisher_addLogger(framework::log::ILogger *logger)
{
	if(nullptr == logger) {
		return;
	}

	framework::log::publisher::addLogger(*logger);
}
