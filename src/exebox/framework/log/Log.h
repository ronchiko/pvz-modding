#pragma once

#include <ostream>

#include "framework/log/LogMessage.h"
#include "framework/log/LogPublisher.h"


namespace framework::log {

/**
	Writes an info log
 */
LogMessage info();

/**
	Writes an error log
 */
LogMessage error();

}

// ReSharper disable once CppUnusedIncludeDirective
#include "framework/log/PublisherExports.h"
