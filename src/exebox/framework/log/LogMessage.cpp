#include "framework/log/LogPublisher.h"

#include "framework/log/LogMessage.h"

namespace framework::log {

LogMessage::LogMessage(const Severity severity, const Time time)
	: m_Severity(severity)
	, m_Time(time)
{}

LogMessage::~LogMessage()
{
	const auto message = m_MessageStream.str();
	if(message.length() <= 0) {
		return;
	}

	publisher::writeLog(m_Severity, message, m_Time);
}

}