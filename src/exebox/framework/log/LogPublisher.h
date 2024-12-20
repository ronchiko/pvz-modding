#pragma once

#include <span>

#include "framework/log/ILogger.h"

namespace framework::log::publisher {

/**
	Adds a logger to publish logs to.
 */
void addLogger(ILogger& logger);

/**
	Removes a logger from the publisher.
 */
void removeLogger(ILogger& logger);

/**
	Returns all the registered loggers.
 */
std::span<std::reference_wrapper<ILogger>> getAllRegisteredLoggers();

/**
	Publishes a log message to all the registered loggers.

	@param severity - The severity of the logs.
	@param message - The message to write.
	@param time - When the log happen.
 */
void writeLog(Severity severity, const std::string& message, Time time);

}
