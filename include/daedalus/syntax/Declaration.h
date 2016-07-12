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
#include <vector>
#include <aw/types/types.h>
#include <daedalus/semantic/Symbol.h>
namespace daedalus {
using StringList = std::vector<std::string>;
using namespace aw;
namespace tree {
/*!
 * Declaration specifies meaning of a symbol.
 */
class Declaration {
public:
	virtual ~Declaration() = default;

	//! List of concrete derived types
	enum Kind {
		FunctionProto,
		Function,
		Variable,
		Class,
		Prototype,
		Instance
	};

	Kind kind() const
	{
		return kind_;
	}

protected:
	Declaration(Kind k)
		: kind_(k)
	{ }

private:
	Kind const kind_;
};

using DeclarationList = std::vector<uptr<Declaration>>;

class Expression;

struct Variable : Declaration {
	Variable(std::string id, Type type)
		: Declaration(Declaration::Variable),
		  id(id), _type(type)
	{ }

	virtual ~Variable() = default;

	std::string name() const
	{
		return id;
	}

	Type type() const
	{
		return _type;
	}

	bool isArray() const
	{
		return _type.count > 1;
	}

	bool isConst()
	{
		return _type.isConst;
	}

	void setConst(bool cnst)
	{
		_type.isConst = cnst;
	}

	Expression* initializer()
	{
		return init_expr.get();
	}

	void setInitialier(std::unique_ptr<Expression> newVal)
	{
		init_expr = std::move(newVal);
	}

	// TODO: ArrayDecl : public Variable?
	// XXX: nope, will be part of type system
	Expression* sizeExpr() const
	{
		return size_expr.get();
	}

	void setSizeExpr(uptr<Expression> newSize)
	{
		size_expr = std::move(newSize);
	}

private:
	std::string id;
	Type _type;
	uptr<Expression> init_expr = nullptr;
	uptr<Expression> size_expr = nullptr;
};

typedef std::vector<std::unique_ptr<Variable>> VarList;

// TODO: merge with Function
struct FunctionProto : Declaration {
	FunctionProto(std::string id, Type returnType, VarList args)
		: Declaration(Declaration::FunctionProto),
		  _name(id), _returnType(returnType), args(std::move(args))
	{ }

	virtual ~FunctionProto() = default;

	std::string name() const
	{
		return _name;
	}

	Type returnType() const
	{
		return _returnType;
	}

	VarList& arguments()
	{
		return args;
	}

private:
	std::string _name;
	Type _returnType;
	VarList args;
};

class StatementBlock;

struct Function : Declaration {
	Function(uptr<tree::FunctionProto> proto,
		 uptr<StatementBlock> body)
		: Declaration(Declaration::Function),
		  proto(std::move(proto)),
		  body(std::move(body))
	{ }

	virtual ~Function() = default;

	tree::FunctionProto& prototype() const
	{
		return *proto;
	}

	StatementBlock& getBody()
	{
		return *body;
	}

	/*
	void addLocal(uptr<Variable> var)
	{
		localVars.push_back(std::move(var));
	}

	VarList& locals()
	{
		return localVars;
	}
	*/

private:
	uptr<tree::FunctionProto> proto;
	uptr<StatementBlock> body;
	//VarList localVars;
};

class Class : public Declaration {
public:
	Class(std::string name, VarList members)
		: Declaration(Declaration::Class),
		  name(name), body(std::move(members))
	{ }

	virtual ~Class() = default;

	std::vector<uptr<tree::Variable>>& members()
	{
		return body;
	}

private:
	std::string name;
	std::vector<uptr<tree::Variable>> body;
};

class Prototype : public Declaration {
public:
	Prototype(std::string name, std::string base,
	          uptr<StatementBlock> body)
		: Declaration(Declaration::Prototype),
		  name_(name), base_(base), block(std::move(body))
	{
	}

	virtual ~Prototype() = default;

	std::string name() const
	{
		return name_;
	}

	std::string base() const
	{
		return base_;
	}

	StatementBlock& body() const
	{
		return *block;
	}

protected:
	std::string name_;
	std::string base_;

	uptr<StatementBlock> block;
};

struct Instance : Declaration {
	Instance(StringList&& names, std::string base,
	          uptr<StatementBlock> body)
		: Declaration(Declaration::Instance),
		  names(names), base_(base), block(std::move(body))
	{ }

	virtual ~Instance() = default;

	std::string name() const
	{
		return names.front();
	}

	StringList const& nameList() const
	{
		return names;
	}

	std::string base() const
	{
		return base_;
	}

	StatementBlock* body() const
	{
		return block.get();
	}

protected:
	StringList names;
	std::string base_;

	uptr<StatementBlock> block;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Declaration
