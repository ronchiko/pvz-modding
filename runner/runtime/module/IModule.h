#pragma once

#include <string>


namespace runner::runtime
{

class IModule
{
public:
	virtual ~IModule() = default;

	/**
		Returns the name of the module.
	 */
	[[nodiscard]] virtual const std::string& name() const = 0;

	/**
		Gets the address of a function using its name.
	 */
	[[nodiscard]] virtual void *getFunction(const std::string& name) const = 0;
	[[nodiscard]] virtual void *getFunction(int ordinal) const = 0;
};

}
