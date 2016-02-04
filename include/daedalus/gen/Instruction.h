/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_CG_Instruction
#define Daedalus_CG_Instruction
#include <daedalus/gen/Opcodes.h>
namespace daedalus {
class Instruction {
public:
	Opcode opcode()
	{
		return op;
	}
private:
	Opcode op;
};

/*
 * kinds:
 * Binary-op — pop 2, push result
 * Unary-op  — pop 1, push result
 * Assign    — pop 2, push none
 * SeInst    — pop 1, push none
 * Push      [arg] — push arg
 * PushArray [arg][idx] — push arg[idx]
 * Call/Jmp  [arg] — jump to arg
 * JmpF      [arg] — pop 1, jump to arg if 0
 * Ret        — doesnt interact with stack
 */
} // namespace daedalus
#endif//Daedalus_CG_Instruction
