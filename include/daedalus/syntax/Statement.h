/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Statement
#define Daedalus_Statement
#include <aw/types/types.h>
#include <daedalus/syntax/Visitor.h>
namespace daedalus {
using namespace aw;
namespace tree {
class Statement {
public:
	virtual ~Statement() = default;

	//! List of concrete derived types
	enum Kind {
		StatementBlock,
		DeclStatement,
		IfStatement,
		WhileStatement,
		DoStatement,
		BreakStatement,
		ContinueStatement,
		ReturnStatement,
		Expr,
	};

	Kind kind() const
	{
		return kind_;
	}

protected:
	Statement(Kind k)
		: kind_(k)
	{ }

private:
	Kind const kind_;
};

class Expression;

using StatementList = std::vector<uptr<Statement>>;

class StatementBlock : public Statement {
public:
	StatementBlock(StatementList statements)
		: Statement(Statement::StatementBlock),
		  stmts(std::move(statements))
	{ }

	virtual ~StatementBlock() = default;

	StatementList& statements()
	{
		return stmts;
	}

private:
	StatementList   stmts;
};

class Declaration;

class DeclStatement : public Statement {
public:
	DeclStatement(uptr<Declaration> local)
		: Statement(Statement::DeclStatement),
		  decl(std::move(local))
	{ }

	virtual ~DeclStatement() = default;

	Declaration& declaration()
	{
		return *decl;
	}

protected:
	uptr<Declaration> decl;
};

/***************************
 * Control flow statements *
 ***************************/
/*!
 * ifStmt = 'if' '(' expr ')' block optElse
 * optElse = ('else' block) | ϵ
 */
class IfStatement : public Statement {
public:
	IfStatement(uptr<Expression> ifExpr,
	            uptr<Statement>  ifBody,
	            uptr<Statement> elseBody)
		: Statement(Statement::IfStatement),
		  ifExpr(std::move(ifExpr)),
		  ifBody(std::move(ifBody)),
		  elseBody(std::move(elseBody))
	{ }

	virtual ~IfStatement() = default;

	Expression& condition()
	{
		return *ifExpr;
	}

	Statement& thenBranch()
	{
		return *ifBody;
	}

	Statement* elseBranch()
	{
		return elseBody.get();
	}

protected:
	uptr<Expression> ifExpr;
	uptr<Statement>  ifBody;
	uptr<Statement>  elseBody;
};

/*!
 * whileStmt   = 'while' '(' expr ')' stmt
 */
class WhileStatement : public Statement {
public:
	WhileStatement(uptr<Expression> cond, uptr<Statement> stmt)
		: Statement(Statement::WhileStatement),
		  cond(std::move(cond)),
		  stmt(std::move(stmt))
	{ }

	virtual ~WhileStatement() = default;

	Expression& condition()
	{
		return *cond;
	}

	Statement& body()
	{
		return *stmt;
	}

private:
	uptr<Expression> cond;
	uptr<Statement>  stmt;
};

/*!
 * DoStmt = 'do' stmt 'while '(' expr ')'
 */
class DoStatement : public Statement {
public:
	DoStatement(uptr<Expression> cond, uptr<Statement> stmt)
		: Statement(Statement::DoStatement),
		  cond(std::move(cond)),
		  stmt(std::move(stmt))
	{ }

	virtual ~DoStatement() = default;

	Expression& condition()
	{
		return *cond;
	}

	Statement& body()
	{
		return *stmt;
	}

private:
	uptr<Expression> cond;
	uptr<Statement>  stmt;
};

class BreakStatement : public Statement {
public:
	BreakStatement()
		: Statement(Statement::BreakStatement)
	{ }

	virtual ~BreakStatement() = default;
};

class ContinueStatement : public Statement {
public:
	ContinueStatement()
		: Statement(Statement::ContinueStatement)
	{ }

	virtual ~ContinueStatement() = default;
};

/*!
 * return = 'return' expr 
 */
class ReturnStatement : public Statement {
public:
	ReturnStatement(uptr<Expression> retExpr)
		: Statement(Statement::ReturnStatement),
		  expr(std::move(retExpr))
	{ }

	virtual ~ReturnStatement() = default;

	Expression* expression()
	{
		return expr.get();
	}

protected:
	uptr<Expression> expr;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Statement
