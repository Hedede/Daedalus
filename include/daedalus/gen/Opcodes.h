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
} // namespace daedalus
#endif//Daedalus_Opcodes
