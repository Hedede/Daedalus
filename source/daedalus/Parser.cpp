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

#include <daedalus/syntax/Declaration.h>
#include <daedalus/syntax/Statement.h>
#include <daedalus/syntax/Expression.h>

#include <daedalus/utility/PrintToken.h>
#include <daedalus/utility/DiagnosticHelper.h>
#include <daedalus/parser/Parser.h>
namespace daedalus {
bool isTypeName(Token tok)
{
	return isIdentifier(tok)       ||
	       tok == Token::kw_void   ||
	       tok == Token::kw_float  ||
	       tok == Token::kw_int    ||
	       tok == Token::kw_string ||
	       tok == Token::kw_func;
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

bool Parser::match(Token::Kind expected)
{
	if (token.type() != expected)
		return false;

	// consume token
	getNextToken();
	return true;
}

uptr<tree::Declaration>
Parser::parseDeclaration()
{
	uptr<tree::Declaration> decl;

	switch(token.type()) {
	case Token::kw_var:
		decl = parseGlobalVar();
		break;
	case Token::kw_const:
		decl = parseConstant();
		break;
	case Token::kw_func:
		decl = parseFunctionDefinition();
		break;
	case Token::kw_class:
		decl = parseClass();
		break;
	case Token::kw_prototype:
		decl = parsePrototype();
		break;
	case Token::kw_instance:
		decl = parseInstance();
		break;
	case Token::eof:
		return nullptr;
	default:
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.data(), "declaration");
	}

	/* TODO: do not forget about global variables*/
	while (match(Token::semicolon));

	return decl;
}

TypeDef* Parser::readType()
{
	if (!isTypeName(token))
		return error(diag, token, Diagnostic::UnexpectedToken, token.data(), "type name");

	//return symtab.find(token);
	auto type = new TypeDef{token.data()};
	if (!type)
		return error(diag, token, Diagnostic::UnknownType, token.data());
	return type;
}

/*
 * variableDecl ::= 'var' type id
 */
uptr<tree::Variable>
Parser::parseVariable(Type type)
{
	// Read variable name
	if (!isIdentifier(getNextToken()))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.data(), "identifier");
	
	// TODO: symbol table lookup
	std::string name = token.data();
	getNextToken(); // consume identifier

	auto var = std::make_unique<tree::Variable>(name, type);

	if (match(Token::l_bracket)) {
		auto size_expr = parseExpression();
		if (!size_expr)
			return nullptr;

		var->setSizeExpr(std::move(size_expr));

		if (!match(Token::r_bracket))
			return error(diag, token, Diagnostic::UnexpectedToken,
			             token.data(), Token::r_brace);
	}

	return var;
}

uptr<tree::Declaration>
Parser::parseGlobalVar()
{
	getNextToken(); // consume 'var';

	auto type = readType();
	if (!type)
		return nullptr;

	Type var_type{type, false, 1};

	auto var = parseVariable(var_type);
	if (!var)
		return nullptr;

	if (match(Token::semicolon))
		return var;

	return error(diag, token, Diagnostic::ExpectedSemicolon,
		     "variable declaration");
}

uptr<tree::Declaration>
Parser::parseLocalVar()
{
	getNextToken(); // consume 'var';

	auto type = readType();
	if (!type)
		return nullptr;

	Type var_type{type, false, 1};

	auto var = parseVariable(var_type);
	if (!var)
		return nullptr;

	// allow initializer for non-array vars
	if (!var->isArray() && match(Token::equal)) {
		auto initializer = parseExpression();
		if (!initializer)
			return nullptr;
		var->setInitialier(std::move(initializer));
	}

	if (match(Token::semicolon))
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

	auto type = readType();
	if (!type)
		return nullptr;

	Type var_type{type, true, 1};

	auto var = parseVariable(var_type);
	if (!var)
		return nullptr;

	// Read constant initializer
	if (!match(Token::equal))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.data(), "constant initializer");

	uptr<tree::Expression> initializer;
	if (match(Token::l_brace))
		initializer = parseArrayInitializer();
	else
		initializer = parseExpression();

	if (!initializer)
		return nullptr;

	var->setInitialier(std::move(initializer));

	if (match(Token::semicolon))
		return var;

	return error(diag, token, Diagnostic::ExpectedSemicolon,
	             "variable declaration");
}

