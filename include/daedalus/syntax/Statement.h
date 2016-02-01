/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Statement
#define Daedalus_Statement
#include <daedalus/common/types.h>
#include <daedalus/syntax/Visitor.h>
namespace daedalus {
namespace tree {
class Statement {
public:
	virtual ~Statement() = default;

	virtual void accept(tree::Visitor& visitor) = 0;
};

} // namespace tree
} // namespace daedalus
#endif//Daedalus_Statement
