/*
 * Copyright (C) 2016   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Parser
#define Daedalus_Parser
#include <daedalus/common/types.h>
#include <daedalus/lexer/Lexer.h>
#include <daedalus/syntax/Declaration.h>
#include <daedalus/parser/Operators.h>

namespace daedalus {
namespace tree {
class Statement;
class StatementBlock;
class FunctionProto;
class Variable;
class Prototype;
class Instance;
class Expression;
}

class DiagnosticHelper;

/*!
 * Parser transforms token stream into the Abstract Syntax Tree
 */
class Parser {
public:
	Parser(Lexer& lexer, DiagnosticHelper& diag)
		: lexer(lexer), diag(diag)
	{
		token = lexer.currentToken();
	}

	virtual ~Parser() = default;

	/*!
	 * Parse next top-level declaration.
	 */
	uptr<tree::Declaration> parseDeclaration();

	/*!
	 * Consume peek token and extract next token.
	 */
	Token getNextToken()
	{
		return token = lexer.nextToken();
	}
private:
	/*! Current lookahead (peek) token. */
	Token token;

	/*! Lexer which provides the stream of tokens */
	Lexer& lexer;

	/*! Diagnostics helper for error reporting */
	DiagnosticHelper& diag;

	/*!
	 * Match token, and consume if matched.
	 */
	bool match(TokenType expected);

	uptr<tree::FunctionProto> parseFunctionPrototype();
	uptr<tree::Declaration> parseFunctionDefinition();
	uptr<tree::Declaration> parseClass();
	uptr<tree::Prototype> parsePrototype();
	uptr<tree::Instance> parseInstance();
	uptr<tree::Variable> parseVariable();
	uptr<tree::Declaration> parseConstant();

	uptr<tree::StatementBlock> parseStatementBlock();
	uptr<tree::Statement> parseStatement();
	uptr<tree::Statement> parseBranchStatement();
	uptr<tree::Statement> parseWeirdIfStatement();
	uptr<tree::Statement> parseReturnStatement();
	uptr<tree::Statement> parseExprStatement();

	uptr<tree::Expression> parseExpression();
	uptr<tree::Expression> parseUnaryExpr();
	uptr<tree::Expression> parseBinaryExpr(
	                uptr<tree::Expression> LHS,
	                prec::Level minPrec);

	uptr<tree::Expression> parsePrimaryExpr();
	uptr<tree::Expression> parseIdentifierExpr();
	uptr<tree::Expression> parseParenExpr();
	uptr<tree::Expression> parseNumberExpr();
	uptr<tree::Expression> parseStringExpr();
	uptr<tree::Expression> parseCallExpr(std::string func);
	uptr<tree::Expression> parseFieldAccess(std::string id);
	uptr<tree::Expression> parseArraySubscript(uptr<tree::Expression>);
};
} // namespace daedalus
#endif //Daedalus_Parser
