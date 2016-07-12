/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Type
#define Daedalus_Type
//#include <daedalus/semantic/Symbol.h>
namespace daedalus {
class Class;

/*!
 * Reference to a type, with added type modifiers.
 */
struct Type {
	Class* baseType;
	bool isConst;
	unsigned count;
};

} // namespace daedalus
#endif//Daedalus_Type
