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

class Parser {
public:
	Parser(SourceBuffer& buf)
		: buf(buf)
	{
		cur = buf.begin();
		end = buf.end();
	}

	OutputUnitList loadOutputUnits();
private:
	template <class UnaryPredicate>
	bool advance_if(UnaryPredicate predicate);
	bool advance(char c);

	std::string readWord();
	std::string readString();

	void processSubtitle(std::string name, OutputUnitList& list);
	void processAI_Output(OutputUnitList& units);
	void processSVMVar(OutputUnitList& units);
	void processSVMInstance(OutputUnitList& units);

	SourceBuffer& buf;
	char const* cur;
	char const* end;
};
} // namespace ou
} // namespace daedalus
#endif//Daedaalus_OU_PP
