/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Variable
#define Daedalus_Variable
#include <memory>
#include <string>
#include <daedalus/syntax/Declaration.h>
#include <daedalus/syntax/Expression.h>
namespace daedalus {
namespace tree {
class Variable : public Declaration {
public:
	static uptr<Variable>
	create(std::string id, bool is_const)
	{
		uptr<Variable> tmp(new Variable(id, is_const));
		return std::move(tmp);
	}

	virtual ~Variable() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	virtual bool isConst()
	{
		return constant;
	}

	virtual void setConst(bool cnst)
	{
		constant = cnst;
	}

	virtual void setInitialier(std::unique_ptr<Expression> newVal)
	{
		init_expr = std::move(newVal);
	}

	virtual Expression* initializer()
	{
		return init_expr.get();
	}

	void setSizeExpr(uptr<Expression> newSize)
	{
		size_expr = std::move(newSize);
	}

	Expression* sizeExpr() const
	{
		return size_expr.get();
	}

	std::string getName() const
	{
		return name;
	}
private:
	Variable(std::string id, bool constant)
		: Declaration(Declaration::Variable),
		  name(id), constant(constant)
	{
	}

	std::string name;
	bool constant;
	uptr<Expression> init_expr = nullptr;
	uptr<Expression> size_expr = nullptr;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Variable