uptr<tree::Expression>
Parser::parseArrayInitializer()
{
	std::vector<uptr<tree::Expression>> initList;
	while (!match(Token::r_brace)) {
		auto expr = parseExpression();
		if (!expr)
			return nullptr;
		initList.push_back(std::move(expr));

		if (token == Token::r_brace)
			break;

		if (!match(Token::comma))
			return error(diag, token,
			             Diagnostic::UnexpectedToken2,
			             token.data(), Token::comma);
	}

	if (!match(Token::r_brace))
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
	auto type = readType();
	if (!type)
		return nullptr;

	Type ret{type, false, 1};

	// Function name
	if (!isIdentifier(getNextToken()))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.data(), "identifier");

	// TODO: symbol table lookup
	std::string name = token.data();

	// consume identifier
	getNextToken();
	if (!match(Token::l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::l_paren);

	// Argument list
	std::vector<uptr<tree::Variable>> args;

	while (match(Token::kw_var)) {
		auto type = readType();
		if (!type)
			return nullptr;

		Type arg_type{type, false, 1};
		auto arg = parseVariable(arg_type);
		if (!arg)
			return nullptr;
		if (arg->isArray())
			return error(diag, token, Diagnostic::ArrayNotAllowed);

		args.push_back(std::move(arg));

		if (token.type() == Token::r_paren)
			break;

		if (!match(Token::comma))
			return error(diag, token,
			             Diagnostic::UnexpectedToken2,
			             token.data(), Token::comma);
	}

	if (!match(Token::r_paren))
		return error(diag, token, Diagnostic::ExpectedVariableDecl);

	return std::make_unique<tree::FunctionProto>(name, ret, std::move(args));
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

	if (token.type() == Token::semicolon)
		return proto;

	auto body = parseStatementBlock();

	if (!body)
		return nullptr;

	return std::make_unique<tree::Function>(std::move(proto), std::move(body));
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
		             token.data(), "class name");

	std::string name = token.data();

	getNextToken(); // consume name;

	if (!match(Token::l_brace))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::l_brace);

	// Class members
	std::vector<uptr<tree::Variable>> members;
	while (match(Token::kw_var)) {
		auto type = readType();
		if (!type)
			return error(diag, token, Diagnostic::UnexpectedToken,
			             token.data(), "type name");

		Type var_type{type, false, 1};
		auto var = parseVariable(var_type);
		if (var == nullptr)
			return nullptr;

		members.push_back(std::move(var));

		if (token == Token::r_brace)
			break;

		/*
		if (match(Token::comma)) {
			lexer.unget(var.typename());
			lexer.unget("var");
		}*/

		if (!match(Token::semicolon))
			return error(diag, token, Diagnostic::ExpectedSemicolon,
			             "variable declaration");
	}

	if (!match(Token::r_brace))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::r_brace);

	return std::make_unique<tree::Class>(name, std::move(members));
}

