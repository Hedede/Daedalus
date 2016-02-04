/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_While
#define Daedalus_While
#include <daedalus/syntax/Statement.h>
namespace daedalus {
namespace tree {
class Exression;
/*!
 * whileStmt   = 'while' '(' expr ')' stmt
 */
class WhileStatement : public Statement {
public:
	static uptr<WhileStatement> create(
	                uptr<Expression> cond, uptr<Statement> stmt)
	{
		auto tmp = new WhileStatement(std::move(cond), std::move(stmt));
		return uptr<WhileStatement>(tmp);
	}

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
	WhileStatement(uptr<Expression> cond, uptr<Statement> stmt)
		: cond(std::move(cond)), stmt(std::move(stmt))
	{
	}
	uptr<Expression> cond;
	uptr<Statement>  stmt;
};

/*!
 * DoStmt = 'do' stmt 'while '(' expr ')'
 */
class DoStatement : public Statement {
public:
	static uptr<DoStatement> create(
	                uptr<Expression> cond, uptr<Statement> stmt)
	{
		auto tmp = new DoStatement(std::move(cond), std::move(stmt));
		return uptr<DoStatement>(tmp);
	}

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
	DoStatement(uptr<Expression> cond, uptr<Statement> stmt)
		: cond(std::move(cond)), stmt(std::move(stmt))
	{
	}
	uptr<Expression> cond;
	uptr<Statement>  stmt;
};

class BreakStatement : public Statement {
public:
	static uptr<BreakStatement> create()
	{
		return uptr<BreakStatement>(new BreakStatement);
	}

	virtual ~BreakStatement() = default;
	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	BreakStatement()
		: Statement()
	{
	}
};

class ContinueStatement : public Statement {
public:
	static uptr<ContinueStatement> create()
	{
		return uptr<ContinueStatement>(new ContinueStatement);
	}

	virtual ~ContinueStatement() = default;
	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	ContinueStatement()
		: Statement()
	{
	}
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_While
