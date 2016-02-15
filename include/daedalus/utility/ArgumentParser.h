/*
 * Copyright (C) 2014-2015  absurdworlds
 * Copyright (C) 2016       Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_ArgumentParser
#define Daedalus_ArgumentParser
#include <string>
#include <daedalus/common/types.h>
namespace daedalus {
/*!
 * Command line argument,
 * represents a single option or argument
 */
struct Argument {
	Argument()
		: type(Invalid)
	{ }

	Argument(std::string str)
		: type(Operand),
		  name(str)
	{ }

	/*!
	 * List of types of arguments:
	 */
	enum Type : u8 {
		//! Invalid argument (unused)
		Invalid,
		//! Option ('-o') or ('--option')
		Option,
		//! Argument or operand
		Operand,
		//! End of arguments delimiter: "--"
		Delim
	} type;

	std::string name;

	//! Returns true if Argument is GNU long option
	bool longOpt() const
	{
		return type == Option && name.size() == 1;
	}
};

/*!
 * Parses program arguments using POSIX syntax.
 */
class ArgumentParser {
public:
	ArgumentParser(char const* const* argv);

	/*!
	 * Get the next argument from the command line
	 *
	 * \return
	 *     An \a Argument object or \a nullopt in case
	 *     there is no more arguments.
	 */
	opt<Argument> parseArgument();

	/*!
	 * Returns either the next parameter, or the rest of
	 * current argument group. If there are no more
	 * arguments left, returns empty string
	 */
	std::string getParam();
private:
	Argument nextArg(char const* arg);

	char const* const* argv;
	// Needed to avoid modifying pointers in argv array
	char const* args;
};
} //namespace daedalus
#endif//Daedalus_ArgumentParser
