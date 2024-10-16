#include "framework/log/Log.h"
#include "framework/log/LogPublisher.h"

#include "exebox/cli/api/ExeBoxApi.h"
#include "exebox/cli/ExeBoxSettings.h"
#include "exebox/cli/log/CliLogger.h"
#include "exebox/module/HaltedExecutable.h"

#include "exebox/cli/ExeBox.h"

namespace exebox {

namespace {

/**
	Adds the loggers to the framework context so exebox can write logs.
 */
void registerLoggers()
{
	static std::unique_ptr<framework::log::ILogger> s_CliLogger = std::make_unique<log::CliLogger>();

	framework::log::publisher::addLogger(*s_CliLogger);
}

}

void main(const std::vector<std::string>& arguments)
{
	registerLoggers();
	api::initializeApi();

	framework::log::info() << "ExeBox is starting";
	const auto settings = parseSettings(arguments);

	try {
		auto runtime = createHaltedRuntime(settings.mainPath);
	} catch(const std::exception& e) {
		framework::log::error() << e.what();
	}
}

}
