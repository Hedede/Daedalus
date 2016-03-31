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
#include <daedalus/common/types.h>
#include <daedalus/syntax/Visitor.h>
namespace daedalus {
namespace tree {
class Statement {
public:
	virtual ~Statement() = default;

	virtual void accept(tree::Visitor& visitor) = 0;
};

class Expression;

typedef std::vector<uptr<Statement>>   StatementList;
class StatementBlock : public Statement {
public:
	StatementBlock(StatementList statements)
		: stmts(std::move(statements))
	{
	}

	virtual ~StatementBlock() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

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
		: decl(std::move(local))
	{
	}
	virtual ~DeclStatement() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

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
		: ifExpr(std::move(ifExpr)),
		  ifBody(std::move(ifBody)),
		  elseBody(std::move(elseBody))
	{ }

	virtual ~IfStatement() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

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
		: cond(std::move(cond)), stmt(std::move(stmt))
	{ }

	virtual ~WhileStatement() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

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
		: cond(std::move(cond)), stmt(std::move(stmt))
	{ }

	virtual ~DoStatement() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

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
		: Statement()
	{
	}

	virtual ~BreakStatement() = default;
	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
};

class ContinueStatement : public Statement {
public:
	ContinueStatement()
		: Statement()
	{
	}

	virtual ~ContinueStatement() = default;
	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
};

/*!
 * return = 'return' expr 
 */
class ReturnStatement : public Statement {
public:
	ReturnStatement(uptr<Expression> retExpr)
		: expr(std::move(retExpr))
	{ }

	virtual ~ReturnStatement() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

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
