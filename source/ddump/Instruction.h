/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Instruction
#define Daedalus_Instruction
#include <iomanip>
#include <aw/types/types.h>
#include <daedalus/gen/Opcodes.h>
#include "zCPar_Symbol.h"

namespace daedalus {
struct Instruction {
	Opcode op;
	aw::u32 operand1;
	aw::u8  operand2;
};

Instruction readOp(char const*& begin, char const* end)
{
	Instruction i;
	i.op = Opcode(*(aw::u8*)begin++);
	
	switch (i.op) {
	default:
		break;
	case Opcode::Call:
	case Opcode::CallExtern:
	case Opcode::PushAdr:
	case Opcode::SelectInst:
	case Opcode::PushInst:
	case Opcode::PushInt:
	case Opcode::PushStr:
	case Opcode::Jmp:
	case Opcode::JmpFalse:
		i.operand1 = *(aw::u32*)begin;
		begin += 4;
		break;
	case Opcode::ArrayVar:
		i.operand1 = *(aw::u32*)begin;
		begin += 4;
		i.operand2 = *(aw::u8*)begin++;
		break;
	};
	return i;
}

void printOp(std::ostream& os, Instruction const& i)
{
	auto osflags = os.flags();
	auto oswidth = os.width();
	auto osfill  = os.fill();

	os << std::hex << std::setw(2) << std::setfill('0') << unsigned(i.op);

	os.flags(osflags);
	os.width(oswidth);
	os.fill(osfill);

	os << ": " << opcodeString(i.op);

	switch (i.op) {
	default:
		break;
	case Opcode::Call:
	case Opcode::CallExtern:
	case Opcode::PushAdr:
	case Opcode::SelectInst:
	case Opcode::PushInst:
	case Opcode::PushInt:
	case Opcode::PushStr:
	case Opcode::Jmp:
	case Opcode::JmpFalse:
		os << " " << i.operand1;
		break;
	case Opcode::ArrayVar:
		os << " " << i.operand1;
		os << "[" << unsigned(i.operand2) << "]";
		break;
	};
}

unsigned opSize(Opcode op)
{
	switch (op) {
	default:
		return 1;
	case Opcode::Call:
	case Opcode::CallExtern:
	case Opcode::PushAdr:
	case Opcode::SelectInst:
	case Opcode::PushInst:
	case Opcode::PushInt:
	case Opcode::PushStr:
	case Opcode::Jmp:
	case Opcode::JmpFalse:
		return 5;
	case Opcode::ArrayVar:
		return 6;
	};
}

void printRef(std::ostream& os, Instruction const& i, zCPar_SymbolTable const& symtab)
{
	switch (i.op) {
	case Opcode::PushAdr:
	case Opcode::SelectInst:
	case Opcode::PushInst:
	case Opcode::PushStr:
	case Opcode::ArrayVar:
	case Opcode::CallExtern:
		if (i.operand1 < symtab.syms.size()) {
			auto& sym = symtab.syms[i.operand1];
			os << " (" << sym.name << ")";
		}
		break;
	case Opcode::Call:
	case Opcode::Jmp:
	case Opcode::JmpFalse: {
		auto func = symtab.findFunc(i.operand1);
		if (func.empty())
			break;
		os << " (" << func << ")";
		}
	case Opcode::PushInt:
	default:
		break;
	};
}

} // namespace daedalus
#endif//Daedalus_Instruction
