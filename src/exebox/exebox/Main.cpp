#include "exebox/cli/ExeBox.h"


int main(const int argc, char** argv)
{
	std::vector<std::string> arguments;
	arguments.reserve(argc);
	for(int i = 0; i < argc; ++i)
	{
		arguments.emplace_back(argv[i]);
	}

	exebox::main(arguments);
}
