/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Declaration
#define Daedalus_Declaration
#include <daedalus/common/types.h>
#include <daedalus/syntax/Visitor.h>
namespace daedalus {
namespace tree {
class Declaration {
public:
	virtual ~Declaration() = default;

	//! Implements Visitor Pattern
	virtual void accept(tree::Visitor& visitor) = 0;

	//! List of concrete derived types
	enum Kind {
		FunctionProto,
		Function,
		Variable,
		Class,
		Prototype,
		Instance
	};

	Kind getKind() const
	{
		return kind;
	}

protected:
	Declaration(Kind k)
		: kind(k)
	{
	}

private:
	Kind kind;
};

} // namespace tree
} // namespace daedalus
#endif//Daedalus_Declaration
