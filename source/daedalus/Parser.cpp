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
#include <daedalus/syntax/stmt/LocalDecl.h>

#include <daedalus/syntax/Expression.h>
#include <daedalus/syntax/expr/UnaryExpr.h>
#include <daedalus/syntax/expr/BinaryExpr.h>
#include <daedalus/syntax/expr/CallExpr.h>
#include <daedalus/syntax/expr/NumberExpr.h>
#include <daedalus/syntax/expr/StringExpr.h>
#include <daedalus/syntax/expr/IdentifierExpr.h>
#include <daedalus/syntax/expr/InitializerExpr.h>
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
	return isIdentifier(tok) ||
	       tok == kw_void    ||
	       tok == kw_float   ||
	       tok == kw_int     ||
	       tok == kw_string  ||
	       tok == kw_func;
}

template<typename...Args>
std::nullptr_t error(DiagnosticHelper& diag, Token tok,
                     Diagnostic::ID id, Args... args)
{
	Diagnostic msg(tok.location(), id);
	int dummy[sizeof...(Args)] = { (msg << args, 0)... };
	diag.report(msg);

	return nullptr;
}

bool Parser::match(TokenType expected)
{
	if (token.getType() != expected)
		return false;

	// consume token
	getNextToken();
	return true;
}

uptr<tree::Declaration>
Parser::parseDeclaration()
{
	uptr<tree::Declaration> decl;

	switch(token.getType()) {
	case kw_var:
		decl = parseGlobalVar();
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
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.getData(), "declaration");
	}

	/* TODO: do not forget about global variables*/
	while (match(tok_semicolon));

	return decl;
}

/*
 * variableDecl ::= 'var' type id
 */
uptr<tree::Variable>
Parser::parseVariable(bool isConst, bool array)
{
	// Read variable type
	if (!isTypeName(token))
		return nullptr;

	// Read variable name
	if (!isIdentifier(getNextToken()))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.getData(), "identifier");
	
	// TODO: symbol table lookup
	std::string name = token.getData();
	getNextToken(); // consume identifier

	auto var = tree::Variable::create(name, isConst);

	if (match(tok_l_bracket)) {
		if (!array)
			return error(diag, token, Diagnostic::ArrayNotAllowed);

		auto size_expr = parseExpression();
		if (!size_expr)
			return nullptr;
		var->setSizeExpr(std::move(size_expr));

		if (!match(tok_r_bracket))
			return error(diag, token, Diagnostic::UnexpectedToken,
			             token.getData(), tok_r_brace);
	}

	return var;
}

uptr<tree::Declaration>
Parser::parseGlobalVar()
{
	getNextToken(); // consume 'var';

	auto var = parseVariable(false, true);
	if (!var)
		return nullptr;

	if (match(tok_semicolon))
		return var;

	return error(diag, token, Diagnostic::ExpectedSemicolon,
		     "variable declaration");
}

uptr<tree::Declaration>
Parser::parseLocalVar()
{
	getNextToken(); // consume 'var';

	auto var = parseVariable(false, true);
	if (!var)
		return nullptr;

	// allow initializer for non-array vars
	if (!var->sizeExpr() && match(tok_equal)) {
		auto initializer = parseExpression();
		if (!initializer)
			return nullptr;
		var->setInitialier(std::move(initializer));
	}

	if (match(tok_semicolon))
		return var;

	return error(diag, token, Diagnostic::ExpectedSemicolon,
	             "variable declaration");
}

/*
 * constantDecl ::= 'const' id id '=' expr
 */
uptr<tree::Declaration>
Parser::parseConstant()
{
	getNextToken(); // consume 'const';

	auto var = parseVariable(true, true);
	if (!var)
		return nullptr;

	// Read constant initializer
	if (!match(tok_equal))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.getData(), "constant initializer");

	uptr<tree::Expression> initializer;
	if (match(tok_l_brace))
		initializer = parseArrayInitializer();
	else
		initializer = parseExpression();

	if (!initializer)
		return nullptr;

	var->setInitialier(std::move(initializer));

	if (match(tok_semicolon))
		return var;

	return error(diag, token, Diagnostic::ExpectedSemicolon,
	             "variable declaration");
}

