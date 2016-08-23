#include <UnitTest++/UnitTest++.h>
#include "../src/ProgramOptions.h"
#include <exception>

TEST(ProgramOptionsConstruction)
{
	ProgramOptions options;

	char* argv[] = {
	    "<app>",
	    "--help"
	};

	options.parseCommandLine(2, argv, {});
	CHECK(!options.isOptionPresent("help"));
	CHECK(options.getOptionArgument("help").empty());

	options.parseCommandLine(2, argv, {{"help", ProgramOptions::kNoArgument}});
	CHECK(options.isOptionPresent("help"));
	CHECK(options.getOptionArgument("help").empty());
	CHECK(options.getOptionArgument("blahblah").empty());


	char* argv1[] = {
	    "<app>",
	    "--help",
	    "--input",
	    "fileName"
	};

	options.parseCommandLine(4, argv1, {
	                             {"help", ProgramOptions::kNoArgument},
	                             {"input", ProgramOptions::kRequiredArgument}
	                         });
	CHECK(options.isOptionPresent("help"));
	CHECK(options.isOptionPresent("input"));
	CHECK(options.getOptionArgument("help").empty());
	CHECK(!options.getOptionArgument("input").empty());
	CHECK_EQUAL(std::string("fileName"), options.getOptionArgument("input"));

	char* argv2[] = {
	    "<app>",
	    "--input",
	    "fileName1",
	    "--output",
	    "fileName2"
	};

	options.parseCommandLine(5, argv2, {
	                             {"help", ProgramOptions::kNoArgument},
	                             {"input", ProgramOptions::kRequiredArgument},
	                              {"output", ProgramOptions::kRequiredArgument},
	                         });
	CHECK(!options.isOptionPresent("help"));
	CHECK(options.isOptionPresent("input"));
	CHECK(!options.getOptionArgument("input").empty());
	CHECK_EQUAL(std::string("fileName1"), options.getOptionArgument("input"));
	CHECK(options.isOptionPresent("output"));
	CHECK(!options.getOptionArgument("output").empty());
	CHECK_EQUAL(std::string("fileName2"), options.getOptionArgument("output"));

	char* argv3[] = {
	    "<app>",
	    "-i",
	    "fileName1",
	    "-o",
	    "fileName2"
	};

	options.parseCommandLine(5, argv3, {
	                             {"help", ProgramOptions::kNoArgument},
	                             {"input", ProgramOptions::kRequiredArgument},
	                              {"output", ProgramOptions::kRequiredArgument},
	                         });
	CHECK(!options.isOptionPresent("help"));
	CHECK(options.isOptionPresent("input"));
	CHECK(!options.getOptionArgument("input").empty());
	CHECK_EQUAL(std::string("fileName1"), options.getOptionArgument("input"));
	CHECK(options.isOptionPresent("output"));
	CHECK(!options.getOptionArgument("output").empty());
	CHECK_EQUAL(std::string("fileName2"), options.getOptionArgument("output"));
}

TEST(ProgramOptionsTestWrongArgs)
{
	ProgramOptions options;

	char* argv1[] = {
	    "<app>",
	    "--help",
	    "someOne"
	};

	try
	{
		options.parseCommandLine(3, argv1, {{"help", ProgramOptions::kNoArgument}});
		CHECK(false);
	}
	catch (std::exception e)
	{
		CHECK(true);
	}


	char* argv2[] = {
	    "<app>",
	    "--input",
	    "--help"
	};

	try
	{
		options.parseCommandLine(3, argv2, {
		                             {"help", ProgramOptions::kNoArgument},
		                             {"input", ProgramOptions::kRequiredArgument}
		                         });
		CHECK(false);
	}
	catch (std::exception e)
	{
		CHECK(true);
	}
}
