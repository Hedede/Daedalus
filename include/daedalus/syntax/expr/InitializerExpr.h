/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_ArrayInitializer
#define Daedalus_ArrayInitializer
#include <daedalus/syntax/Expression.h>
namespace daedalus {
namespace tree {
// TODO: Probably initializer should be a class on its own
class ArrayInitializer : public Expression {
public:
	typedef std::vector<uptr<Expression>> InitList;

	ArrayInitializer(InitList initList)
		: exprs(std::move(initList))
	{
	}

	virtual ~ArrayInitializer() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	InitList& initList()
	{
		return exprs;
	}

	size_t size()
	{
		return exprs.size();
	}
private:
	InitList exprs;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_ArrayInitializer
