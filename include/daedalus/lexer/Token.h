/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Lexer_Token
#define Daedalus_Lexer_Token
#include <cassert>
#include <string>
#include <daedalus/common/types.h>
namespace daedalus {
enum TokenType {
#define TOKEN(x) tok_ ## x,
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) kw_ ## x,
#include <daedalus/lexer/TokenDef.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
};

struct Location {
	Location() = default;
	Location(size_t offset)
		: pos(offset)
	{
		assert(offset < (1 << 31));
	}
	u32 pos;
};

class Token {
public:
	// TODO: Illegal
	Token() : kind(tok_illegal) { }
	Token(char const* start, char const* end)
		: start(start), end(end)
	{
	}

	TokenType getType() const
	{
		return type;
	}
	
	void setType(TokenType newType)
	{
		type = newType;
	}

	std::string getData() const
	{
		return std::string(start, end);
	}

	void setData(char const* start, char const* end)
	{
		this->start = start;
		this->end = end;
	}

	void setLocation(Location newLoc)
	{
		loc = newLoc;
	}

	Location location()
	{
		return loc;
	}
private:
	TokenType type;
	char const* start;
	char const* end;
	Location loc;
};

inline bool isIdentifier(Token tok)
{
	return tok.getType() == tok_identifier;
}

inline bool operator == (Token tok, TokenType kind)
{
	return tok.getType() == kind;
}
} // namespace daedalus
#endif//Daedalus_Lexer_Token
