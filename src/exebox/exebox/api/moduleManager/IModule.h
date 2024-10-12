#pragma once

#include "exebox/api/types/ApiString.h"


namespace exebox
{

class IModule
{
public:
	virtual ~IModule() = default;

	/**
		Returns the name of the module.
	 */
	[[nodiscard]] virtual ApiString name() const = 0;

	/**
		Gets the address of a function using its name.
	 */
	[[nodiscard]] virtual void *getFunction(ApiString name) const = 0;
	[[nodiscard]] virtual void *getFunction(int ordinal) const = 0;

	/**
		Returns the address of the module.
	 */
	[[nodiscard]] virtual void *address() = 0;
};

}
