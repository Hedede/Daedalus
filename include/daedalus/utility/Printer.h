/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Printer
#define Daedalus_Printer
#include <daedalus/io/WriteStream.h>
#include <daedalus/syntax/Visitor.h>

namespace daedalus {
class Printer : public tree::Visitor {
public:
	Printer(io::WriteStream& out);
	virtual ~Printer() = default;

	virtual void visit(tree::FunctionProto& node);
	virtual void visit(tree::Function& node);
	virtual void visit(tree::Variable& node);
	virtual void visit(tree::Class& node);
	virtual void visit(tree::LocalDecl& node);
	virtual void visit(tree::StatementBlock& node);
	virtual void visit(tree::IfElseStatement& node);
	virtual void visit(tree::ReturnStatement& node);
	virtual void visit(tree::NumberExpr& node);
	virtual void visit(tree::StringExpr& node);
	virtual void visit(tree::IdentifierExpr& node);
	virtual void visit(tree::ArrayInitializer& node);
	virtual void visit(tree::CallExpr& node);
	virtual void visit(tree::FieldExpr& node);
	virtual void visit(tree::SubscriptExpr& node);
	virtual void visit(tree::UnaryExpr& node);
	virtual void visit(tree::BinaryExpr& node);

private:
	void printSignature(tree::FunctionProto& node);
	void startInline(std::string name);
	void endInline();
	void start(std::string name = "");
	void end();
	void startLine();
	void endLine();

	io::WriteStream& writer;
	size_t depth = 0;
};
} // namespace daedalus
#endif//Daedalus_Printer
