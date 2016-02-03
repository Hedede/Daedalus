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
 * doWhileStmt = 'do' stmt 'while '(' expr ')'
 */
class WhileStatement : public Statement {
public:
	enum Kind {
		While,
		Do,
	};

	static uptr<WhileStatement> create(Kind kind,
	                uptr<Expression> cond,
	                uptr<Statement> stmt)
	{
		auto tmp = new WhileStatement(
		                kind, std::move(cond), std::move(stmt));
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

	Kind kind()
	{
		return kind_;
	}
private:
	WhileStatement(Kind kind, uptr<Expression> cond, uptr<Statement> stmt)
		: kind_(kind), cond(std::move(cond)), stmt(std::move(stmt))
	{
	}
	Kind kind_;
	uptr<Expression> cond;
	uptr<Statement>  stmt;
};

class BreakStatement : public Statement {
public:
	enum Kind {
		Break,
		Continue,
	};
	static uptr<BreakStatement> create(Kind kind)
	{
		auto tmp = new BreakStatement(kind);
		return uptr<BreakStatement>(tmp);
	}
	virtual ~BreakStatement() = default;
	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	Kind kind()
	{
		return kind_;
	}
private:
	BreakStatement(Kind kind)
		: kind_(kind)
	{
	}
	Kind kind_;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_While
