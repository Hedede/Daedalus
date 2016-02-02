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
class Class : public Declaration {
public:
	static uptr<Class> create(std::string name,
	                std::vector<uptr<tree::Variable>> members)
	{
		auto tmp = new Class(name, std::move(members));
		return uptr<Class>(tmp);
	}

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
	Class(std::string name, std::vector<uptr<tree::Variable>> members)
		: Declaration(Declaration::Class),
		  name(name), body(std::move(members))
	{
	}

	std::string name;
	std::vector<uptr<tree::Variable>> body;
};

class Prototype : public Declaration {
public:
	static uptr<Prototype> create(
	                std::string name, std::string base,
	                uptr<StatementBlock> body)
	{
		auto tmp = new Prototype(name, base, std::move(body));
		return uptr<Prototype>(tmp);
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
	Prototype(std::string name, std::string base,
	          uptr<StatementBlock> body)
		: Declaration(Declaration::Prototype),
		  name_(name), base_(base), block(std::move(body))
	{
	}

	std::string name_;
	std::string base_;

	uptr<StatementBlock> block;
};

class Instance : public Declaration {
public:
	static uptr<Instance> create(
	                std::string name, std::string base,
	                uptr<StatementBlock> body)
	{
		auto tmp = new Instance(name, base, std::move(body));
		return uptr<Instance>(tmp);
	}
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
	Instance(std::string name, std::string base,
	          uptr<StatementBlock> body)
		: Declaration(Declaration::Instance),
		  name_(name), base_(base), block(std::move(body))
	{
	}

	std::string name_;
	std::string base_;

	uptr<StatementBlock> block;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Class
