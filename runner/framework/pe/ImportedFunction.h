#pragma once

#include <variant>
#include <string>


namespace framework::pe
{

struct ImportedFunction
{
	std::string importName;
	std::variant<int, std::string> function;
};


}