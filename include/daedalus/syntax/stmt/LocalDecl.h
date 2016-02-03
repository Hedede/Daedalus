/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Local_Decl
#define Daedalus_Local_Decl
#include <daedalus/syntax/Statement.h>
#include <daedalus/syntax/Declaration.h>
namespace daedalus {
namespace tree {
// TODO: incorporate locals into statemet-block
class LocalDecl : public Statement {
public:
	LocalDecl(uptr<Declaration> local)
		: decl(std::move(local))
	{
	}
	virtual ~LocalDecl() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	Declaration& declaration()
	{
		return *decl;
	}
protected:
	uptr<Declaration> decl;
};

} // namespace tree
} // namespace daedalus
#endif//Daedalus_Local_Decl