uptr<tree::Prototype>
Parser::parsePrototype()
{
	if (!isIdentifier(getNextToken()))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.data(), "prototype name");

	std::string name = token.data();
	getNextToken(); // consume name;

	if (!match(Token::l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
	                     token.data(), Token::l_paren);

	std::string base = token.data();
	getNextToken();

	if (!match(Token::r_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::r_paren);

	auto body = parseStatementBlock();
	if (!body)
		return nullptr;

	return std::make_unique<tree::Prototype>(name, base, std::move(body));
}

uptr<tree::Instance>
Parser::parseInstance()
{
	if (!isIdentifier(getNextToken()))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.data(), "instance name");

	std::string name = token.data();
	getNextToken(); // consume name;

	if (!match(Token::l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::l_paren);

	std::string base = token.data();
	getNextToken();

	if (!match(Token::r_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
	                     token.data(), Token::r_paren);

	if (match(Token::semicolon))
		return std::make_unique<tree::Instance>(name, base, nullptr);

	auto body = parseStatementBlock();
	if (!body)
		return nullptr;

	return std::make_unique<tree::Instance>(name, base, std::move(body));
}

uptr<tree::Statement>
Parser::parseStatement()
{
	switch (token.type()) {
	case Token::kw_if:
		getNextToken();
		return parseBranchStatement();
	case Token::kw_return:
		getNextToken();
		return parseReturnStatement();
	case Token::kw_while:
		getNextToken();
		return parseWhileStatement();
	case Token::kw_do:
		getNextToken();
		return parseDoStatement();
	case Token::kw_break:
	case Token::kw_continue:
		return parseBreakStatement();
	case Token::kw_const:
	case Token::kw_var:
		return parseLocal();
	case Token::l_brace:
		return parseStatementBlock();
	default:
		return parseExprStatement();
	}
}

uptr<tree::Statement>
Parser::parseBreakStatement()
{
	assert(token == Token::kw_break || token == Token::kw_continue);

	uptr<tree::Statement> stmt;
	if (token == Token::kw_break)
		stmt = std::make_unique<tree::BreakStatement>();
	else if (token == Token::kw_continue)
		stmt = std::make_unique<tree::ContinueStatement>();

	getNextToken();

	if (match(Token::semicolon))
		return stmt;

	return error(diag, token, Diagnostic::UnexpectedToken2,
		     token.data(), Token::semicolon);
}


uptr<tree::Statement>
Parser::parseLocal()
{
	uptr<tree::Declaration> var;
	if (token == Token::kw_var)
		var = parseLocalVar();

	if (token == Token::kw_const)
		var = parseConstant();

	if (var)
		return std::make_unique<tree::DeclStatement>(std::move(var));

	return nullptr;
}

uptr<tree::Statement>
Parser::parseExprStatement()
{
	auto expr = parseExpression();

	if (!match(Token::semicolon))
		return error(diag, token, Diagnostic::ExpectedSemicolon,
		             "expression");

	return std::make_unique<tree::ExprStatement>(std::move(expr));
}

uptr<tree::StatementBlock>
Parser::parseStatementBlock()
{
	if (!match(Token::l_brace))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::l_brace);

	std::vector<uptr<tree::Statement>> statements;
	while (!match(Token::r_brace)) {
		auto statement = parseStatement();
		if (!statement)
			return nullptr;

		statements.push_back(std::move(statement));
	}

	while(match(Token::semicolon));

	return std::make_unique<tree::StatementBlock>(std::move(statements));
}

constexpr bool AllowParenlessIf = true;
uptr<tree::Statement>
Parser::parseBranchStatement()
{
	uptr<tree::Expression> ifExpr = AllowParenlessIf ?
	                parseExpression() : parseParenExpr();
	if (!ifExpr)
		return nullptr;

	uptr<tree::Statement> ifBody = parseStatement();
	if (!ifBody)
		return nullptr;

	uptr<tree::Statement> elseBody = nullptr;

	if (match(Token::kw_else)) {
		elseBody = parseStatement();

		if (!elseBody)
			return nullptr;
	}

	return std::make_unique<tree::IfStatement>(
	       std::move(ifExpr), std::move(ifBody), std::move(elseBody));
}

uptr<tree::Statement>
Parser::parseWhileStatement()
{
	uptr<tree::Expression> ifExpr = AllowParenlessIf ?
	                parseExpression() : parseParenExpr();
	if (!ifExpr)
		return nullptr;

	uptr<tree::Statement> body = parseStatement();
	if (!body)
		return nullptr;

	return std::make_unique<tree::WhileStatement>(
	                std::move(ifExpr), std::move(body));
}

uptr<tree::Statement>
Parser::parseDoStatement()
{
	uptr<tree::Statement> body = parseStatement();
	if (!body)
		return nullptr;

	if (!match(Token::kw_while))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::kw_while);

	uptr<tree::Expression> ifExpr = AllowParenlessIf ?
	                parseExpression() : parseParenExpr();
	if (!ifExpr)
		return nullptr;

	if (!match(Token::semicolon))
		return error(diag, token, Diagnostic::ExpectedSemicolon,
		             "do-while statement");

	return std::make_unique<tree::DoStatement>(
	                std::move(ifExpr), std::move(body));
}


