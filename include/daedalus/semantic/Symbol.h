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
namespace daedalus {
class Declaration;
struct Symbol {
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

struct Variable : Symbol { };
struct Function : Symbol { };
struct Prototype : Symbol { };
struct Instance : Symbol { };

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

	TypeID id;
};
} // namespace daedalus
#endif//Daedalus_Symbol
