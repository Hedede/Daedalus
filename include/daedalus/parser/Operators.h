/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_OperatorPrecedence
#define Daedalus_OperatorPrecedence
#include <daedalus/lexer/Token.h>
namespace daedalus {
namespace prec {
enum Level {
	None           = -1,
	Unknown        = 0,
	Assignment     = 1,  // =, -=, +=, *=, /=
	LogicalOr      = 2,
	LogicalAnd     = 3,
	BitwiseOr      = 4,
	BitwiseXor     = 5,
	BitwiseAnd     = 6,
	Equality       = 7,  // ==, !=
	Relational     = 8,  // >=, <=, <, >
	Shift          = 9,  // <<, >>
	Additive       = 10, // +, -
	Multiplicative = 11  // *, *, %
};
} // namespace prec

prec::Level getOperatorPrecedence(Token tok)
{
	switch(tok.type()) {
	default:
		// Not an operator
		return prec::None;
	case Token::equal:
	case Token::ast_equal:
	case Token::plus_equal:
	case Token::minus_equal:
	case Token::slash_equal:
		return prec::Assignment;
	case Token::pipe_pipe:
		return prec::LogicalOr;
	case Token::amp_amp:
		return prec::LogicalAnd;
	case Token::pipe:
		return prec::BitwiseOr;
	case Token::caret:
		return prec::BitwiseXor;
	case Token::amp:
		return prec::BitwiseAnd;
	case Token::equal_equal:
	case Token::bang_equal:
		return prec::Equality;
	case Token::less:
	case Token::less_equal:
	case Token::greater:
	case Token::greater_equal:
		return prec::Relational;
	case Token::less_less:
	case Token::greater_greater:
		return prec::Shift;
	case Token::plus:
	case Token::minus:
		return prec::Additive;
	case Token::ast:
	case Token::slash:
	case Token::percent:
		return prec::Multiplicative;	
	}
}

bool isRightAssociative(Token tok)
{
	return getOperatorPrecedence(tok) == prec::Assignment;
}

bool isBinaryOperator(Token tok)
{
	return getOperatorPrecedence(tok) > prec::Unknown;
}

bool isUnaryOperator(Token tok)
{
	switch (tok.type()) {
	case Token::bang:
	case Token::tilde:
	case Token::plus:
	case Token::minus:
		return true;
	default:
		return false;
	}
}

bool isOperator(Token tok)
{
	return isBinaryOperator(tok) || isUnaryOperator(tok);
}
} // namespace daedalus
#endif //Daedalus_OperatorPrecedence
