#pragma once

#include "framework/log/ILogger.h"

namespace exebox::log {

/**
	A logger that is used for the command line, printing to cout every log.
 */
class CliLogger final : public framework::log::ILogger
{
public:
	void write(framework::log::Severity severity, const std::string& message, framework::log::Time time) override;
};

}
