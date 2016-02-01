/*
 * Copyright (C) 2015  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Diagnostic
#define Daedalus_Diagnostic
#include <daedalus/utility/PrintToken.h>
namespace daedalus {
//! TODO
struct Location {
};

class DiagnosticsEngine;

/*!
 * This class is used to compose diagnostic messages
 */
class Diagnostic {
public:
	enum ID {
#define DIAG(type, id, msg) id,
#include <daedalus/utility/Messages.h>
#undef DIAG
	};

	Diagnostic(Location loc, ID id)
		: id(id), loc(loc)
	{
	}

	Diagnostic& arg(std::string str)
	{
		args.push_back(str);
		return *this;
	}
private:
	friend class DiagnosticHelper;

	Location loc;
	ID id;
	std::vector<std::string> args;
};

std::string getDiagMsg(Diagnostic::ID id)
{
	switch (id) {
	// TODO: array
#define DIAG(type, id, msg) case Diagnostic::id: return msg;
#include <daedalus/utility/Messages.h>
#undef DIAG
	}
}

Diagnostic& operator << (Diagnostic& diag, std::string str)
{
	return diag.arg(str);
}

Diagnostic& operator << (Diagnostic& diag, TokenType type)
{
	return diag.arg(spellToken(type));
}
} // namespace daedalus
#endif//Daedalus_Diagnostic
