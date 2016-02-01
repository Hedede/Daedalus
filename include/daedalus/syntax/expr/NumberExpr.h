/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Expt_Number
#define Daedalus_Expt_Number
#include <daedalus/syntax/Expression.h>
namespace daedalus {
namespace tree {
/*! Expression for numeric literals */
class NumberExpr : public Expression {
public:
	NumberExpr(std::string value)
		: value(value)
	{
	}

	virtual ~NumberExpr() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::string getValue() const
	{
		return value;
	}
private:
	std::string value;
};

class FPNumberExpr : NumberExpr {
	virtual ~FPNumberExpr() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
};

class IntNumberExpr : NumberExpr {
	virtual ~IntNumberExpr() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
};

} // namespace tree
} // namespace daedalus
#endif//Daedalus_Expt_Number
