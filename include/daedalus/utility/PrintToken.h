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
inline std::string printToken(Token::Kind type);

/*!
 * Print sequence of characters which would produce the token
 */
inline std::string spellToken(Token::Kind kind);

/*!
 * Get characters compirising the token.
 */
inline std::string spellToken(Token token)
{
	return token.data();
}


// some macro magic to print token names
#define STR1(x) #x
#define STR(x)  STR1(x)
#define TOKEN_NAME(x) Token::x
#define KEYWORD_NAME(x) Token::kw_ ## x

inline std::string nameToken(Token::Kind type)
{
	switch (type) {
#define TOKEN(x)   case TOKEN_NAME(x)   : return STR(TOKEN1(x));
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) case KEYWORD_NAME(x) : return STR(KEYWORD1(x));
#include <daedalus/lexer/TokenDef.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	}
}

inline std::string spellToken(Token::Kind kind)
{
	switch (kind) {
#define TOKEN(x)    case TOKEN_NAME(x)   : return STR(x);
#define PUNCT(x, y) case TOKEN_NAME(x)   : return y;
#define KEYWORD(x)  case KEYWORD_NAME(x) : return STR(x);
#include <daedalus/lexer/TokenDef.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	}
}

#undef TOKEN_NAME
#undef KEYWORD_NAME
#undef STR
#undef STR1
} // namespace daedalus
#endif//Daedalus_PrintToken
