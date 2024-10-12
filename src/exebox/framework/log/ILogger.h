#pragma once

#include <string>
#include <chrono>

namespace framework::log {

enum class Severity
{
	Info,
	Error,
};

using Time = std::chrono::time_point<std::chrono::system_clock>;

/**
	Interface for logging
 */
class ILogger
{
public:
	virtual ~ILogger() = default;

	/**
		Writes a log message.

		@param severity - The severity of the log.
		@param message - The log message.
		@param time - The time the message was logged.
	 */
	virtual void write(Severity severity, const std::string& message, Time time) = 0;
};

}
