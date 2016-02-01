/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Class
#define Daedalus_Class
#include <daedalus/syntax/Statement.h>
#include <daedalus/syntax/Declaration.h>
#include <daedalus/syntax/decl/Variable.h>
namespace daedalus {
namespace tree {
class TypeDeclaration : public Declaration {
public:
	virtual ~TypeDeclaration();

	virtual void accept(tree::Visitor& visitor) = 0;
protected:
	TypeDeclaration(Declaration::Kind k)
		: Declaration(k)
	{
	}
};

class Class : public TypeDeclaration {
public:
	Class()
		: TypeDeclaration(Declaration::Class)
	{
	}
	virtual ~Class();

	virtual void accept(tree::Visitor& visitor);
private:
	std::string name;
	std::vector<tree::Variable*> body;
};

class Prototype : public TypeDeclaration {
public:
	Prototype()
		: TypeDeclaration(Declaration::Prototype)
	{
	}
	virtual ~Prototype();

	virtual void accept(tree::Visitor& visitor);
protected:
	std::string name;
	TypeDeclaration* base;
	// It seems like prototype allows only assignments inside it,
	// while instance additionally allows at least function calls
	std::vector<Statement*> stmts;
};

class Instance : public Declaration {
public:
	Instance()
		: Declaration(Declaration::Instance)
	{
	}

	virtual ~Instance();

	virtual void accept(tree::Visitor& visitor);
private:
	std::string name;
	TypeDeclaration* base;
	std::vector<Statement*> stmts;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Class
