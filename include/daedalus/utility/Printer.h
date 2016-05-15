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
#include <aw/io/WriteStream.h>
namespace daedalus {
using namespace aw;
class Printer {
public:
	Printer(io::WriteStream& out);
	~Printer() = default;

	void visit(tree::Declaration& node);

	void visit(tree::FunctionProto& node);
	void visit(tree::Function& node);
	void visit(tree::Variable& node);
	void visit(tree::Class& node);
	void visit(tree::Prototype& node);
	void visit(tree::Instance& node);

	void visit(tree::Statement& node);

	void visit(tree::DeclStatement& node);
	void visit(tree::StatementBlock& node);
	void visit(tree::IfStatement& node);
	void visit(tree::ReturnStatement& node);
	void visit(tree::WhileStatement& node);
	void visit(tree::DoStatement& node);
	void visit(tree::BreakStatement& node);
	void visit(tree::ContinueStatement& node);

	void visit(tree::Expression& node);

	void visit(tree::NumberExpr& node);
	void visit(tree::StringExpr& node);
	void visit(tree::IdentifierExpr& node);
	void visit(tree::ArrayInitializer& node);
	void visit(tree::CallExpr& node);
	void visit(tree::FieldExpr& node);
	void visit(tree::SubscriptExpr& node);
	void visit(tree::UnaryExpr& node);
	void visit(tree::BinaryExpr& node);

private:
	void printSignature(tree::FunctionProto& node);
	void write(char c);
	void write(std::string s);
	void writeInline(char c);
	void writeInline(std::string s);
	void startInline(std::string name);
	void endInline();
	void start(std::string name = "");
	void end();
	void startLine();
	void endLine();

	io::WriteStream& writer;
	size_t depth = 0;
	enum State {
		LineStart,
		Middle,
		LineEnd,
	} state;
};
} // namespace daedalus
#endif//Daedalus_Printer
