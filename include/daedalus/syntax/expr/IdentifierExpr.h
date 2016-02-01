/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Expr_Identifier
#define Daedalus_Expr_Identifier
#include <daedalus/syntax/Expression.h>
namespace daedalus {
namespace tree {
/*! Expression for identifier referencing */
class IdentifierExpr : public Expression {
public:
	IdentifierExpr(std::string id)
		: id(id)
	{
	}

	virtual ~IdentifierExpr() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::string getName()
	{
		return id;
	}
private:
	std::string id;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Expr_Identifier
