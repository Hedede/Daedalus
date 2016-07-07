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
#include <aw/types/types.h>
namespace daedalus {
using namespace aw;

struct Location {
	Location() = default;
	Location(size_t offset)
		: pos(offset)
	{
		assert(offset < (1u << 31));
	}
	u32 fileId;
	u32 pos;
};

struct Token {
	/*!
	 * Enumeration of all token kinds,
	 * All kinds are defined if TokenDef.h
	 */
	enum Kind {
#define TOKEN(x) x,
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) kw_ ## x,
#include <daedalus/lexer/TokenDef.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	};

	// TODO: Illegal
	Token() : kind(Token::illegal) { }
	Token(char const* start, char const* end)
		: start(start), end(end)
	{
	}

	Kind type() const
	{
		return kind;
	}
	
	void setType(Kind newKind)
	{
		kind = newKind;
	}

	std::string data() const
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
	Kind kind;
	char const* start;
	char const* end;
	Location loc;
};

inline bool isIdentifier(Token tok)
{
	// TODO: Identifier
	return tok.type() == Token::identifier;
}

inline bool operator == (Token tok, Token::Kind kind)
{
	return tok.type() == kind;
}
} // namespace daedalus
#endif//Daedalus_Lexer_Token
