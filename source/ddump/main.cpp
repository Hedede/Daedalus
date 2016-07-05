/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <fstream>
#include <iostream>
#include <iomanip>
#include "zCPar_Symbol.h"
#include "Instruction.h"
namespace daedalus {
//using namespace aw;

int main(char* argv)
{
	std::fstream file{argv};

	char version;
	file.read((char*)&version,1);
	std::cout << "Version: " << unsigned(version) << "\n";

	unsigned num_symbols;
	file.read((char*)&num_symbols,4);
	std::cout << "Symbols: " << num_symbols << "\n";

	zCPar_SymbolTable symtab;
	for (unsigned i = 0; i < num_symbols; ++i) {
		unsigned idx;
		file.read((char*)&idx, 4);
		symtab.indices.push_back(idx);
	}

	for (unsigned i = 0; i < num_symbols; ++i)
		symtab.addSymbol(readSymbol(file));

	printSymbols(std::cout, symtab);

	unsigned stacksize;
	std::vector<Instruction> ops;
	{
		std::vector<char> stack;

		file.read((char*)&stacksize,4);

		stack.resize(stacksize);
		file.read(stack.data(), stacksize);

		char const* begin = stack.data();
		char const* end   = stack.data() + stacksize;

		while (begin != end)
			ops.push_back(readOp(begin, end));
	}

	std::cout << "------ STACK ------\n";
	std::cout << "stack size: " << stacksize << "\n";

	unsigned adr = 0;
	size_t width = std::to_string(stacksize).size();

	auto osflags = std::cout.flags();
	auto oswidth = std::cout.width();
	auto osfill  = std::cout.fill();
	for (auto op : ops) {
		auto func = symtab.findFunc(adr);
		if (!func.empty())
			std::cout << func << ":\n";

		std::cout << std::setfill('0') << std::setw(width) << adr << ": ";

		std::cout.flags(osflags);
		std::cout.width(oswidth);
		std::cout.fill(osfill);

		printOp(std::cout, op);
		printRef(std::cout, op, symtab);

		adr += opSize(op.op);
		std::cout << "\n";
	};
}
} // namespace daedalus

int main(int, char** argv)
{
	using namespace std::string_literals;
	if (!argv[1]) return 1;
	return daedalus::main(argv[1]);
}
