#include "ProgramOptions.h"
#include <algorithm>

void ProgramOptions::parseCommandLine(int argc, char* argv[], const std::vector<ProgramOptions::OptionDescription>& acceptableOptions)
{
	_options.clear();

	std::string previousOptionName;
	ArgumentType previousArgumentType = kNoArgument;

	for (int index = 1; index < argc; ++index)
	{
		std::string argument(argv[index]);
		bool isOption = false;
		if (argument.find("--") == 0 && argument.size() > 2)
		{
			isOption = true;
			argument.erase(0, 2);
		}
		else if (argument.find("-") == 0 && argument.size() == 2)
		{
			isOption = true;
			argument.erase(0, 1);
		}

		if (isOption)
		{
			checkPreviousOption(previousOptionName, std::string(), previousArgumentType);
			previousOptionName.clear();
			previousArgumentType = kNoArgument;

			const auto optionNameMatch = [argument](const auto& desc) -> bool
			{
				return argument == desc.name || argument == (std::string() + desc.name[0]);
			};
			std::vector<OptionDescription>::const_iterator i = std::find_if(std::begin(acceptableOptions), std::end(acceptableOptions), optionNameMatch);
			if (i != std::end(acceptableOptions))
			{
				previousOptionName = i->name;
				previousArgumentType = i->type;
			}
		}
		else
		{
			checkPreviousOption(previousOptionName, argument, previousArgumentType);
			previousOptionName.clear();
			previousArgumentType = kNoArgument;
		}
	}

	checkPreviousOption(previousOptionName, std::string(), previousArgumentType);
}

bool ProgramOptions::isOptionPresent(const std::string& option) const
{
	return getOptionIterator(option) != std::end(_options);
}

std::string ProgramOptions::getOptionArgument(const std::string& option) const
{
	Options::const_iterator i = getOptionIterator(option);
	if (i != std::end(_options))
	{
		return std::get<kArgument>(*i);
	}
	return std::string();
}

ProgramOptions::Options::const_iterator ProgramOptions::getOptionIterator(const std::string& option) const
{
	const auto optionNameMatch = [option](const auto& optionPair) -> bool
	{
		return option == std::get<kOption>(optionPair);
	};
	return std::find_if(std::begin(_options), std::end(_options), optionNameMatch);
}

void ProgramOptions::checkPreviousOption(const std::string& option, const std::string& argument, ArgumentType argumentType)
{
	if (!option.empty())
	{
		if (argument.empty())
		{
			switch (argumentType)
			{
			case kNoArgument: // fall thru
			case kOptionalArgument:
				_options.push_back(std::make_tuple(option, argument));
				break;

			case kRequiredArgument:
				throw std::invalid_argument(std::string("Command line option '") + option + "' requires argument");
				break;
			}
		}
		else
		{
			switch (argumentType)
			{
			case kNoArgument:
				throw std::invalid_argument(std::string("Invalid command line argument: ") + argument);
				break;

			case kRequiredArgument: // fall thru
			case kOptionalArgument:
				_options.push_back(std::make_tuple(option, argument));
				break;
			}
		}
	}
}
