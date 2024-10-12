#pragma once

#include <ostream>

#include "framework/log/LogMessage.h"


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
