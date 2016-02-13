/*
 * Copyright (C) 2014-2015  absurdworlds
 * Copyright (C) 2016       Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/utility/ArgumentParser.h>
namespace daedalus {
namespace {
// Dummy variable for pointer-to-null initialization
char const dummy = 0;
}

ArgumentParser::ArgumentParser(char const* const* argv)
	: argv(argv), args(&dummy)
{
}

Argument ArgumentParser::nextArg(char const* arg)
{
	using namespace std::string_literals;

	if (*arg != '-')
		return std::string(arg);

	++arg; // Eat '-'

	Argument tok;

	switch (*arg) {
	case 0:
		return "-"s;
	case '-':
		++arg;
		if (*arg == 0) {
			tok.type = Argument::Delim;
			tok.name = "--";
		} else {
			tok.type = Argument::Option;
			tok.name = std::string(arg);
		}
		break;
	default:
		tok.type = Argument::Option;
		tok.name = *(arg++);
		args = arg;
	}

	return tok;
}

opt<Argument> ArgumentParser::parseArgument()
{
	if (*args != 0) {
		Argument tok;
		tok.type = Argument::Option;
		tok.name = *(args++);
		return tok;
	}

	if (*argv == 0)
		return nullopt;

	return nextArg(*argv++);
}

std::string ArgumentParser::getParam()
{
	if (*args != 0) {
		std::string tmp(args);
		args = &dummy; // reset 'args'
		return tmp;
	}

	if (*argv == 0)
		return "";

	return std::string(*argv++);
}
} //namespace daedalus
