/*
 * Copyright (C) 2016 Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <daedalus/utility/string.h>
#include <daedalus/io/SourceBuffer.h>
namespace daedalus {
struct OutputUnit {
	std::string name;
	std::string soundFile;
	std::string subtitle;
};

class OUParser {
public:
	typedef std::map<std::string, OutputUnit> OutputUnits;

	OUParser(SourceBuffer& buf)
		: buf(buf)
	{
		cur = buf.begin();
	}

	OutputUnits loadOutputUnits();
private:
	std::string getWord();
	std::string processCommentText();
	std::string processString();
	void skipLineComment();
	void skipBlockComment();
	void skipWhitespace();
	bool eatWord(std::string prefix = "");
	bool search(char c, char stop = 0);
	bool search_strict(std::string str);
	void processThing(OutputUnits& ou);
	void processAI_Output(OutputUnits& units);
	void processInstance(OutputUnits& units);

	SourceBuffer& buf;
	char const* cur;
};

void OUParser::skipLineComment()
{
	do {
		++cur;
	} while (*cur != '\n');
}

void OUParser::skipBlockComment()
{
	while (true) {
		do {
			++cur;
		} while (*cur != '/' && *cur != 0);

		if (*cur == 0)
			break;

		char const* prev = cur - 1;
		if (*prev == '*')
			break;
	}
}

void OUParser::skipWhitespace()
{
	while (isspace(*cur))
		++cur;
}

std::string OUParser::getWord()
{
	auto* start = cur++;

	while (isalnum(*cur) || *cur == '_')
		++cur;

	return std::string(start, cur);
}

std::string OUParser::processString()
{
	assert(*cur == '"');
	++cur;

	std::string str;
	while (*cur && *cur != '"') {
		if (*cur == '\\') {
			++cur;
		}
		str += *cur++;
	}
	return str;
}

std::string OUParser::processCommentText()
{
	auto* start = cur;
	while (*cur && *cur != '\n') {
		 *cur++;
	}
	return std::string(start, cur);
}

bool OUParser::search(char c, char stop)
{
	while (*cur != c) {
		if (!*cur && *cur != stop)
			return false;
		++cur;
	}
	return true;
}

bool OUParser::search_strict(std::string str)
{
	assert(!str.empty());

	if (!search(str[0], '\n'))
		return false;

	for (char c : str) {
		if (*cur++ != c) return false;
	}
	return true;
}

bool OUParser::eatWord(std::string prefix)
{
	if (!prefix.empty()) {
		if (!search_strict(prefix))
			return false;
	}
	while (!isalnum(*cur)) {
		if (!*cur)
			return false;
		++cur;
	}
	return true;
}

auto OUParser::loadOutputUnits() -> OutputUnits
{
	OutputUnits ou;

	while (*cur) {
		switch (*cur) {
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
		case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
		case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
		case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
		case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
		case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
		case 'v': case 'w': case 'x': case 'y': case 'z':
			processThing(ou);
			continue;
		case ' ': case '\n': case '\r': case '\t': case '\v':
			skipWhitespace();
			continue;
		case '/':
			++cur;
			if (*cur == '*')
				skipBlockComment();
			if (*cur == '/')
				skipLineComment();
		default:
			++cur;
			continue;
		};
	}

	return ou;
}

void OUParser::processThing(OutputUnits& ou)
{
	std::string id = getWord();
	string::tolower(id);

	if (id == "ai_output") {
		processAI_Output(ou);
	} else if (id == "instance") {
		processInstance(ou);
	}
}

void OUParser::processAI_Output(OutputUnits& units)
{
	search('(');
	search(',');
	search(',');

	if (!search('"'))
		return;

	auto name = processString();

	search(')');
	search(';');

	if (!search_strict("//"))
		return;
	
	OutputUnit ou;
	ou.subtitle = processCommentText();
	ou.soundFile = name + ".wav";
	units[name] = ou;
}

void OUParser::processInstance(OutputUnits& units)
{
	if (!eatWord("SVM")) return;
	search('(');
	if (!eatWord("C_SVM")) return;
	search(')');
	if (!search('{')) return;
	if (!eatWord()) return;
	
	while (true) {
		while (!isalnum(*cur)) {
			if (!*cur) break;
			if (*cur == '/') {
				++ cur;
				if (*(cur+1) == '/')
					skipLineComment();
				else if (*(cur+1) == '*')
					skipBlockComment();
			}
			if (*cur == '}') break;
			++cur;
		}
		if (!eatWord()) return;
		if (!search('=')) return;
		if (!search('"')) return;

		auto name = processString();

		search(';');
		if (!search_strict("//"))
			return;

		OutputUnit ou;
		ou.subtitle = processCommentText();
		ou.soundFile = name + ".wav";
		units[name] = ou;
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

	for (auto& u : ou) {
		std::cout << u.first << " = " << u.second.subtitle << std::endl;
	}

	return 0;
}
} // namespace daedalus

int main(int, char** argv)
{
	return daedalus::main(argv);
}
