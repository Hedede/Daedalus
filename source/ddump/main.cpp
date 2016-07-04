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

	auto& funcs = symtab.funcMap;
	auto& syms  = symtab.syms;
	int stacksize;
	file.read((char*)&stacksize,4);
	std::cout << "------ STACK ------\n";
	std::cout << "stack size: " << stacksize << "\n";
	size_t width = std::to_string(stacksize).size();
	for (int i = 0; i < stacksize;) {
		unsigned char byte;
		file.read((char*)&byte,1);
		
		auto func = symtab.findFunc(i);
		if (!func.empty())
			std::cout << func << ":\n";

		using namespace std;
		Opcode op{Opcode(byte)};
		std::cout << setfill('0') << setw(width) << i << ": ";
		std::cout << std::hex << setw(2) << int(op) << ": ";
		std::cout << std::dec << setfill(' ') << setw(0) << opcodeString(op);

		switch (op) {
		default:
			++i;
			break;
		case Opcode::Call:
			{
				int value;
				file.read((char*)&value,4);
				std::cout << " " << value;
				std::cout << " (" << symtab.findFunc(value) << ")";
				i += 1 + 4;
			}
			break;
		case Opcode::CallExtern:
		case Opcode::PushAdr:
		case Opcode::SelectInst:
			{
				unsigned value;
				file.read((char*)&value,4);
				std::cout << " " << value;
				std::cout << " (" << syms[value].name << ")";
				i += 1 + 4;
			}
			break;
		case Opcode::PushInt:
		case Opcode::PushStr:
		case Opcode::PushInst:
		case Opcode::Jmp:
		case Opcode::JmpFalse:
			{
				int value;
				file.read((char*)&value,4);
				std::cout << " " << value;
				i += 1 + 4;
			}
			break;
		case Opcode::ArrayVar:
			{
				int value;
				char value2;
				file.read((char*)&value,4);
				file.read((char*)&value2,1);
				std::cout << " " << value;
				std::cout << " (" << syms[value].name << ")";
				std::cout << " " << unsigned(value2);
				i += 1 + 4 + 1;
			}
		};

		std::cout << "\n";
	}
}
} // namespace daedalus

int main(int, char** argv)
{
	using namespace std::string_literals;
	if (!argv[1]) return 1;
	return daedalus::main(argv[1]);
}
