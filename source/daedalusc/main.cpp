/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <iostream>
#include <daedalus/io/WriteStream.h>
#include <daedalus/utility/Printer.h>
#include <daedalus/utility/DiagnosticHelper.h>
#include <daedalus/parser/Parser.h>

namespace daedalus {
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

	ReadFile file(argv[1]);
	SourceBuffer buffer(file);
	Lexer lexer(&buffer);
	DiagnosticHelper diag;
	Parser parser(lexer, diag);

	TestWriteStream out;
	Printer printer(out);
	auto decl = parser.parseDeclaration();
	while (decl) {
		decl->accept(printer);
		decl = parser.parseDeclaration();
	}

	return 0;
}
} // namespace


int main(int, char** argv)
{
	return daedalus::main(argv);
}
