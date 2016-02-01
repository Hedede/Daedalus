/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Stmt_Block
#define Daedalus_Stmt_Block
#include <vector>
#include <daedalus/syntax/Statement.h>

namespace daedalus {
namespace tree {
class StatementBlock : public Statement {
public:
	StatementBlock(std::vector<uptr<Statement>> statements)
		: statements(std::move(statements))
	{
	}

	virtual ~StatementBlock() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::vector<uptr<Statement>>& getStatements()
	{
		return statements;
	}
private:
	std::vector<std::unique_ptr<Statement>> statements;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Stmt_Block
