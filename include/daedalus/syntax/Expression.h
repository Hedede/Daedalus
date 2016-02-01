/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Expression
#define Daedalus_Expression
#include <daedalus/syntax/Statement.h>
namespace daedalus {
namespace tree {
class Expression : public Statement {
public:
	virtual ~Expression() = default;

	virtual void accept(tree::Visitor& visitor) = 0;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Expression
