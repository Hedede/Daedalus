/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Function
#define Daedalus_Function
#include <daedalus/syntax/Declaration.h>
#include <daedalus/syntax/decl/Variable.h>
#include <daedalus/syntax/stmt/StatementBlock.h>
namespace daedalus {
namespace tree {
typedef std::vector<std::unique_ptr<Variable>> ArgList;

// TODO: merge with Function
class FunctionProto : public Declaration {
public:
	static uptr<FunctionProto>
	create(std::string id,
	       std::string returnType,
	       ArgList args)
	{
		uptr<FunctionProto> tmp(
		        new FunctionProto(id, returnType, std::move(args)));
		return std::move(tmp);
	}

	virtual ~FunctionProto() = default;

	virtual std::string getName() const
	{
		return name;
	}

	virtual std::string getReturnType() const
	{
		return returnType;
	}

	ArgList& getArguments()
	{
		return args;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	FunctionProto(std::string id,
	         std::string returnType,
	         ArgList args)
		: Declaration(Declaration::FunctionProto),
		  name(id), args(std::move(args)), returnType(returnType)
	{
	}

	std::string name;
	std::string returnType;
	ArgList args;
};

class Function : public Declaration {
public:
	static uptr<Function>
	create(uptr<tree::FunctionProto> proto,
	       uptr<StatementBlock> body)
	{
		uptr<Function> tmp(new Function(
		         std::move(proto), std::move(body)));
		return std::move(tmp);
	}

	virtual ~Function() = default;

	tree::FunctionProto& prototype() const
	{
		return *proto;
	}

	StatementBlock& getBody()
	{
		return *body;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	Function(uptr<tree::FunctionProto> proto,
		 uptr<StatementBlock> body)
		: Declaration(Declaration::Function),
		  proto(std::move(proto)),
		  body(std::move(body))
	{
	}

	uptr<tree::FunctionProto> proto;
	uptr<StatementBlock> body;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Function
