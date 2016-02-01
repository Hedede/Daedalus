/*
 * Copyright (C) 2016   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Expr_Subscript
#define Daedalus_Expr_Subscript
#include <vector>
#include <string>
#include <daedalus/syntax/Expression.h>
namespace daedalus {
namespace tree {
class SubscriptExpr : public Expression {
public:
	SubscriptExpr(uptr<Expression> array,
	              uptr<Expression> expr)
		: arrayExpr(std::move(array)),
		  subscriptExpr(std::move(expr))
	{
	}

	virtual ~SubscriptExpr() = default;

	virtual Expression& array() const
	{
		return *arrayExpr;
	}

	virtual Expression& subscript()
	{
		return *subscriptExpr;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	uptr<Expression> arrayExpr;
	uptr<Expression> subscriptExpr;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Expr_Subscript