uptr<tree::Expression>
Parser::parseArrayInitializer()
{
	std::vector<uptr<tree::Expression>> initList;
	while (!match(tok_r_brace)) {
		auto expr = parseExpression();
		if (!expr)
			return nullptr;
		initList.push_back(std::move(expr));

		if (token == tok_r_brace)
			break;

		if (!match(tok_comma))
			return error(diag, token,
			             Diagnostic::UnexpectedToken2,
			             token.getData(), tok_comma);
	}

	if (!match(tok_r_brace))
		return error(diag, token, Diagnostic::ExpectedExpression);

	return std::make_unique<tree::ArrayInitializer>(std::move(initList));
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
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.getData(), "identifier");

	// TODO: symbol table lookup
	std::string name = token.getData();

	// consume identifier
	getNextToken();
	if (!match(tok_l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.getData(), tok_l_paren);

	// Argument list
	std::vector<uptr<tree::Variable>> args;

	while (match(kw_var)) {
		auto arg = parseVariable(false);
		if (!arg)
			return nullptr;

		args.push_back(std::move(arg));

		if (token.getType() == tok_r_paren)
			break;

		if (!match(tok_comma))
			return error(diag, token,
			             Diagnostic::UnexpectedToken2,
			             token.getData(), tok_comma);
	}

	if (!match(tok_r_paren))
		return error(diag, token, Diagnostic::ExpectedVariableDecl);

	return tree::FunctionProto::create(name, ret, std::move(args));
}

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
	// Class name
	if (!isIdentifier(getNextToken()))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.getData(), "class name");

	std::string name = token.getData();

	getNextToken(); // consume name;

	if (!match(tok_l_brace))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.getData(), tok_l_brace);

	// Class members
	std::vector<uptr<tree::Variable>> members;
	while (match(kw_var)) {
		auto var = parseVariable(false, true);
		if (var == nullptr)
			return nullptr;

		members.push_back(std::move(var));

		if (token == tok_r_brace)
			break;

		/*
		if (match(tok_comma)) {
			lexer.unget(var.typename());
			lexer.unget("var");
		}*/

		if (!match(tok_semicolon))
			return error(diag, token, Diagnostic::ExpectedSemicolon,
			             "variable declaration");
	}

	if (!match(tok_r_brace))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.getData(), tok_r_brace);

	return tree::Class::create(name, std::move(members));
}

uptr<tree::Prototype>
Parser::parsePrototype()
{
	if (!isIdentifier(getNextToken()))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.getData(), "prototype name");

	std::string name = token.getData();
	getNextToken(); // consume name;

	if (!match(tok_l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
	                     token.getData(), tok_l_paren);

	std::string base = token.getData();
	getNextToken();

	if (!match(tok_r_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.getData(), tok_r_paren);

	auto body = parseStatementBlock();
	if (!body)
		return nullptr;

	return tree::Prototype::create(name, base, std::move(body));
}

uptr<tree::Instance>
Parser::parseInstance()
{
	if (!isIdentifier(getNextToken()))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.getData(), "instance name");

	std::string name = token.getData();
	getNextToken(); // consume name;

	if (!match(tok_l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.getData(), tok_l_paren);

	std::string base = token.getData();
	getNextToken();

	if (!match(tok_r_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
	                     token.getData(), tok_r_paren);

	if (match(tok_semicolon))
		return tree::Instance::create(name, base, nullptr);

	auto body = parseStatementBlock();
	if (!body)
		return nullptr;

	return tree::Instance::create(name, base, std::move(body));
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
	case kw_const:
	case kw_var:
		return parseLocal();
	case tok_l_brace:
		return parseStatementBlock();
	default:
		return parseExprStatement();
	}
}

uptr<tree::Statement>
Parser::parseLocal()
{
	uptr<tree::Declaration> var;
	if (token == kw_var)
		var = parseLocalVar();

	if (token == kw_const)
		var = parseConstant();

	if (var)
		return std::make_unique<tree::LocalDecl>(std::move(var));

	return nullptr;
}

uptr<tree::Statement>
Parser::parseExprStatement()
{
	auto expr = parseExpression();

	if (!match(tok_semicolon))
		return error(diag, token, Diagnostic::ExpectedSemicolon,
		             "expression");

	return std::move(expr);
}

uptr<tree::StatementBlock>
Parser::parseStatementBlock()
{
	if (!match(tok_l_brace))
		return error(diag, token, Diagnostic::UnexpectedToken2,
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

constexpr bool AllowParenlessIf = true;

uptr<tree::Statement>
Parser::parseBranchStatement()
{
	if (!AllowParenlessIf && !match(tok_l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.getData(), tok_l_paren);

	uptr<tree::Expression> ifExpr = parseExpression();
	if (!ifExpr)
		return nullptr;

	if (!AllowParenlessIf && !match(tok_r_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
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
Parser::parseReturnStatement()
{
	if (match(tok_semicolon))
		return std::make_unique<tree::ReturnStatement>(nullptr);

	uptr<tree::Expression> retExpr = parseExpression();
	if (!retExpr)
		return nullptr;

	if (!match(tok_semicolon))
		return error(diag, token, Diagnostic::ExpectedSemicolon,
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
		return error(diag, token, Diagnostic::ExpectedExpression);
	}
}

uptr<tree::Expression>
Parser::parseParenExpr()
{
	if (!match(tok_l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.getData(), tok_l_paren);

	uptr<tree::Expression> expr = parseExpression();

	if (!match(tok_r_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
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
				return error(diag, token,
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
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.getData(), tok_r_bracket);

	return std::make_unique<tree::SubscriptExpr>(
	                std::move(array), std::move(arg));
}

uptr<tree::Expression>
Parser::parseFieldAccess(std::string id)
{
	if (!isIdentifier(token))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.getData(), "identifier");

	std::string field = token.getData();

	getNextToken(); // consume identifier

	return std::make_unique<tree::FieldExpr>(id, field);
}

uptr<tree::Expression>
Parser::parseStringExpr()
{
	assert(token == tok_string_literal);

	Token tok = token;
	// Consume string
	getNextToken();

	return std::make_unique<tree::StringExpr>(tok.getData());
}

uptr<tree::Expression>
Parser::parseNumberExpr()
{
	assert(token == tok_numeric_constant);

	// store token, because we need to consume it
	Token tok = token;
	// consume number
	getNextToken();

	return std::make_unique<tree::NumberExpr>(tok.getData());
}
} // namespace hrscript
