/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/syntax/Declaration.h>
#include <daedalus/syntax/Statement.h>
#include <daedalus/syntax/Expression.h>

#include <daedalus/utility/Printer.h>
#include <daedalus/utility/PrintToken.h>
namespace daedalus {
using namespace aw;
Printer::Printer(io::WriteStream& out)
	: writer(out)
{
}

void Printer::writeInline(char c)
{
	writer.put(c);
	state = Middle;
}

void Printer::writeInline(std::string s)
{
	writer.put(s);
	state = Middle;
}

void Printer::write(char c)
{
	if (state == Middle)
		writer.put(' ');
	writeInline(c);
}

void Printer::write(std::string s)
{
	if (state == Middle)
		writer.put(' ');
	writeInline(s);
}

void Printer::startLine()
{
	if ((depth == 0) || (state != LineEnd))
		return;

	std::string indent(depth*2, ' ');
	writer.put(indent);
	state = LineStart;
}

void Printer::endLine()
{
	if (state != Middle)
		return;

	writer.put('\n');
	state = LineEnd;
}

void Printer::startInline(std::string name)
{
	write('(');
	if (!name.empty())
		writer.put(name);
}

void Printer::endInline()
{
	writeInline(')');
}

void Printer::start(std::string name)
{
	endLine();
	startLine();
	++depth;
	startInline(name);
}

void Printer::end()
{
	--depth;
	startLine();
	endInline();
	endLine();
}


void Printer::printSignature(tree::FunctionProto& node)
{
	write(node.getReturnType());
	write(node.getName());

	start();
	endLine();

	for (auto& arg : node.getArguments())
		visit(*arg);

	end();
}

void Printer::visit(tree::FunctionProto& node)
{
	start("func");
	printSignature(node);
	end();
}

void Printer::visit(tree::Function& node)
{
	start("func");
	printSignature(node.prototype());

	start();
	visit(node.getBody());
	end();
	end();
}

void Printer::visit(tree::Prototype& node)
{
	start("prototype");
	write(node.name());
	write('(');
	write(node.base());
	write(')');

	start();
	visit(node.body());
	end();
	end();
}

void Printer::visit(tree::Instance& node)
{
	start("instance");
	write(node.name());
	write('(');
	write(node.base());
	write(')');

	start();
	if (node.body())
		visit(*node.body());
	end();
	end();
}

void Printer::visit(tree::Class& node)
{
	start("class");
	for (auto& var : node.members())
		visit(*var);
	end();
}

void Printer::visit(tree::Variable& node)
{
	start(node.isConst() ? "const" : "var");
	write(node.name());
	if (node.sizeExpr()) {
		writer.put('[');
		visit(*node.sizeExpr());
		writer.put(']');
	}

	if (node.initializer())
		visit(*node.initializer());

	end();
}

void Printer::visit(tree::Declaration& node)
{
	switch(node.kind()) {
	case tree::Declaration::FunctionProto:
		return visit(static_cast<tree::FunctionProto&>(node));
	case tree::Declaration::Function:
		return visit(static_cast<tree::Function&>(node));
	case tree::Declaration::Variable:
		return visit(static_cast<tree::Variable&>(node));
	case tree::Declaration::Class:
		return visit(static_cast<tree::Class&>(node));
	case tree::Declaration::Prototype:
		return visit(static_cast<tree::Prototype&>(node));
	case tree::Declaration::Instance:
		return visit(static_cast<tree::Instance&>(node));
	}
}

void Printer::visit(tree::DeclStatement& node)
{
	visit(node.declaration());
}

void Printer::visit(tree::StatementBlock& node)
{
	start();
	endLine();
	for (auto& stmt : node.statements()) {
		startLine();
		visit(*stmt);
		endLine();
	}
	end();
}

void Printer::visit(tree::IfStatement& node)
{
	start("if");
	
	visit(node.condition());

	start("then");
	visit(node.thenBranch());
	end();

	if (node.elseBranch()) {
		start("else");
		visit(*node.elseBranch());
		end();
	}
	end();
}

void Printer::visit(tree::WhileStatement& node)
{
	start("while");
	visit(node.condition());

	start("");
	visit(node.body());
	end();
	end();
}
void Printer::visit(tree::DoStatement& node)
{
	start("do");
	start("");
	visit(node.body());
	end();
	write("while");
	visit(node.condition());
	end();
}
void Printer::visit(tree::BreakStatement& node)
{
	startInline("break");
	endInline();
}
void Printer::visit(tree::ContinueStatement& node)
{
	startInline("continue");
	endInline();
}

void Printer::visit(tree::ReturnStatement& node)
{
	startInline("return");
	
	if (node.expression())
		visit(*node.expression());

	endInline();
}

void Printer::visit(tree::Statement& node)
{
	switch(node.kind()) {
	case tree::Statement::DeclStatement:
		return visit(static_cast<tree::DeclStatement&>(node));
	case tree::Statement::StatementBlock:
		return visit(static_cast<tree::StatementBlock&>(node));
	case tree::Statement::IfStatement:
		return visit(static_cast<tree::IfStatement&>(node));
	case tree::Statement::ReturnStatement:
		return visit(static_cast<tree::ReturnStatement&>(node));
	case tree::Statement::WhileStatement:
		return visit(static_cast<tree::WhileStatement&>(node));
	case tree::Statement::DoStatement:
		return visit(static_cast<tree::DoStatement&>(node));
	case tree::Statement::BreakStatement:
		return visit(static_cast<tree::BreakStatement&>(node));
	case tree::Statement::ContinueStatement:
		return visit(static_cast<tree::ContinueStatement&>(node));
	case tree::Statement::ExprStatement:
		return visit(static_cast<tree::ExprStatement&>(node).expression());
	}
}

void Printer::visit(tree::NumberExpr& node)
{
	write(node.getValue());
}

void Printer::visit(tree::StringExpr& node)
{
	write("\"" + node.getValue() + "\"");
}

void Printer::visit(tree::IdentifierExpr& node)
{
	startInline(":");
	write(node.getName());
	endInline();
}

void Printer::visit(tree::ArrayInitializer& node)
{
	startInline("{}");
	for (auto& arg : node.initList())
		visit(*arg);
	endInline();
}

void Printer::visit(tree::CallExpr& node)
{
	startInline(node.function());
	
	startInline("");
	for (auto& arg : node.arguments())
		visit(*arg);
	endInline();
	endInline();
}

void Printer::visit(tree::FieldExpr& node)
{
	startInline(".");
	write(node.identifier());
	write(node.fieldName());
	endInline();
}

void Printer::visit(tree::SubscriptExpr& node)
{
	startInline("[]");
	visit(node.array());
	visit(node.subscript());
	endInline();
}

void Printer::visit(tree::UnaryExpr& node)
{
	std::string const tmp = spellToken(Token::Kind(node.getOperation()));
	startInline(tmp);
	visit(node.getOperand());
	endInline();
}

void Printer::visit(tree::BinaryExpr& node)
{
	std::string const tmp = spellToken(Token::Kind(node.getOperation()));
	startInline(tmp);
	visit(node.getLHS());
	visit(node.getRHS());
	endInline();
}

void Printer::visit(tree::Expression& node)
{
	switch(node.kind()) {
	case tree::Expression::NumberExpr:
		return visit(static_cast<tree::NumberExpr&>(node));
	case tree::Expression::StringExpr:
		return visit(static_cast<tree::StringExpr&>(node));
	case tree::Expression::IdentifierExpr:
		return visit(static_cast<tree::IdentifierExpr&>(node));
	case tree::Expression::ArrayInitializer:
		return visit(static_cast<tree::ArrayInitializer&>(node));
	case tree::Expression::CallExpr:
		return visit(static_cast<tree::CallExpr&>(node));
	case tree::Expression::FieldExpr:
		return visit(static_cast<tree::FieldExpr&>(node));
	case tree::Expression::SubscriptExpr:
		return visit(static_cast<tree::SubscriptExpr&>(node));
	case tree::Expression::UnaryExpr:
		return visit(static_cast<tree::UnaryExpr&>(node));
	case tree::Expression::BinaryExpr:
		return visit(static_cast<tree::BinaryExpr&>(node));
	}
}
} // namespace daedalus
