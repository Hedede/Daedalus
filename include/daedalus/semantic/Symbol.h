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
		Variable,
		Function,
		Class,
		Instance,
		Prototype,
		NumKinds
	};

	std::string name;
	Kind kind;
};

struct Variable : Symbol { };
struct Function : Symbol { };
struct Class : Symbol { };
struct Prototype : Symbol { };
struct Instance : Symbol { };
} // namespace daedalus
#endif//Daedalus_Symbol
