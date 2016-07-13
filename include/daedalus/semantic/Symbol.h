/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Symbol
#define Daedalus_Symbol
#include <vector>
#include <limits>
#include <daedalus/semantic/Type.h>
namespace daedalus {
struct Symbol {
	static constexpr unsigned undefined = std::numeric_limits<unsigned>::max();

	enum Kind {
		Undefined,
		Variable,
		Function,
		Class,
		Instance,
		Prototype,
		NumKinds
	};

	Symbol(std::string name, Kind kind)
		: name(name), kind(kind)
	{}

	std::string name;
	Kind kind;
};

struct SymbolRef {
	SymbolRef()
		: kind(Symbol::Undefined)
	{}

	SymbolRef(Symbol::Kind kind)
		: kind(kind), scopeId{0}, index{0}
	{}

	SymbolRef(Symbol::Kind kind, size_t scope, size_t index)
		: kind(kind), scopeId(scope), index(index)
	{}

	bool isValid()
	{
		return kind != Symbol::Undefined;
	}

	aw::u64 kind    : 3;
	aw::u64 scopeId : 24;
	aw::u64 index   : 29;
};

using SymbolRefList = std::vector<SymbolRef>;

struct Variable : Symbol {
	Variable(std::string name, Type type)
		: Symbol(name, Symbol::Variable), type(type)
	{}
	Type type;
};

struct Function : Symbol {
	Function(std::string name, Type ret)
		: Symbol(name, Symbol::Function), returnType(ret)
	{}

	Type returnType;
	std::vector<unsigned> args;
	unsigned scope = Symbol::undefined;
};

struct Prototype : Symbol {
	SymbolRef parent;
	unsigned scope = Symbol::undefined;
};

struct Instance : Symbol {
	SymbolRef parent;
	unsigned scope = Symbol::undefined;
};

enum class TypeID {
	Void,
	Integer,
	Float,
	String,
	FuncRef,
	Function,
	Class
};

struct Class : Symbol {
	Class(std::string name, TypeID id)
		: Symbol(name, Symbol::Class), id(id)
	{ }

	Class(std::string name, unsigned scope)
		: Symbol(name, Symbol::Class), id(TypeID::Class), scope(scope)
	{ }

	TypeID id;
	unsigned scope = Symbol::undefined;
};
} // namespace daedalus
#endif//Daedalus_Symbol
