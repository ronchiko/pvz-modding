#pragma once

#include <stdexcept>
#include <sstream>

#include <Windows.h>


namespace framework {

class WindowsException final : public std::runtime_error
{
public:
	explicit WindowsException(const std::string& message, const uint32_t errorCode = GetLastError())
		: runtime_error(createMessageString(message, errorCode))
	{}

private:

	static std::string createMessageString(const std::string& message, const uint32_t errorCode)
	{
		std::stringstream stream;
		stream << message << ". Error Code: " << errorCode;

		return stream.str();
	}
};

}