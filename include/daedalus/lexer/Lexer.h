/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Lexer
#define Daedalus_Lexer
#include <daedalus/io/SourceBuffer.h>
#include <daedalus/lexer/Token.h>
#include <daedalus/lexer/KeywordMap.h>
namespace daedalus {
/*!
 * Produces token stream from a source buffer.
 */
class Lexer {
public:
	Lexer(SourceBuffer* inputBuffer);

	Token currentToken();

	/*!
	 * Extract next token from stream
	 */
	Token nextToken();
private:
	void init();
	char peek();

	bool lexIllegalToken(Token& token);
	bool lexIdentifier(Token& token);
	bool lexStringLiteral(Token& token);
	bool lexNumericConstant(Token& token);
	bool lexNextToken(Token& token);

	void skipLineComment();
	void skipBlockComment();
	void handleComment();

	bool lexCommentToken(Token& tok);

	KeywordMap kwmap;
	SourceBuffer* buf;

	char const* cur;
	Token cur_token;
};
} // namespace daedalus
#endif//Daedalus_Lexer
