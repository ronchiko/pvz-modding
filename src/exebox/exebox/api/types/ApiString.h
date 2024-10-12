#pragma once

#include <string>

namespace exebox {

struct ApiString
{
	const char *data;
	size_t length;

	ApiString(const std::string& content)
		: data(content.data())
		, length(content.length())
	{}

	ApiString(std::string_view view)
		: data(view.data())
		, length(view.length())
	{}

	operator std::string_view() const
	{
		return std::string_view(data, length);
	}

	operator std::string() const
	{
		return std::string(data, length);
	}

	friend bool operator==(const std::string& lhs, const ApiString& rhs)
	{
		return lhs == std::string_view(rhs.data, rhs.length);
	}
};

}