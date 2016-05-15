/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Expression
#define Daedalus_Expression
#include <aw/types/types.h>
#include <daedalus/syntax/Statement.h>
namespace daedalus {
using namespace aw;
namespace tree {
class Expression : public Statement {
public:
	virtual ~Expression() = default;

	virtual void accept(tree::Visitor& visitor) = 0;

protected:
	Expression()
		: Statement(Statement::Expr)
	{ }

private:
	// Kind const kind_;
};

/*!
 * Reference to a identifier
 */
class IdentifierExpr : public Expression {
public:
	IdentifierExpr(std::string id)
		: id(id)
	{
	}

	virtual ~IdentifierExpr() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::string getName()
	{
		return id;
	}
private:
	std::string id;
};

/*!
 * Reference to a class field
 */
class FieldExpr : public Expression {
public:
	FieldExpr(std::string inst,
	          std::string field)
		: inst(inst), field(field)
	{
	}

	virtual ~FieldExpr() = default;

	virtual std::string identifier() const
	{
		return inst;
	}

	virtual std::string fieldName()
	{
		return field;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	std::string inst;
	std::string field;
};

class CallExpr : public Expression {
public:
	CallExpr(std::string callee,
	         std::vector<std::unique_ptr<Expression>> arguments)
		: callee(callee), arguments(std::move(arguments))
	{
	}

	virtual ~CallExpr() = default;

	virtual std::string getFunction() const
	{
		return callee;
	}

	virtual std::vector<uptr<Expression>>& getArguments()
	{
		return arguments;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	//Function* callee;
	std::string callee;
	std::vector<std::unique_ptr<Expression>> arguments;
};

/*!
 * unary = op primary
 */
class UnaryExpr : public Expression {
public:
	UnaryExpr(int op, std::unique_ptr<Expression> operand)
		: op(op), operand(std::move(operand))
	{
	}

	virtual ~UnaryExpr() = default;

	virtual Expression& getOperand()
	{
		return *operand.get();
	}

	virtual int getOperation()
	{
		return op;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	std::unique_ptr<Expression> operand;
	int op;
};

/*!
 * binary = unary op unary
 */
class BinaryExpr : public Expression {
public:
	BinaryExpr(int op,
	           std::unique_ptr<Expression> lhs,
	           std::unique_ptr<Expression> rhs)
		: op(op), lhs(std::move(lhs)), rhs(std::move(rhs))
	{
	}

	virtual ~BinaryExpr() = default;

	virtual Expression& getLHS()
	{
		return *lhs.get();
	}
	virtual Expression& getRHS()
	{
		return *rhs.get();
	}
	virtual int getOperation()
	{
		return op;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	std::unique_ptr<Expression> lhs;
	std::unique_ptr<Expression> rhs;
	int op;
};

/*!
 * Numeric literal (int or float)
 */
class NumberExpr : public Expression {
public:
	NumberExpr(std::string value)
		: value(value)
	{
	}

	virtual ~NumberExpr() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::string getValue() const
	{
		return value;
	}
private:
	std::string value;
};

/*!
 * String literal (double-quoted "")
 */
class StringExpr : public Expression {
public:
	StringExpr(std::string value)
		: value(value)
	{
	}

	virtual ~StringExpr() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::string getValue() const
	{
		return value;
	}
private:
	std::string value;
};

class SubscriptExpr : public Expression {
public:
	SubscriptExpr(uptr<Expression> array,
	              uptr<Expression> expr)
		: arrayExpr(std::move(array)),
		  subscriptExpr(std::move(expr))
	{
	}

	virtual ~SubscriptExpr() = default;

	virtual Expression& array() const
	{
		return *arrayExpr;
	}

	virtual Expression& subscript()
	{
		return *subscriptExpr;
	}

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	uptr<Expression> arrayExpr;
	uptr<Expression> subscriptExpr;
};

// TODO: Probably initializer should be a class on its own
class ArrayInitializer : public Expression {
public:
	typedef std::vector<uptr<Expression>> InitList;

	ArrayInitializer(InitList initList)
		: exprs(std::move(initList))
	{
	}

	virtual ~ArrayInitializer() = default;

	virtual void accept(tree::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	InitList& initList()
	{
		return exprs;
	}

	size_t size()
	{
		return exprs.size();
	}
private:
	InitList exprs;
};
} // namespace tree
} // namespace daedalus
#endif//Daedalus_Expression
