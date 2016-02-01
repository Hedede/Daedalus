/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/syntax/decl/Variable.h>
#include <daedalus/syntax/decl/Function.h>
#include <daedalus/syntax/decl/Class.h>
#include <daedalus/syntax/stmt/LocalDecl.h>

#include <daedalus/syntax/expr/UnaryExpr.h>
#include <daedalus/syntax/expr/BinaryExpr.h>
#include <daedalus/syntax/expr/NumberExpr.h>
#include <daedalus/syntax/expr/StringExpr.h>
#include <daedalus/syntax/expr/IdentifierExpr.h>
#include <daedalus/syntax/expr/InitializerExpr.h>
#include <daedalus/syntax/expr/CallExpr.h>
#include <daedalus/syntax/expr/FieldExpr.h>
#include <daedalus/syntax/expr/SubscriptExpr.h>

#include <daedalus/syntax/stmt/IfElseStatement.h>
#include <daedalus/syntax/stmt/ReturnStatement.h>
#include <daedalus/syntax/stmt/StatementBlock.h>

#include <daedalus/utility/Printer.h>
#include <daedalus/utility/PrintToken.h>

namespace daedalus {
Printer::Printer(io::WriteStream& out)
	: writer(out)
{
}

void Printer::startLine()
{
	if (depth == 0)
		return;

	std::string indent(depth*2, ' ');
	writer.put(indent);
}

void Printer::endLine()
{
	writer.put('\n');
}

void Printer::startInline(std::string name)
{
	writer.put('(');
	writer.put(name);
	writer.put(' ');
}

void Printer::endInline()
{
	writer.put(')');
	writer.put(' ');
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
	writer.put(')');
	endLine();
}


void Printer::printSignature(tree::FunctionProto& node)
{
	writer.put(node.getReturnType());
	writer.put(' ');
	writer.put(node.getName());

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
	writer.put(node.name());
	writer.put(' ');
	writer.put('(');
	writer.put(node.base());
	writer.put(')');

	start();
	node.body().accept(*this);
	end();
	end();
}

void Printer::visit(tree::Instance& node)
{
	start("instance");
	writer.put(node.name());
	writer.put(' ');
	writer.put('(');
	writer.put(node.base());
	writer.put(')');

	start();
	node.body().accept(*this);
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
	writer.put(node.getName());
	if (node.sizeExpr()) {
		writer.put(' ');
		writer.put('[');
		node.sizeExpr()->accept(*this);
		writer.put(']');
	}
	if (node.initializer()) {
		writer.put(' ');
		node.initializer()->accept(*this);
	}
	end();
}

void Printer::visit(tree::LocalDecl& node)
{
	node.declaration().accept(*this);
}

void Printer::visit(tree::StatementBlock& node)
{
	start();
	endLine();
	for (auto& stmt : node.getStatements()) {
		startLine();
		stmt->accept(*this);
		endLine();
	}
	end();
}

void Printer::visit(tree::IfElseStatement& node)
{
	start("if");
	
	node.getCondition().accept(*this);

	start("then");
	node.getThenBranch().accept(*this);
	end();

	if (node.getElseBranch()) {
		start("else");
		node.getElseBranch()->accept(*this);
		end();
	}
	end();
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
	writer.put(node.getValue());
	writer.put(' ');
}
void Printer::visit(tree::StringExpr& node)
{
	writer.put('"');
	writer.put(node.getValue());
	writer.put(' ');
	writer.put('"');
}
void Printer::visit(tree::IdentifierExpr& node)
{
	startInline(":");
	writer.put(node.getName());
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
	writer.put(node.identifier());
	writer.put(' ');
	writer.put(node.fieldName());
	endInline();
}

void Printer::visit(tree::SubscriptExpr& node)
{
	startInline("[]");
	node.array().accept(*this);
	writer.put(' ');
	node.subscript().accept(*this);
	endInline();
}

void Printer::visit(tree::UnaryExpr& node)
{
	std::string const tmp = spellToken(TokenType(node.getOperation()));
	startInline(tmp);
	node.getOperand().accept(*this);
	endInline();
}

void Printer::visit(tree::BinaryExpr& node)
{
	std::string const tmp = spellToken(TokenType(node.getOperation()));
	startInline(tmp);
	node.getLHS().accept(*this);
	node.getRHS().accept(*this);
	endInline();
}
} // namespace daedalus
