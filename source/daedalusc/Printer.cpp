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

	for (auto& arg : node.getArguments()) {
		arg->accept(*this);
	}

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
	node.getBody().accept(*this);
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
	node.body().accept(*this);
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
		node.body()->accept(*this);
	end();
	end();
}

void Printer::visit(tree::Class& node)
{
	start("class");
	for (auto& var : node.members())
		var->accept(*this);
	end();
}

void Printer::visit(tree::Variable& node)
{
	start(node.isConst() ? "const" : "var");
	write(node.name());
	if (node.sizeExpr()) {
		writer.put('[');
		node.sizeExpr()->accept(*this);
		writer.put(']');
	}
	if (node.initializer()) {
		node.initializer()->accept(*this);
	}
	end();
}

void Printer::visit(tree::DeclStatement& node)
{
	node.declaration().accept(*this);
}

void Printer::visit(tree::StatementBlock& node)
{
	start();
	endLine();
	for (auto& stmt : node.statements()) {
		startLine();
		stmt->accept(*this);
		endLine();
	}
	end();
}

void Printer::visit(tree::IfStatement& node)
{
	start("if");
	
	node.condition().accept(*this);

	start("then");
	node.thenBranch().accept(*this);
	end();

	if (node.elseBranch()) {
		start("else");
		node.elseBranch()->accept(*this);
		end();
	}
	end();
}

void Printer::visit(tree::WhileStatement& node)
{
	start("while");
	node.condition().accept(*this);

	start("");
	node.body().accept(*this);
	end();
	end();
}
void Printer::visit(tree::DoStatement& node)
{
	start("do");
	start("");
	node.body().accept(*this);
	end();
	write("while");
	node.condition().accept(*this);
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
		node.expression()->accept(*this);

	endInline();
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
	for (auto& arg : node.initList()) {
		arg->accept(*this);
	}
	endInline();
}

void Printer::visit(tree::CallExpr& node)
{
	startInline(node.getFunction());
	
	startInline("");
	for (auto& arg : node.getArguments()) {
		arg->accept(*this);
	}
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
	node.array().accept(*this);
	node.subscript().accept(*this);
	endInline();
}

void Printer::visit(tree::UnaryExpr& node)
{
	std::string const tmp = spellToken(Token::Kind(node.getOperation()));
	startInline(tmp);
	node.getOperand().accept(*this);
	endInline();
}

void Printer::visit(tree::BinaryExpr& node)
{
	std::string const tmp = spellToken(Token::Kind(node.getOperation()));
	startInline(tmp);
	node.getLHS().accept(*this);
	node.getRHS().accept(*this);
	endInline();
}
} // namespace daedalus
