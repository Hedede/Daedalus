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

typedef std::vector<uptr<Declaration>> DeclarationList;

class Expression;

class Variable : public Declaration {
public:
	Variable(std::string id, bool is_const)
		: Declaration(Declaration::Variable),
		  id(id), constant(is_const)
	{ }

	virtual ~Variable() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::string name() const
	{
		return id;
	}

	bool isConst()
	{
		return constant;
	}

	virtual void setConst(bool cnst)
	{
		constant = cnst;
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
	bool constant;
	uptr<Expression> init_expr = nullptr;
	uptr<Expression> size_expr = nullptr;
};

typedef std::vector<std::unique_ptr<Variable>> VarList;

// TODO: merge with Function
class FunctionProto : public Declaration {
public:
	FunctionProto(std::string id, std::string returnType, VarList args)
		: Declaration(Declaration::FunctionProto),
		  name(id), args(std::move(args)), returnType(returnType)
	{ }

	virtual ~FunctionProto() = default;

	virtual std::string getName() const
	{
		return name;
	}

	virtual std::string getReturnType() const
	{
		return returnType;
	}

	VarList& getArguments()
	{
		return args;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

private:
	std::string name;
	std::string returnType;
	VarList args;
};

class Function : public Declaration {
public:
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

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

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

	virtual void accept(Visitor& visitor)
	{
		visitor.visit(*this);
	}

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

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

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

class Instance : public Declaration {
public:
	Instance(std::string name, std::string base,
	          uptr<StatementBlock> body)
		: Declaration(Declaration::Instance),
		  name_(name), base_(base), block(std::move(body))
	{ }

	virtual ~Instance() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::string name() const
	{
		return name_;
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
	std::string name_;
	std::string base_;

	uptr<StatementBlock> block;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Declaration
