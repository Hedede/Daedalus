/*
 * Copyright (C) 2016 Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <set>
#include <string>
#include <functional>
#include <iostream>
#include <daedalus/utility/string.h>
#include <daedalus/io/SourceBuffer.h>
namespace daedalus {
struct OutputUnit {
	std::string name;
	std::string soundFile;
	std::string subtitle;
};

bool operator == (OutputUnit const& a, OutputUnit const& b)
{
	a.name == b.name;
}

bool operator < (OutputUnit const& a, OutputUnit const& b)
{
	a.name < b.name;
}

class OUParser {
public:
	typedef std::set<OutputUnit> OutputUnits;

	OUParser(SourceBuffer& buf)
		: buf(buf)
	{
		cur = buf.begin();
	}

	OutputUnits loadOutputUnits();
private:
	template <class UnaryPredicate>
	bool advance_if(UnaryPredicate predicate);
	bool advance(char c);
	template <class Unary1, class Unary2>
	bool advance_if(Unary1 trueCond, Unary2 falseCond);
	bool advance(char c, char e);
	bool advance(std::string str);

	void skipLineComment();
	void skipBlockComment();
	void skipComment();
	void skipWhitespace();

	std::string readWord();
	std::string readCommentText();
	std::string readString();

	void processAI_Output(OutputUnits& units);
	void processSVMInstance(OutputUnits& units);

	SourceBuffer& buf;
	char const* cur;
	char const* end;
};

template <class UnaryPredicate>
bool OUParser::advance_if(UnaryPredicate predicate)
{
	for (; cur != end; ++cur) {
		if (predicate(*cur))
			return true;
	}
	return false;
}

bool OUParser::advance(char c)
{
	return advance_if([c] (char x) {return x == c;});
}

template <class Unary1, class Unary2>
bool OUParser::advance_if(Unary1 trueCond, Unary2 falseCond)
{
	for (; cur != end; ++cur) {
		if (trueCond(*cur))
			return true;
		if (falseCond(*cur))
			return false;
	}
	return false;
}

bool OUParser::advance(char c, char e)
{
	return advance_if([c] (char x) {return x == c;},
	              [e] (char x) {return x == e;});
}

bool OUParser::advance(std::string str)
{
	assert(!str.empty());

	if (!advance(str[0], '\n'))
		return false;

	for (char c : str) {
		if (*cur++ != c) return false;
	}

	return true;
}

void OUParser::skipComment()
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

void OUParser::skipWhitespace()
{
	advance_if([] (char x) {return !isspace(x);});
}

std::string OUParser::readWord()
{
	auto* start = cur++;

	advance_if([] (char x) {return !isalnum(x) && x != '_';});

	return std::string(start, cur);
}

std::string OUParser::readString()
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

std::string OUParser::readCommentText()
{
	auto* start = cur;
	advance('\n');
	return std::string(start, cur);
}

auto OUParser::loadOutputUnits() -> OutputUnits
{
	OutputUnits ou;

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

void OUParser::processAI_Output(OutputUnits& list)
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

void OUParser::processSVMInstance(OutputUnits& list)
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

int main(char** argv)
{
	if (argv[1] == 0)
		return 1;

	ReadFile file(argv[1]);
	SourceBuffer buffer(file);
	OUParser parser(buffer);
	auto ou = parser.loadOutputUnits();

	std::cout << argv[1] << std::endl;
	for (auto& u : ou) {
		std::cout << u.name << " = " << u.subtitle << std::endl;
	}

	return 0;
}
} // namespace daedalus

int main(int, char** argv)
{
	return daedalus::main(argv);
}
