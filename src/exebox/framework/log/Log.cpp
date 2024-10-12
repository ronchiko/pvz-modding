#include "framework/log/Log.h"

namespace framework::log {

LogMessage info()
{
	return LogMessage(Severity::Info);
}

LogMessage error()
{
	return LogMessage(Severity::Error);
}

}