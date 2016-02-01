/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Expr_Binary
#define Daedalus_Expr_Binary
#include <daedalus/syntax/Expression.h>
namespace daedalus {
namespace tree {
/*!
 * binary = unary op unary
 */
class BinaryExpr : public Expression {
public:
	BinaryExpr(int op,
	           std::unique_ptr<Expression> lhs,
	           std::unique_ptr<Expression> rhs)
		: op(op), lhs(std::move(lhs)), rhs(std::move(rhs))
	{
	}

	virtual ~BinaryExpr() = default;

	virtual Expression& getLHS()
	{
		return *lhs.get();
	}
	virtual Expression& getRHS()
	{
		return *rhs.get();
	}
	virtual int getOperation()
	{
		return op;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	std::unique_ptr<Expression> lhs;
	std::unique_ptr<Expression> rhs;
	int op;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Expr_Binary
