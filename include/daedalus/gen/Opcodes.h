/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Opcodes
#define Daedalus_Opcodes
namespace daedalus {
enum class Opcode {
	Add = 0,
	Sub = 1,
	Mul = 2,
	Div = 3,
	Mod = 4,
	Or  = 5,
	And = 6,
	Less    = 7,
	Greater = 8,
	Assign  = 9,
	LogicalOr  = 11,
	LogicalAnd = 12,
	ShiftL = 13,
	ShiftR = 14,
	Leq = 15,
	Eq  = 16,
	Neq = 17,
	Geq = 18,
	AssignAdd = 19,
	AssignSub = 20,
	AssignMul = 21,
	AssignDiv = 22,
	Plus  = 30,
	Minus = 31,
	Not   = 32,
	Neg   = 33,
	Ret   = 60,
	Call        = 61,
	CallExtern  = 62,
	PushInt     = 64,
	PushAdr     = 65,
	PushStr     = 66,
	PushInst    = 67,
	AssignStr   = 70,
	AssignStrp  = 71,
	AssignFunc  = 72,
	AssignFloat = 73,
	AssignInst  = 74,
	Jmp         = 75,
	JmpFalse    = 76,
	SelectInst  = 80,
	ArrayVar    = 0xF5,
};

std::string opcodeString(Opcode op)
{
	switch (op) {
		case Opcode::Add:         return "Add";
		case Opcode::Sub:         return "Sub";
		case Opcode::Mul:         return "Mul";
		case Opcode::Div:         return "Div";
		case Opcode::Mod:         return "Mod";
		case Opcode::Or:          return "Or";
		case Opcode::And:         return "And";
		case Opcode::Less:        return "Less";
		case Opcode::Greater:     return "Greater";
		case Opcode::Assign:      return "Assign";
		case Opcode::LogicalOr:   return "LogicalOr";
		case Opcode::LogicalAnd:  return "LogicalAnd";
		case Opcode::ShiftL:      return "ShiftL";
		case Opcode::ShiftR:      return "ShiftR";
		case Opcode::Leq:         return "Leq";
		case Opcode::Eq:          return "Eq";
		case Opcode::Neq:         return "Neq";
		case Opcode::Geq:         return "Geq";
		case Opcode::AssignAdd:   return "AssignAdd";
		case Opcode::AssignSub:   return "AssignSub";
		case Opcode::AssignMul:   return "AssignMul";
		case Opcode::AssignDiv:   return "AssignDiv";
		case Opcode::Plus:        return "Plus";
		case Opcode::Minus:       return "Minus";
		case Opcode::Not:         return "Not";
		case Opcode::Neg:         return "Neg";
		case Opcode::Ret:         return "Ret";
		case Opcode::Call:        return "Call";
		case Opcode::CallExtern:  return "CallExtern";
		case Opcode::PushInt:     return "PushInt";
		case Opcode::PushAdr:     return "PushAdr";
		case Opcode::PushStr:     return "PushStr";
		case Opcode::PushInst:    return "PushInst";
		case Opcode::AssignStr:   return "AssignStr";
		case Opcode::AssignStrp:  return "AssignStrp";
		case Opcode::AssignFunc:  return "AssignFunc";
		case Opcode::AssignFloat: return "AssignFloat";
		case Opcode::AssignInst:  return "AssignInst";
		case Opcode::Jmp:         return "Jmp";
		case Opcode::JmpFalse:    return "JmpFalse";
		case Opcode::SelectInst:  return "SelectInst";
		case Opcode::ArrayVar:    return "ArrayVar";
		default:                  return "UnknownOp";
	}
}
} // namespace daedalus
#endif//Daedalus_Opcodes
