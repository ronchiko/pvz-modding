#pragma once

#include <sstream>

#include "framework/log/ILogger.h"

namespace framework::log {

class LogMessage
{
public:
	explicit LogMessage(Severity severity, Time time = Time::clock::now());

	LogMessage(const LogMessage&) = delete;
	LogMessage(LogMessage&&) = default;

	LogMessage& operator=(const LogMessage&) = delete;
	LogMessage& operator=(LogMessage&&) = default;

	~LogMessage();

	template<typename T>
	LogMessage& operator<<(const T& other)
	{
		m_MessageStream << other;
		return *this;
	}

private:
	Severity m_Severity;
	Time m_Time;
	std::stringstream m_MessageStream;
};

}
