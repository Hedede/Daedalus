/*
 * Copyright (C) 2016 Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <daedalus/utility/string.h>
#include <daedalus/ou/Parser.h>
namespace daedalus {
namespace ou {
template <class UnaryPredicate>
bool Parser::advance_if(UnaryPredicate predicate)
{
	for (; cur != end; ++cur) {
		if (predicate(*cur))
			return true;
	}
	return false;
}

bool Parser::advance(char c)
{
	return advance_if([c] (char x) {return x == c;});
}

template <class Unary1, class Unary2>
bool Parser::advance_if(Unary1 trueCond, Unary2 falseCond)
{
	for (; cur != end; ++cur) {
		if (trueCond(*cur))
			return true;
		if (falseCond(*cur))
			return false;
	}
	return false;
}

bool Parser::advance(char c, char e)
{
	return advance_if([c] (char x) {return x == c;},
	              [e] (char x) {return x == e;});
}

bool Parser::advance(std::string str)
{
	assert(!str.empty());

	if (!advance(str[0], '\n'))
		return false;

	for (char c : str) {
		if (*cur++ != c) return false;
	}

	return true;
}

void Parser::skipComment()
{
	++ cur;
	if (*cur == '/') {
		advance('\n');
	} else if (*cur == '*') {
		while (true) {
			if (!advance('/'))
				break;

			char const* prev = cur++ - 1;
			if (*prev == '*')
				break;
		}
	}
}

void Parser::skipWhitespace()
{
	advance_if([] (char x) {return !isspace(x);});
}

std::string Parser::readWord()
{
	auto* start = cur++;

	advance_if([] (char x) {return !isalnum(x) && x != '_';});

	return std::string(start, cur);
}

std::string Parser::readString()
{
	++cur; // skip "
	std::string str;
	while (*cur && *cur != '"') {
		if (*cur == '\\')
			++cur;
		str += *cur++;
	}
	return str;
}

std::string Parser::readCommentText()
{
	auto* start = cur;
	advance('\n');
	return std::string(start, cur);
}

OutputUnitList Parser::loadOutputUnits()
{
	OutputUnitList ou;

	while (*cur) {
		if (isalpha(*cur)) {
			std::string id = readWord();
			string::tolower(id);

			if (id == "ai_output") {
				processAI_Output(ou);
			} else if (id == "instance") {
				processSVMInstance(ou);
			}
		} else if (isspace(*cur)) {
			skipWhitespace();
		} else if (*cur == '/') {
			skipComment();
		} else {
			++cur;
		}
	}

	return ou;
}

void Parser::processAI_Output(OutputUnitList& list)
{
	advance('(');
	advance(',');
	advance(',');

	if (!advance('"')) return;

	auto name = readString();

	advance(')');
	advance(';');

	if (!advance("//"))
		return;
	
	OutputUnit ou;
	ou.name = name;
	ou.subtitle = readCommentText();
	ou.soundFile = name + ".wav";
	list.insert(ou);
}

void Parser::processSVMInstance(OutputUnitList& list)
{
	if (!advance_if(isalpha)) return;
	advance('(');
	if (!advance("C_SVM")) return;
	advance(')');
	if (!advance('{')) return;
	
	while (*cur) {
		if (isspace(*cur))
			skipWhitespace();

		if (*cur == '/')
			skipComment();

		if (*cur == '}') break;

		if (!advance_if(isalpha)) return;
		if (!advance('=')) return;
		if (!advance('"')) return;

		auto name = readString();

		advance(';');
		if (!advance("//"))
			return;

		OutputUnit ou;
		ou.name = name;
		ou.subtitle = readCommentText();
		ou.soundFile = name + ".wav";
		list.insert(ou);
	};
}
} // namespace ou
} // namespace daedalus
