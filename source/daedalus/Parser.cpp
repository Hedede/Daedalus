/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <vector>

#include <daedalus/syntax/decl/Class.h>
#include <daedalus/syntax/decl/Variable.h>
#include <daedalus/syntax/decl/Function.h>

#include <daedalus/syntax/Expression.h>
#include <daedalus/syntax/expr/UnaryExpr.h>
#include <daedalus/syntax/expr/BinaryExpr.h>
#include <daedalus/syntax/expr/CallExpr.h>
#include <daedalus/syntax/expr/NumberExpr.h>
#include <daedalus/syntax/expr/StringExpr.h>
#include <daedalus/syntax/expr/IdentifierExpr.h>
#include <daedalus/syntax/expr/FieldExpr.h>
#include <daedalus/syntax/expr/SubscriptExpr.h>

#include <daedalus/syntax/Statement.h>
#include <daedalus/syntax/stmt/IfElseStatement.h>
#include <daedalus/syntax/stmt/ReturnStatement.h>
#include <daedalus/syntax/stmt/StatementBlock.h>

#include <daedalus/utility/PrintToken.h>
#include <daedalus/utility/DiagnosticHelper.h>
#include <daedalus/parser/Parser.h>

namespace daedalus {
bool isTypeName(Token tok)
{
	return true;
}

template<typename...Args>
std::nullptr_t error(DiagnosticHelper& diag, Location loc,
                     Diagnostic::ID id, Args... args)
{
	Diagnostic msg(loc, id);
	int dummy[sizeof...(Args)] = { (msg << args, 0)... };
	diag.report(msg);

	return nullptr;

}

uptr<tree::Declaration>
ErrorDeclaration(std::string msg)
{
	//print(msg);
	return nullptr;
}

uptr<tree::Declaration>
Parser::parseDeclaration()
{
	uptr<tree::Declaration> decl;

	switch(token.getType()) {
	case kw_var:
		decl = parseVariable();
		break;
	case kw_const:
		decl = parseConstant();
		break;
	case kw_func:
		decl = parseFunctionDefinition();
		break;
	case kw_class:
		decl = parseClass();
		break;
	case kw_prototype:
		decl = parsePrototype();
		break;
	case kw_instance:
		decl = parseInstance();
		break;
	case tok_eof:
		return nullptr;
	default:
		return error(diag, Location(), Diagnostic::UnexpectedToken,
		             token.getData(), "declaration");
	}

	/* TODO: do not forget about global variables*/
	while (match(tok_semicolon));

	return decl;
}

/*
 * variableDecl ::= 'var' id id
 */
uptr<tree::Variable>
Parser::parseVariable()
{
	// Read variable type
	if (!isTypeName(token))
		return nullptr;

	// Read variable name
	if (!isIdentifier(getNextToken()))
		return error(diag, Location(), Diagnostic::UnexpectedToken,
		             token.getData(), "identifier");
	
	// TODO: symbol table lookup
	std::string name = token.getData();

	return tree::Variable::create(name, false);
}

/*
 * constantDecl ::= 'const' id id '=' expr
 */
uptr<tree::Declaration>
Parser::parseConstant()
{
	return error(diag, Location(), Diagnostic::NotImplemented, "const");
#if 0
	// Read variable type
	if (!isTypeName(getNextToken()))
		return 0;

	// Read variable name
	if (!isIdentifier(getNextToken()))
		return 0;

	// TODO: symbol table lookup
	std::string name = token.getData();

	// Read constant initializer
	if (getNextToken().getType() != tok_equals)
		return 0;

	tree::Expression* initializer = parseExpression();

	// Constant* constant = new Constant(/*symbol*/, /*thingy*/); // TODO 

	return new ConstantDeclaration(name, initializer);
#endif
}

bool Parser::match(TokenType expected)
{
	if (token.getType() != expected)
		return false;

	// consume token
	getNextToken();
	return true;
}

/*
 * functionDecl ::= 'func' id id '(' args ')'
 *         args ::= arg (',' args)?
 *          arg ::= variableDecl
 */
uptr<tree::FunctionProto>
Parser::parseFunctionPrototype()
{
	// Return type
	if (!isTypeName(token))
		return nullptr;

	std::string ret = token.getData();

	// Function name
	if (!isIdentifier(getNextToken()))
		return error(diag, Location(), Diagnostic::UnexpectedToken,
		             token.getData(), "identifier");

	// TODO: symbol table lookup
	std::string name = token.getData();

	// consume identifier
	getNextToken();
	if (!match(tok_l_paren))
		return error(diag, Location(), Diagnostic::UnexpectedToken2,
		             token.getData(), tok_l_paren);

	// Argument list
	std::vector<uptr<tree::Variable>> args;

	while (match(kw_var)) {
		auto arg = parseVariable();
		if (!arg)
			return nullptr;

		args.push_back(std::move(arg));

		getNextToken();

		if (token.getType() == tok_r_paren)
			break;

		if (!match(tok_comma))
			return error(diag, Location(),
			             Diagnostic::UnexpectedToken2,
			             token.getData(), tok_comma);
	}
	
	if (!match(tok_r_paren))
		return error(diag, Location(), Diagnostic::ExpectedVariableDecl);

	return tree::FunctionProto::create(name, ret, std::move(args));
}
/*
std::nullptr_t Parser::error(Diagnostic::ID id)
{
	return nullptr;
}*/

/*
 * functionDef  ::= functionDecl '{' stmts '}'
 */
uptr<tree::Declaration>
Parser::parseFunctionDefinition()
{
	getNextToken(); // consume 'func'

	auto proto = parseFunctionPrototype();

	if (!proto)
		return nullptr;

	if (token.getType() == tok_semicolon)
		return std::move(proto);

	auto body = parseStatementBlock();

	if (!body)
		return nullptr;

	return tree::Function::create(
	        std::move(proto), std::move(body));
}

/*
 *      classDecl ::= 'class' id
 *          class ::= classDecl '{' classBodyDecls '}'
 * classBodyDecl  ::= functionDecl | varDecl
 * classBodyDecls ::= classBodyDecl ';' classBodyDecls?
 */
uptr<tree::Declaration>
Parser::parseClass()
{
	return error(diag, Location(), Diagnostic::NotImplemented, "class");
#if 0
	// Class name
	if (!isIdentifier(getNextToken()))
		return 0;

	std::string name = token.getData();

	if (!getNextToken().getType() != tok_l_brace)
		return new ClassDeclaration(name);

	// Class members
	std::vector<Variable*> members;
	while (getNextToken().getType() == kw_var) {
		auto var = parseVariableDeclaration();
		if (var == 0)
			return 0;

		members.push_back(var);

		getNextToken();

		if (token != tok_semicolon)
			return 0;
	}

	if (!getNextToken().getType() != tok_r_brace)
		return 0;

	return new ClassDeclaration(name, members);
#endif
}

uptr<tree::Prototype>
Parser::parsePrototype()
{
	return error(diag, Location(), Diagnostic::NotImplemented, "prototype");
}

uptr<tree::Instance>
Parser::parseInstance()
{
	return error(diag, Location(), Diagnostic::NotImplemented, "instance");
}

uptr<tree::Statement>
Parser::parseStatement()
{
	switch (token.getType()) {
	case kw_if:
		getNextToken(); // consume 'if'
		return parseBranchStatement();
	case kw_return:
		getNextToken(); // consume 'return'
		return parseReturnStatement();
	case tok_l_brace:
		return parseStatementBlock();
	default:
		return parseExprStatement();
	}
}

uptr<tree::Statement>
Parser::parseExprStatement()
{
	auto expr = parseExpression();

	if (!match(tok_semicolon))
		return error(diag, Location(), Diagnostic::ExpectedSemicolon,
		             "expression");

	return std::move(expr);
}

uptr<tree::StatementBlock>
Parser::parseStatementBlock()
{
	if (!match(tok_l_brace))
		return error(diag, Location(), Diagnostic::UnexpectedToken2,
		             token.getData(), tok_l_brace);

	std::vector<uptr<tree::Statement>> statements;
	while (!match(tok_r_brace)) {
		auto statement = parseStatement();
		if (!statement)
			return nullptr;

		statements.push_back(std::move(statement));
	}

	while(match(tok_semicolon));

	return std::make_unique<tree::StatementBlock>(std::move(statements));
}

uptr<tree::Statement>
Parser::parseBranchStatement()
{
	if (!match(tok_l_paren))
		return parseWeirdIfStatement();
		//return unexpectedTokenError(tok_l_paren);

	uptr<tree::Expression> ifExpr = parseExpression();
	if (!ifExpr)
		return nullptr;

	if (!match(tok_r_paren))
		return error(diag, Location(), Diagnostic::UnexpectedToken2,
		             token.getData(), tok_r_paren);

	uptr<tree::Statement> ifBody = parseStatement();
	if (!ifBody)
		return nullptr;

	uptr<tree::Statement> elseBody = nullptr;

	if (match(kw_else)) {
		elseBody = parseStatement();

		if (!elseBody)
			return nullptr;
	}

	return std::make_unique<tree::IfElseStatement>(
	        std::move(ifExpr), std::move(ifBody), std::move(elseBody));
}

uptr<tree::Statement>
Parser::parseWeirdIfStatement()
{
	uptr<tree::Expression> ifExpr = parseExpression();
	if (!ifExpr)
		return nullptr;

	uptr<tree::Statement> ifBody = parseStatementBlock();
	if (!ifBody)
		return nullptr;

	uptr<tree::Statement> elseBody = nullptr;
	if (match(kw_else)) {
		elseBody = parseStatement();

		if (!elseBody)
			return nullptr;
	}

	return std::make_unique<tree::IfElseStatement>(
	        std::move(ifExpr), std::move(ifBody), std::move(elseBody));
	
}

uptr<tree::Statement>
Parser::parseReturnStatement()
{
	if (match(tok_semicolon))
		return std::make_unique<tree::ReturnStatement>(nullptr);

	uptr<tree::Expression> retExpr = parseExpression();
	if (!retExpr)
		return nullptr;

	if (!match(tok_semicolon))
		return error(diag, Location(), Diagnostic::ExpectedSemicolon,
		             "expression");

	return std::make_unique<tree::ReturnStatement>(
	        std::move(retExpr));
}


/********************** Expressions **********************/
uptr<tree::Expression>
Parser::parseExpression()
{
	// Parse left hand side
	uptr<tree::Expression> lhs = parseUnaryExpr();

	if (!lhs)
		return nullptr;

	return parseBinaryExpr(std::move(lhs), prec::Unknown);
}

uptr<tree::Expression>
Parser::parsePrimaryExpr()
{
	switch (token.getType()) {
	case tok_l_paren:
		return parseParenExpr();
	case tok_identifier:
		return parseIdentifierExpr();
	case tok_numeric_constant:
		return parseNumberExpr();
	case tok_string_literal:
		return parseStringExpr();
	default:
		return error(diag, Location(), Diagnostic::ExpectedExpression);
	}
}

uptr<tree::Expression>
Parser::parseParenExpr()
{
	if (!match(tok_l_paren))
		return error(diag, Location(), Diagnostic::UnexpectedToken2,
		             token.getData(), tok_l_paren);

	uptr<tree::Expression> expr = parseExpression();

	if (!match(tok_r_paren))
		return error(diag, Location(), Diagnostic::UnexpectedToken2,
		             token.getData(), tok_r_paren);

	return expr;
}

uptr<tree::Expression>
Parser::parseBinaryExpr(uptr<tree::Expression> LHS,
                        prec::Level minPrec)
{
	while (1) {
		prec::Level curPrec = getOperatorPrecedence(token);
		if (curPrec < minPrec)
			return LHS;

		Token opcode = token;
		getNextToken(); // consume operator

		uptr<tree::Expression> RHS = parseUnaryExpr();
		if (!RHS)
			return nullptr;


		prec::Level nextPrec = getOperatorPrecedence(token);
		bool isRightAssoc = isRightAssociative(token);

		if (curPrec < nextPrec ||
		   (curPrec == nextPrec && isRightAssoc)) {
			RHS = parseBinaryExpr(
			       std::move(RHS),
			       prec::Level(curPrec + !isRightAssoc));
			if (!RHS)
				return nullptr;
		}

		LHS = std::make_unique<tree::BinaryExpr>(
		       opcode.getType(), std::move(LHS), std::move(RHS));
	}
}

uptr<tree::Expression>
Parser::parseUnaryExpr()
{
	if (!isUnaryOperator(token))
		return parsePrimaryExpr();

	Token opcode = token;
	getNextToken(); // consume operator

	uptr<tree::Expression> operand = parseUnaryExpr();
	if (!operand)
		return nullptr;

	return std::make_unique<tree::UnaryExpr>(
	        opcode.getType(), std::move(operand));
}

uptr<tree::Expression>
Parser::parseIdentifierExpr()
{
	std::string name = token.getData();

	getNextToken(); // consume identifier
	if (match(tok_l_paren))
		return parseCallExpr(name);

	uptr<tree::Expression> id_expr;
	if (match(tok_dot)) {
		id_expr = parseFieldAccess(name);
	} else { 
		id_expr = std::make_unique<tree::IdentifierExpr>(name);
	}

	if (match(tok_l_bracket)) {
		id_expr = parseArraySubscript(std::move(id_expr));
	}

	// TODO: postfix operators

	return id_expr;
}

uptr<tree::Expression>
Parser::parseCallExpr(std::string func)
{
	std::vector<uptr<tree::Expression>> args;

	if (!match(tok_r_paren)) {
		while (true) {
			auto arg = parseExpression();

			if (!arg)
				return nullptr;

			args.push_back(std::move(arg));

			if (match(tok_r_paren))
				break;

			if (!match(tok_comma))
				return error(diag, Location(),
				             Diagnostic::UnexpectedToken2,
				             token.getData(), tok_comma);
		}
	}

	return std::make_unique<tree::CallExpr>(
	        std::move(func), std::move(args));
}

uptr<tree::Expression>
Parser::parseArraySubscript(uptr<tree::Expression> array)
{
	auto arg = parseExpression();
	if (!arg)
		return nullptr;

	if (!match(tok_r_bracket))
		return error(diag, Location(), Diagnostic::UnexpectedToken2,
		             token.getData(), tok_r_bracket);

	return std::make_unique<tree::SubscriptExpr>(
	                std::move(array), std::move(arg));
}

uptr<tree::Expression>
Parser::parseFieldAccess(std::string id)
{
	if (!isIdentifier(token))
		return error(diag, Location(), Diagnostic::UnexpectedToken,
		             token.getData(), "identifier");

	std::string field = token.getData();

	getNextToken(); // consume identifier

	return std::make_unique<tree::FieldExpr>(id, field);
}

uptr<tree::Expression>
Parser::parseStringExpr()
{
	assert(token == tok_string_literal && "parseStringExpr called when there's no string literal!");

	Token tok = token;
	// Consume string
	getNextToken();

	return std::make_unique<tree::StringExpr>(tok.getData());
}

uptr<tree::Expression>
Parser::parseNumberExpr()
{
	assert(token == tok_numeric_constant && "parseNumberExpr called when there's no number!");

	// store token, because we need to consume it
	Token tok = token;
	// consume number
	getNextToken();

	return std::make_unique<tree::NumberExpr>(tok.getData());
}

// Print out diagnostic and return nullptr
// Assumes that it is called after failed match()
std::nullptr_t Parser::unexpectedTokenError(TokenType expected)
{
}
} // namespace hrscript
