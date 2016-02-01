/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/common/types.h>
#include <daedalus/lexer/Lexer.h>
namespace daedalus {
Lexer::Lexer(SourceBuffer* inputBuffer)
	: buf(inputBuffer)
{
	// Setup keywords
	kwmap
	.add("const", kw_const)
	.add("var", kw_var)
	.add("func", kw_func)
	.add("if", kw_if)
	.add("else", kw_else)
	.add("class", kw_class)
	.add("prototype", kw_prototype)
	.add("instance", kw_instance)
	.add("return", kw_return)
	.add("void", kw_void)
	.add("float", kw_float)
	.add("int", kw_int)
	.add("string", kw_string)
	.add("while", kw_while)
	.add("extern", kw_extern);

	cur = buf->begin();

	// Extract first token
	nextToken();
}

void Lexer::init()
{
}

Token Lexer::currentToken()
{
	return cur_token;
}

/*!
 * Extract next token from stream
 */
Token Lexer::nextToken()
{
	lexNextToken(cur_token);
	return currentToken();
}

bool Lexer::lexIdentifier(Token& token)
{
	char const* start = cur;
	while (isalnum(*cur) || *cur == '_') {
		++ cur;
	}

	std::string id(start, cur);

	// Check if token is a reserved keyword
	auto kind = kwmap.get(id);

	if (!kind)
		kind = tok_identifier;

	token.setType(kind);
	token.setData(id);
	
	return true;
}

bool Lexer::lexNumericConstant(Token& token)
{
	char const* start = cur;

	// TODO: "non-greedy" algorithm
	while (isalnum(*cur) || *cur == '.') {
		++cur;
	}

	char const* prev = cur - 1;
	if ((*cur == '-' || *cur == '+') && (*prev == 'e' || *prev == 'E')) {
		do {
			++cur;
		} while (isalnum(*cur) || *cur == '.');
	}

	std::string num(start, cur);

	token.setData(num);
	token.setType(tok_numeric_constant);

	return true;
}

bool Lexer::lexStringLiteral(Token& token)
{
	std::string str;

	while (*cur != '"') {
		if (*cur == '\\') {
			++cur;
		}
		str += *cur;
		++cur;
	}

	++cur; // consume '"'

	token.setData(str);
	token.setType(tok_string_literal);

	return true;
}

bool Lexer::lexIllegalToken(Token& token)
{
	char const* begin = cur;
	// TODO: search until token-beginnning character
	while (!isspace(*cur)) {
		++cur;
	}

	std::string str(begin, cur);

	token.setData(str);
	token.setType(tok_illegal);

	return true;
}

void Lexer::skipLineComment()
{
	// crude comment handling
	do {
		++cur;
	} while (*cur != '\n');
}

void Lexer::skipBlockComment()
{
	while (*cur != 0) {
		// TODO: Inefficient! Check multiple chars at once
		while (*cur != '/' && *cur != 0) {
			++cur;
		}

		char const* prev = cur++ - 1;
		if (*prev == '*')
			break;
	}
}

#if 0
void Lexer::handleComment()
{
	char c;

	stream.peek(c);

	assert((c == '*' || c == '/')
	       && "Lexer::handleComment called without a comment");
}
#endif

void Lexer::handleComment()
{
	char p = peek();
	if (p == '*') {
		skipBlockComment();
	} else if (p == '/') {
		skipLineComment();
	} else {
		// Not a comment - we're done.
		return;
	}
	
	// Instead of going through everything again, we do everything here.
	// Skip whitespace and check for more comments
	++cur;
	while(isspace(*cur))
		++cur;

	handleComment();
}

char Lexer::peek()
{
	return *(cur + 1);
}

/*!
 * This function is an actual lexer implementation.
 * It reads characters from stream and produces tokens
 */
bool Lexer::lexNextToken(Token& tok)
{
	char const* tok_start = cur;

lexNextToken:
	while (isspace(*cur))
		++cur;

	switch (*cur) {
	case 0:
		tok.setType(tok_eof);
		return true;
	/* Numeric constants */
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return lexNumericConstant(tok);
	/* String literal */
	case '"':
		++ cur; // consume '"'
		return lexStringLiteral(tok);
	/* Identifier */
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case '_':
		return lexIdentifier(tok);
	/* Operators */
	case '^':
		tok.setType(tok_caret);
		break;
	case '~':
		tok.setType(tok_tilde);
		break;
	case ',':
		tok.setType(tok_comma);
		break;
	case '.':
		tok.setType(tok_dot);
		break;
	case ';':
		tok.setType(tok_semicolon);
		break;
	case '%':
		tok.setType(tok_percent);
		break;
	case '{':
		tok.setType(tok_l_brace);
		break;
	case '}':
		tok.setType(tok_r_brace);
		break;
	case '[':
		tok.setType(tok_l_bracket);
		break;
	case ']':
		tok.setType(tok_r_bracket);
		break;
	case '(':
		tok.setType(tok_l_paren);
		break;
	case ')':
		tok.setType(tok_r_paren);
		break;
	case '&':
		if (peek() == '&') {
			tok.setType(tok_amp_amp);
			++cur;
		} else {
			tok.setType(tok_amp);	
		}
		// TODO: does daedalus have '&=' operator?
		break;
	case '|':
		if (peek() == '|') {
			tok.setType(tok_pipe_pipe);
			++cur;
		} else {
			tok.setType(tok_pipe);
		}
		// TODO: does daedalus have '|= operator?
		break;
	case '!':
		if (peek() == '=') {
			tok.setType(tok_bang_equal);
			++cur;
		} else {
			tok.setType(tok_bang);
		}
		break;
	case '*':
		if (peek() == '=') {
			tok.setType(tok_ast_equal);
			++ cur;
		} else {
			tok.setType(tok_ast);
		}
		break;
	case '/':
		// Look for comments first
		handleComment();

		// Check what we have, after we're done with comments
		// If we have '/', continue handling this case.
		// If we have something different, restart lexer.
		// TODO: I could've just restarted the lexer regardless,
		// is this optimization necessary?
		if (*cur != '/')
			// We didn't lex anything, restart the lexer.
			goto lexNextToken;

		if (peek() == '=') {
			tok.setType(tok_slash_equal);
			++cur;
		} else {
			tok.setType(tok_slash);
		}
		break;
	case '=':
		if (peek() == '=') {
			tok.setType(tok_equal_equal);
			++cur;
		} else {
			tok.setType(tok_equal);
		}
		break;
	case '+':
		if (peek() == '+') {
			tok.setType(tok_plus_plus);
			++cur;
		} else if (peek() == '=') {
			tok.setType(tok_plus_equal);
			++cur;
		} else {
			tok.setType(tok_plus);
		}
		break;
	case '-':
		if (peek() == '-') {
			tok.setType(tok_minus_minus);
			++cur;
		} else if (peek() == '=') {
			tok.setType(tok_minus_equal);
			++cur;
		} else {
			tok.setType(tok_minus);
		}
		break;
	case '<':
		if (peek() == '<') {
			tok.setType(tok_less_less);
			++cur;
		} else if (peek() == '=') {
			tok.setType(tok_less_equal);
			++cur;
		} else {
			tok.setType(tok_less);
		}
		break;
	case '>':
		if (peek() == '>') {
			tok.setType(tok_greater_greater);
			++cur;
		} else if (peek() == '=') {
			tok.setType(tok_less_equal);
			++cur;
		} else {
			tok.setType(tok_less);
		}
		break;
	/* Illegal token */
	default:
		// TODO: handle unicode
		return lexIllegalToken(tok);
	}

	std::string val(tok_start, cur);
	tok.setData(val);

	++cur;
	return true;
}
} // namespace daedalus
