#include <iostream>
#include <unordered_map>

#include "exebox/cli/log/CliLogger.h"

namespace exebox::log {
namespace {

const std::unordered_map<framework::log::Severity, std::string> SEVERITY_NAMES = {
	{ framework::log::Severity::Info, "INFO" },
	{ framework::log::Severity::Error, "ERROR" },
};
}

void CliLogger::write(const framework::log::Severity severity, const std::string& message, framework::log::Time)
{
	std::cout << "[" << SEVERITY_NAMES.at(severity) << "]: " << message << std::endl;

}

}
