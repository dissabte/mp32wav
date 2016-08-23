#include <iostream>
#include <lame.h>
#include <libgen.h>
#include "ProgramOptions.h"

static void printUsage(const std::string& name)
{
	std::cout << name << std::endl
	          << "Usage: " << name << " [OPTIONS] FILES\n"
	          << "Options:\n"
	          << "\t-h,--help\t\tShow this message\n"
	          << "Params:\n"
	          << "\t-i,--input FILE\t\tSpecifies input FILE path\n"
	          << "\t-o,--ouput FILE\t\tSpecifies ouput FILE path\n"
	          << std::endl;
}

static const std::string kHelp = "help";
static const std::string kInput = "input";
static const std::string kOutput = "output";

int main(int argc, char* argv[])
{
	ProgramOptions programOptions;
	try
	{
		programOptions.parseCommandLine(argc, argv, {
		                                    {kHelp, ProgramOptions::kNoArgument}
		                                    , {kInput, ProgramOptions::kRequiredArgument}
		                                    , {kOutput, ProgramOptions::kRequiredArgument}});
	}
	catch (std::exception e)
	{
		std::cerr << e.what();
		return 1;
	}

	const bool helpIsPresent = programOptions.isOptionPresent(kHelp);
	const bool inputIsPresent = programOptions.isOptionPresent(kInput);
	const bool outputIsPresent = programOptions.isOptionPresent(kOutput);

	if (helpIsPresent || !inputIsPresent || !outputIsPresent)
	{
		printUsage(basename(argv[0]));
	}
	else
	{
		const std::string inputFileName = programOptions.getOptionArgument(kInput);
		const std::string outputFileName = programOptions.getOptionArgument(kOutput);
		std::cout << "INPUT:" << inputFileName << ", OUTPUT:" << outputFileName << std::endl;
	}

	return 0;
}
