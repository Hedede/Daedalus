/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_PrintToken
#define Daedalus_PrintToken
#include <string>
#include <daedalus/lexer/Token.h>
namespace daedalus {
/*!
 * Print token name
 */
inline std::string printToken(TokenType type);

/*!
 * Print sequence of characters which would produce the token
 */
inline std::string spellToken(TokenType kind);

/*!
 * Get characters compirising the token.
 */
inline std::string spellToken(Token token)
{
	return token.getData();
}


// some macro magic to print token names
#define STR1(x) #x
#define STR(x)  STR1(x)
#define TOKEN1(x) tok_ ## x
#define KEYWORD1(x) kw_ ## x

inline std::string nameToken(TokenType type)
{
	switch (type) {
#define TOKEN(x)   case TOKEN1(x)   : return STR(TOKEN1(x)); 
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) case KEYWORD1(x) : return STR(KEYWORD1(x));
#include <daedalus/lexer/TokenDef.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	}
}

inline std::string spellToken(TokenType kind)
{
	switch (kind) {
#define TOKEN(x)    case TOKEN1(x)   : return STR(x);
#define PUNCT(x, y) case TOKEN1(x)   : return y;
#define KEYWORD(x)  case KEYWORD1(x) : return STR(x);
#include <daedalus/lexer/TokenDef.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	}
}

#undef TOKEN1
#undef KEYWORD1
#undef STR
#undef STR1
} // namespace daedalus
#endif//Daedalus_PrintToken
