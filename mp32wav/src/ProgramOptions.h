#pragma once

#include <vector>
#include <string>
#include <tuple>

class ProgramOptions
{
	enum OptionField
	{
		kOption,
		kArgument
	};

	using Options = std::vector<std::tuple<std::string, std::string>>;

public:
	enum ArgumentType {
		kNoArgument,
		kRequiredArgument,
		kOptionalArgument
	};

	struct OptionDescription
	{
		std::string name;
		ArgumentType type;
	};

public:
	ProgramOptions() = default;
	~ProgramOptions() = default;

	void parseCommandLine(int argc, char* argv[], const std::vector<OptionDescription>& acceptableOptions);

	bool isOptionPresent(const std::string& option) const;
	std::string getOptionArgument(const std::string& option) const;

private:
	Options::const_iterator getOptionIterator(const std::string& option) const;
	void checkPreviousOption(const std::string& option, const std::string& argument, ArgumentType argumentType);

private:
	Options _options;
};
