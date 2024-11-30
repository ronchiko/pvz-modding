#include "framework/log/LogPublisher.h"

namespace framework::log::publisher {

namespace {

std::vector<std::reference_wrapper<ILogger>> g_RegisteredLoggers;

}

void addLogger(ILogger& logger)
{
	const auto iterator = std::ranges::find_if(g_RegisteredLoggers, [&](const auto& possibleMatch) {
		return &possibleMatch.get() == &logger;
	});
	if(g_RegisteredLoggers.end() != iterator) {
		return;
	}

	g_RegisteredLoggers.push_back(std::ref(logger));
}

void removeLogger(ILogger& logger)
{
	const auto iterator = std::ranges::find_if(g_RegisteredLoggers, [&](const auto& possibleMatch) {
		return &possibleMatch.get() == &logger;
	});

	if(g_RegisteredLoggers.end() == iterator) {
		return;
	}

	g_RegisteredLoggers.erase(iterator);
}

std::span<std::reference_wrapper<ILogger>> getAllRegisteredLoggers()
{
	return g_RegisteredLoggers;
}

void writeLog(const Severity severity, const std::string& message, const Time time)
{
	for(auto& logger : g_RegisteredLoggers)
	{
		logger.get().write(severity, message, time);
	}
}


}
