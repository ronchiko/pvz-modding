#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "runner/runtime/HaltedExecutable.h"
#include "runner/runtime/hook/ForwardHook.h"


constexpr runner::runtime::hook::FixedAddress ERROR_CODE_FUNCTION = 0x00405299;


std::vector<std::string> loadCommandLineArguments(const int argc, char** argv) {
	std::vector<std::string> arguments;
	arguments.reserve(argc);

	for(int i = 0; i < argc; ++i) {
		arguments.emplace_back(argv[i]);
	}

	return arguments;
}

int dropErrorCodeCheck(void*)
{
	// Hook to stop the game from checking the activation error codes.

	std::cout << "Hook called!" << std::endl;
	return 1;
}

int main(int argc, char** argv) 
{
	try {
		const auto arguments = loadCommandLineArguments(argc, argv);
		const auto& exePath = arguments[1];

		{
			// Prepare the runtime!
			auto runtime = runner::runtime::createHaltedRuntime(exePath);
			runner::runtime::hook::ForwardHook hook(ERROR_CODE_FUNCTION, dropErrorCodeCheck);
		}

	} catch(const std::exception& ex) {
		std::cerr << "[ERROR] Unexpected failure: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