uptr<tree::Statement>
Parser::parseReturnStatement()
{
	if (match(Token::semicolon))
		return std::make_unique<tree::ReturnStatement>(nullptr);

	uptr<tree::Expression> retExpr = parseExpression();
	if (!retExpr)
		return nullptr;

	if (!match(Token::semicolon))
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
	switch (token.type()) {
	case Token::l_paren:
		return parseParenExpr();
	case Token::identifier:
		return parseIdentifierExpr();
	case Token::numeric_constant:
		return parseNumberExpr();
	case Token::string_literal:
		return parseStringExpr();
	default:
		return error(diag, token, Diagnostic::ExpectedExpression);
	}
}

uptr<tree::Expression>
Parser::parseParenExpr()
{
	if (!match(Token::l_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::l_paren);

	uptr<tree::Expression> expr = parseExpression();

	if (!match(Token::r_paren))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::r_paren);

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
		       opcode.type(), std::move(LHS), std::move(RHS));
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
	        opcode.type(), std::move(operand));
}

uptr<tree::Expression>
Parser::parseIdentifierExpr()
{
	std::string name = token.data();

	getNextToken(); // consume identifier
	if (match(Token::l_paren))
		return parseCallExpr(name);

	uptr<tree::Expression> id_expr;
	if (match(Token::dot)) {
		id_expr = parseFieldAccess(name);
	} else { 
		id_expr = std::make_unique<tree::IdentifierExpr>(name);
	}

	if (match(Token::l_bracket)) {
		id_expr = parseArraySubscript(std::move(id_expr));
	}

	// TODO: postfix operators

	return id_expr;
}

uptr<tree::Expression>
Parser::parseCallExpr(std::string func)
{
	std::vector<uptr<tree::Expression>> args;

	if (!match(Token::r_paren)) {
		while (true) {
			auto arg = parseExpression();

			if (!arg)
				return nullptr;

			args.push_back(std::move(arg));

			if (match(Token::r_paren))
				break;

			if (!match(Token::comma))
				return error(diag, token,
				             Diagnostic::UnexpectedToken2,
				             token.data(), Token::comma);
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

	if (!match(Token::r_bracket))
		return error(diag, token, Diagnostic::UnexpectedToken2,
		             token.data(), Token::r_bracket);

	return std::make_unique<tree::SubscriptExpr>(
	                std::move(array), std::move(arg));
}

uptr<tree::Expression>
Parser::parseFieldAccess(std::string id)
{
	if (!isIdentifier(token))
		return error(diag, token, Diagnostic::UnexpectedToken,
		             token.data(), "identifier");

	std::string field = token.data();

	getNextToken(); // consume identifier

	return std::make_unique<tree::FieldExpr>(id, field);
}

uptr<tree::Expression>
Parser::parseStringExpr()
{
	assert(token == Token::string_literal);

	Token tok = token;
	// Consume string
	getNextToken();

	return std::make_unique<tree::StringExpr>(tok.data());
}

uptr<tree::Expression>
Parser::parseNumberExpr()
{
	assert(token == Token::numeric_constant);

	// store token, because we need to consume it
	Token tok = token;
	// consume number
	getNextToken();

	return std::make_unique<tree::NumberExpr>(tok.data());
}
} // namespace hrscript
