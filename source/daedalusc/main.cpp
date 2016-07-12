/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <iostream>
#include <aw/io/WriteStream.h>
#include <daedalus/syntax/Declaration.h>
#include <daedalus/syntax/Expression.h>
#include <daedalus/semantic/SymbolTable.h>
#include <daedalus/utility/Printer.h>
#include <daedalus/utility/DiagnosticHelper.h>
#include <daedalus/parser/Parser.h>
#include "OUGen.h"
namespace daedalus {
using namespace aw;
class TestWriteStream : public io::WriteStream {
public:
	virtual i32 put(char c)
	{
		std::cout.put(c);
		return 0;
	}

	virtual i32 put(std::string str)
	{
		std::cout << str;
		return 0;
	}

	virtual size_t position() const
	{
		return 0;
	}
};

int main(char** argv)
{
	if (argv[1] == 0)
		return 1;

	std::cout << argv[1] << std::endl;
	io::ReadFile file(argv[1]);
	SourceBuffer buffer(file);
	Lexer lexer(&buffer);
	DiagnosticHelper diag(buffer);
	SymbolTable symtab;
	Parser parser(lexer, diag, symtab);

	TestWriteStream out;
	Printer printer(out);
	std::vector<uptr<tree::Declaration>> decls;
	auto decl = parser.parseDeclaration();
	while (decl) {
		printer.visit(*decl);
		decls.push_back(std::move(decl));
		decl = parser.parseDeclaration();
	}
	while (true);

	return 0;
}
} // namespace daedalus


int main(int, char** argv)
{
	using namespace std::string_literals;
	if (!argv[1]) return 1;
	if (argv[1] == "-ou"s) return daedalus::ou_gen(argv+1);
	if (argv[1] == "-dd"s) return daedalus::main(argv+1);
	return daedalus::main(argv);
}
