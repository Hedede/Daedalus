/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Lexer_KeywordMap
#define Daedalus_Lexer_KeywordMap
#include <map>
#include <string>
#include <daedalus/lexer/Token.h>
namespace daedalus {
/*!
 * Helper class for keyword lookup
 */
class KeywordMap {
public:
	/*!
	 * Add a keyword to the map.
	*/
	KeywordMap& add(std::string def, Token::Kind kind);
	/*!
	 * Get token type from string.
	 */ 
	Token::Kind get(std::string def);
	/*!
	 * Check if a string is keyword in the map.
	 */
	bool isKeyword(std::string def);
private:
	std::map<std::string, Token::Kind> theMap;
};
} // namespace daedalus
#endif//Daedalus_Lexer_KeywordMap
