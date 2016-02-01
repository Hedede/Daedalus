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
#include <string>
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

class Token {
public:
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
		return data;
	}

	void setData(std::string newData)
	{
		data = newData;
	}
private:
	TokenType type;
	std::string data;
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
