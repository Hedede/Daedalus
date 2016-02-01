/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Expr_Field
#define Daedalus_Expr_Field
#include <vector>
#include <string>
#include <daedalus/syntax/Expression.h>
namespace daedalus {
namespace tree {
class FieldExpr : public Expression {
public:
	FieldExpr(std::string inst,
	          std::string field)
		: inst(inst), field(field)
	{
	}

	virtual ~FieldExpr() = default;

	virtual std::string identifier() const
	{
		return inst;
	}

	virtual std::string fieldName()
	{
		return field;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	std::string inst;
	std::string field;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Expr_Field
