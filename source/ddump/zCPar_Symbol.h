/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_zCPar_Symbol
#define Daedalus_zCPar_Symbol
#include <string>
#include <vector>
#include <unordered_map>
#include <istream>
#include <ostream>
#include <aw/types/types.h>
#include <daedalus/gen/Opcodes.h>

enum zPAR_FLAGS : aw::u8 {
	zPAR_FLAG_CONST       = 1,
	zPAR_FLAG_RETURN      = 2,
	zPAR_FLAG_CLASSVAR    = 4,
	zPAR_FLAG_EXTERNAL    = 8,
	zPAR_FLAG_MERGED      = 16,
	zPAR_FLAG_ALLOC_SPACE = 64,
};

enum zPAR_TYPE : aw::u8 {
	zPAR_TYPE_VOID,
	zPAR_TYPE_FLOAT,
	zPAR_TYPE_INT,
	zPAR_TYPE_STRING,
	zPAR_TYPE_CLASS,
	zPAR_TYPE_FUNC,
	zPAR_TYPE_PROTOTYPE,
	zPAR_TYPE_INSTANCE
};

/*!
 * Not actual representation of zCPar_Symbol.
 * Used only to store data read from compiled .dat files.
 */
struct zCPar_Symbol {
	std::string name;

	aw::u8 flags;
	aw::u8 type;
	aw::u16 count;

	aw::u32 file_no;
	aw::u32 line;
	aw::u32 line_count;
	aw::u32 pos;
	aw::u32 length;

	aw::i32 offset;

	aw::i32 adr;

	std::vector<std::string> data;

	aw::i32 parent;
};

void readSymbolData(std::istream& is, zCPar_Symbol& sym)
{
	size_t count = sym.count;
	switch (sym.type) {
	case zPAR_TYPE_FLOAT:
		while (count --> 0) {
			float data;
			is.read((char*)&data, 4);
			sym.data.push_back(std::to_string(data));
		}
		break;
	case zPAR_TYPE_INT:
		while (count --> 0) {
			int data;
			is.read((char*)&data, 4);
			sym.data.push_back(std::to_string(data));
		}
		break;
	case zPAR_TYPE_STRING:
		for (auto i = 0u; i < count; ++i) {
			std::string data;
			std::getline(is, data, '\n');
			sym.data.push_back(data);
		}
		break;
	default:
		is.read((char*)&sym.adr, 4);
	};
}

zCPar_Symbol readSymbol(std::istream& is)
{
	int has_name;
	is.read((char*)&has_name, 4);

	std::string name;
	if (has_name)
		std::getline(is, name, '\n');

	int offset;
	aw::u16 type_ele;
	aw::u16 flags;
	aw::u32 file;
	aw::u32 line;
	aw::u32 line_count;
	aw::u32 pos;
	aw::u32 size;

	is.read((char*)&offset,4);
	is.read((char*)&type_ele,2);
	is.read((char*)&flags,2);
	is.read((char*)&file,4);
	is.read((char*)&line,4);
	is.read((char*)&line_count,4);
	is.read((char*)&pos,4);
	is.read((char*)&size,4);

	aw::u16 ele  =  type_ele & 0x0FFF;
	aw::u8  type = (type_ele & 0xF000) >> 12;

	zCPar_Symbol sym {
		name,
		aw::u8(flags),
		type,
		ele,
		file,
		line,
		line_count,
		pos,
		size
	};

	if (!(flags & zPAR_FLAG_CLASSVAR))
		readSymbolData(is, sym);

	int parent;
	is.read((char*)&parent,4);

	sym.parent = parent;

	return sym;
}

struct zCPar_SymbolTable {
	std::vector<zCPar_Symbol> syms;
	std::vector<unsigned> indices;
	std::unordered_map<int, size_t> funcMap;

	std::string findFunc(int offset)
	{
		auto found = funcMap.find(offset);
		if (found != std::end(funcMap))
			return syms[found->second].name;
		return "";
	}

	void addSymbol(zCPar_Symbol sym)
	{
		syms.push_back(sym);

		switch (sym.type) {
		case zPAR_TYPE_FUNC:
			if (!(sym.flags & zPAR_FLAG_CONST))
				return;
			if (sym.flags & zPAR_FLAG_EXTERNAL)
				return;
		case zPAR_TYPE_PROTOTYPE:
		case zPAR_TYPE_INSTANCE:
			funcMap[sym.adr] = syms.size() - 1;
		}
	}
};

void printSymbolData(std::ostream& os, zCPar_Symbol const& sym)
{
	size_t count = sym.count;
	switch (sym.type) {
	case zPAR_TYPE_FLOAT:
	case zPAR_TYPE_INT:
	case zPAR_TYPE_STRING:
		if (count > 1) {
			os << "{";
			for (auto i = 0u; i < count; ++i) {
				os << sym.data[i];
				if (i < count - 1)
					os << ", ";
			}
			os << "}";
		} else if (count) {
			os << sym.data[0];
		}
		break;
	default:
		os << sym.adr;
		break;
	};
}

void printSymbol(std::ostream& os, zCPar_Symbol const& sym)
{
	os << "Name: "   << sym.name << "\n";
	os << "Offset: " << sym.offset << "\n";
	os << "Flags: ";
	if (sym.flags & zPAR_FLAG_CONST)
		os << "[C]";
	if (sym.flags & zPAR_FLAG_RETURN)
		os << "[R]";
	if (sym.flags & zPAR_FLAG_CLASSVAR)
		os << "[CV]";
	if (sym.flags & zPAR_FLAG_EXTERNAL)
		os << "[E]";
	if (sym.flags & zPAR_FLAG_MERGED)
		os << "[M]";
	if (sym.flags & 32)
		os << "[?]";
	if (sym.flags & zPAR_FLAG_ALLOC_SPACE)
		os << "[A]";
	os << "\n";

	os << "Type: " << unsigned(sym.type) << " (";
	switch (sym.type) {
	case 1: os << "float"; break;
	case 2: os << "int"; break;
	case 3: os << "string"; break;
	case 4: os << "class"; break;
	case 5: os << "func"; break;
	case 6: os << "prototype"; break;
	case 7: os << "instance"; break;
	}
	os << ")\n";
	os << "Count: " << sym.count << "\n";

	if (!(sym.flags & zPAR_FLAG_CLASSVAR)) {
		os << "Value: ";
		printSymbolData(os, sym);
		os << "\n";
	}

	if (sym.parent != -1)
		std::cout << "Parent: " << sym.parent << "\n";
}

void printSymbols(std::ostream& os, zCPar_SymbolTable const& table)
{
	unsigned i = 0;
	for (auto& sym : table.syms) {
		std::cout << "------ Symbol " << i++ << " ------\n";
		printSymbol(os, sym);
	}
}

#endif//Daedalus_zCPar_Symbol
