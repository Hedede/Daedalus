/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Stmt_Return
#define Daedalus_Stmt_Return
#include <daedalus/syntax/Statement.h>
namespace daedalus {
namespace tree {
class Expression;
/*!
 * return = 'return' expr 
 */
class ReturnStatement : public Statement {
public:
	ReturnStatement(uptr<Expression> retExpr)
		: expr(std::move(retExpr))
	{
	}

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
#endif//Daedalus_Stmt_Return
