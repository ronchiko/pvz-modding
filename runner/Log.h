#pragma once

#include <ostream>


namespace runner::log
{

/**
	Writes an info log
 */
std::ostream& info();

/**
	Writes an error log
 */
std::ostream& error();

}
