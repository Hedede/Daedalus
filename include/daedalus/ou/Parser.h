/*
 * Copyright (C) 2016 Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedaalus_OU_PP
#define Daedaalus_OU_PP
#include <set>
#include <string>
#include <daedalus/io/SourceBuffer.h>
#include <daedalus/lexer/Lexer.h>
namespace daedalus {
namespace ou {
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

typedef std::set<OutputUnit> OutputUnitList;

class OUParser {
public:

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
} // namespace ou
} // namespace daedalus
#endif//Daedaalus_OU_PP
