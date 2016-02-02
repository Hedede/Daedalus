/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/utility/string.h>
#include <daedalus/lexer/KeywordMap.h>
namespace daedalus {
constexpr bool Case_Insensitive_Keywords = true;
// Link token to a keyword
KeywordMap& KeywordMap::add(std::string def, TokenType kind)
{
	theMap[def] = kind;
	return *this;
}

// Get token type from string
TokenType KeywordMap::get(std::string def)
{
	if (Case_Insensitive_Keywords)
		string::tolower(def);

	auto result = theMap.find(def);

	if (result == std::end(theMap))
		// TODO: better indication that it is not present
		return tok_illegal;

	return theMap[def];
}

// Check if keyword is present
bool KeywordMap::isKeyword(std::string def)
{
	return get(def) != tok_illegal;
}
} // namespace daedalus
