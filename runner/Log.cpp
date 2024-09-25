#include <iostream>

#include "runner/Log.h"

namespace runner::log {

namespace {

std::ostream& g_LogStream = std::cout;

}

std::ostream& info()
{
	return g_LogStream << "[INFO]: ";
}

std::ostream& error()
{
	return g_LogStream << "[ERROR]: ";
}

}
