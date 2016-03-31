/*
 * Copyright (C) 2016 Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <algorithm>
#include <aw/utility/string/case.h>
#include <aw/utility/string/compose.h>
#include <daedalus/ou/Parser.h>
namespace daedalus {
using namespace aw;
namespace ou {
namespace {
char const* skipComment(char const* cur, char const* end)
{
	++cur;
	if (*cur == '/')
		return std::find(cur, end, '\n');

	if (*cur != '*')
		return cur;

	cur += 2;
	while (true) {
		cur = std::find(cur, end, '/');

		if (cur == end)
			return cur;

		char const* prev = cur++ - 1;
		if (*prev == '*')
			return cur;
	}
}

char const* skipJunk(char const* cur, char const* end)
{
	if (isspace(*cur))
		return std::find_if_not(cur, end, isspace);

	if (cur == end)
		return cur;

	if (*cur == '/')
		return skipComment(cur, end);

	return cur + 1;
}
} // namespace

template <class Predicate>
bool Parser::advance_if(Predicate predicate)
{
	for (; cur != end; cur = skipJunk(cur, end)) {
		if (predicate(*cur))
			return true;
	}
	return false;
}

bool Parser::advance(char c)
{
	if (advance_if([c] (char x) {return x == c;})) {
		++cur;
		return true;
	}
	return false;
}

std::string Parser::readWord()
{
	auto* start = cur++;

	advance_if([] (char x) {return !isalnum(x) && x != '_';});

	return std::string(start, cur);
}

std::string Parser::readString()
{
	std::string str;
	for (; cur != end; ++cur) {
		if (*cur == '"')
			break;
		if (*cur == '\\')
			++cur;
		str += *cur;
	}
	++ cur; // skip '"'
	return str;
}

OutputUnitList Parser::loadOutputUnits()
{
	OutputUnitList ou;

	while (*cur) {
		advance_if(isalpha);
		std::string id = readWord();
		string::tolower(id);

		if (id == "ai_output") {
			processAI_Output(ou);
		} else if (id == "instance") {
			processSVMInstance(ou);
		}
	}

	return ou;
}

void Parser::processSubtitle(std::string name, OutputUnitList& list)
{
	cur = std::find_if_not(cur, end, isspace);

	if (*cur++ != '/') return;
	if (*cur++ != '/') return;

	auto start = std::find_if_not(cur, end, isspace);
	cur = std::find(start, end, '\n');

	auto comment = std::string(start, cur);

	OutputUnit ou;
	ou.name = name;
	ou.subtitle = comment;
	ou.soundFile = name + ".wav";
	list.insert(ou);
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

	processSubtitle(name, list);
}

void Parser::processSVMVar(OutputUnitList& list)
{
	if (!advance_if(isalpha)) return;
	if (!advance('=')) return;
	if (!advance('"')) return;

	auto name = readString();

	advance(';');
	processSubtitle(name, list);
}

void Parser::processSVMInstance(OutputUnitList& list)
{
	if (!advance_if(isalpha)) return;
	advance('(');
	advance_if(isalpha);

	if (readWord() != "C_SVM") {
		// Hopefully, nobody puts AI_Output inside instances
		advance('}');
		return;
	}

	advance(')');

	if (!advance('{')) return;

	while (*cur) {
		processSVMVar(list);
		cur = skipJunk(cur, end);
		if (*cur == '}')
			break;
	};
}
} // namespace ou
} // namespace daedalus
