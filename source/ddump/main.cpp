/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <daedalus/gen/Opcodes.h>
namespace daedalus {
//using namespace aw;

std::unordered_map<unsigned,std::string> funcs;
std::vector<std::string> syms;

void printSym(std::istream& is)
{
	int unk;
	std::string name;
	int offset;
	short type_ele;
	short flags;
	int file, line, line_count, pos, size;

	is.read((char*)&unk,4);
	std::getline(is, name, '\n');
	is.read((char*)&offset,4);
	is.read((char*)&type_ele,2);
	is.read((char*)&flags,2);
	is.read((char*)&file,4);
	is.read((char*)&line,4);
	is.read((char*)&line_count,4);
	is.read((char*)&pos,4);
	is.read((char*)&size,4);

	syms.push_back(name);

	int type;
	int ele;

	ele  = (type_ele & 0x0FFF);
	type = (type_ele & 0x7000) >> 12;

	std::cout << "Name: " << name << "\n";
	std::cout << "Offset: " << offset << "\n";
	std::cout << "Flags: ";
	if (flags & 1)
		std::cout << "[C]";
	if (flags & 2)
		std::cout << "[R]";
	if (flags & 4)
		std::cout << "[CV]";
	if (flags & 8)
		std::cout << "[E]";
	if (flags & 16)
		std::cout << "[M]";
	if (flags & 32)
		std::cout << "[?]";
	if (flags & 64)
		std::cout << "[A]";
	std::cout << "\n";

	std::cout << "Type: " << type << " (";
	switch (type) {
	case 1: std::cout << "float"; break;
	case 2: std::cout << "int"; break;
	case 3: std::cout << "string"; break;
	case 4: std::cout << "class"; break;
	case 5: std::cout << "func"; break;
	case 6: std::cout << "prototype"; break;
	case 7: std::cout << "instance"; break;
	}
	std::cout << ")\n";
	std::cout << "Count: " << ele << "\n";

	if (!(flags & 4)) {
	std::cout << "Value: ";
	switch (type) {
	case 1:
		for (unsigned i = 0; i < ele; ++i) {
			float value;
			is.read((char*)&value,4);
			std::cout << value;
			if (i < ele - 1)
				std::cout << ", ";
		}
		break;
	case 2:
		for (unsigned i = 0; i < ele; ++i) {
			int value;
			is.read((char*)&value,4);
			std::cout << value;
			if (i < ele - 1)
				std::cout << ", ";
		}
		break;
	case 3:
		for (unsigned i = 0; i < ele; ++i) {
			std::string value;
			std::getline(is, value, '\n');
			std::cout << value;
			if (i < ele - 1)
				std::cout << ", ";
		}
		break;
	case 5:
	case 6:
	case 7:
		{
		unsigned value;
		is.read((char*)&value,4);
		if ((flags & 1) && !(flags & 8))
			funcs[value] = name;
		std::cout << value;
		break;
		}
	case 4:
		{
		int value;
		is.read((char*)&value,4);
		std::cout << value;
		break;
		}
	}
	std::cout << "\n";
	}

	int parent;
	is.read((char*)&parent,4);
	if (parent != -1)
		std::cout << "Parent: " << parent << "\n";
}

int main(char* argv)
{
	std::fstream file{argv};

	char version;
	file.read((char*)&version,1);
	std::cout << "version: " << unsigned(version) << "\n";

	unsigned num_symbols;
	file.read((char*)&num_symbols,4);
	std::cout << "symbols: " << num_symbols << "\n";

	std::vector<unsigned> indices;
	for (unsigned i = 0; i < num_symbols; ++i) {
		unsigned idx;
		file.read((char*)&idx, 4);
		indices.push_back(idx);
	}

	for (unsigned i = 0; i < num_symbols; ++i) {
		std::cout << "------ Symbol " << i << " ------\n";
		// std::cout << "Sorted pos: " << std::find(std::begin(indices), std::end(indices), i) - std::begin(indices) << "\n";
		printSym(file);
	}

	unsigned stacksize;
	file.read((char*)&stacksize,4);
	std::cout << "------ STACK ------\n";
	std::cout << "stack size: " << stacksize << "\n";
	size_t width = std::to_string(stacksize).size();
	for (unsigned i = 0; i < stacksize;) {
		unsigned char byte;
		file.read((char*)&byte,1);
		
		auto found = funcs.find(i);
		if (found != std::end(funcs))
			std::cout << found->second << ":\n";

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
				unsigned value;
				file.read((char*)&value,4);
				std::cout << " " << value;
				std::cout << " (" << funcs[value] << ")";
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
				std::cout << " (" << syms[value] << ")";
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
				std::cout << " (" << syms[value] << ")";
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
