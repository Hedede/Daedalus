/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <daedalus/semantic/SymbolTable.h>
namespace daedalus {
SymbolTable::SymbolTable()
{
	// Create global scope
	scopes.emplace_back();
	scopeStack.emplace_back(0);

	insertClass({"void",   TypeID::Void});
	insertClass({"int",    TypeID::Integer});
	insertClass({"float",  TypeID::Float});
	insertClass({"string", TypeID::String});
	insertClass({"func",   TypeID::FuncRef});
}

SymbolRef SymbolTable::getSymbol(std::string name)
{
	auto iter = table.find(name);
	if (iter == std::end(table))
		return {};

	return iter->second;
}

Scope* SymbolTable::getScope(SymbolRef ref)
{
	if (!ref.isValid())
		return nullptr;

	assert(ref.scopeId < scopes.size());

	return &scopes[ref.scopeId];
}

Symbol* SymbolTable::getSymbol(SymbolRef ref)
{
	if (!ref.isValid())
		return nullptr;

	auto scope = getScope(ref);
	if (!scope)
		return nullptr;

	switch (ref.kind) {
	case Symbol::Variable:
		return &scope->variables[ref.index];
	case Symbol::Function:
		return &scope->functions[ref.index];
	case Symbol::Class:
		return &scope->classes[ref.index];
	case Symbol::Instance:
		return &scope->instances[ref.index];
	case Symbol::Prototype:
		return &scope->prototypes[ref.index];
	default:
		break;
	}

	return nullptr;
}

unsigned SymbolTable::pushScope()
{
	scopes.emplace_back();

	tableDeltas.emplace_back();
	scopeStack.emplace_back(scopes.size() - 1);

	return currentScope();
}

unsigned SymbolTable::currentScope()
{
	// scopeStack should NEVER by empty, if it's empty, I screwed up
	assert(!scopeStack.empty());
	assert(scopeStack.size() < std::numeric_limits<unsigned>::max());
	return scopeStack.back();
}

void SymbolTable::popScope()
{
	assert(currentScope() != 0);
	if (currentScope() == 0)
		return;

	for (auto& entry : tableDeltas.back())
		table[entry.first] = entry.second;

	tableDeltas.pop_back();
	scopeStack.pop_back();
}

void SymbolTable::saveSymbol(std::string name)
{
	auto iter = table.find(name);
	if (iter == std::end(table))
		tableDeltas.back().emplace_back(name, SymbolRef{});
	else
		tableDeltas.back().emplace_back(*iter);
}

void SymbolTable::insertSymbol(std::string name, SymbolRef ref)
{
	if (currentScope() != 0)
		saveSymbol(name);

	table[name] = ref;
}

SymbolRef SymbolTable::insertVariable(Variable&& var)
{
	auto sym = getSymbol(var.name);
	if (sym.isValid()) {
		if (sym.scopeId == currentScope())
			return {Symbol::Undefined};
	}

	auto& variables = scopes[currentScope()].variables;
	variables.emplace_back(std::move(var));

	auto& name = variables.back().name;
	auto index = variables.size() - 1;

	SymbolRef result{Symbol::Variable, currentScope(), index};
	insertSymbol(name, result);

	return result;
}

SymbolRef SymbolTable::insertClass(Class&& type)
{
	auto sym = getSymbol(type.name);
	if (sym.isValid())
		return {};

	auto& classes = scopes[currentScope()].classes;
	classes.emplace_back(std::move(type));

	auto& name = classes.back().name;
	auto index = classes.size() - 1;

	SymbolRef result{Symbol::Class, currentScope(), index};
	insertSymbol(name, result);

	return result;
}

SymbolRef SymbolTable::insertFunction(Function&& func)
{
	auto sym = getSymbol(func.name);
	if (sym.isValid())
		return {};

	auto& functions = scopes[currentScope()].functions;
	functions.emplace_back(std::move(func));

	auto& name = functions.back().name;
	auto index = functions.size() - 1;

	SymbolRef result{Symbol::Function, currentScope(), index};
	insertSymbol(name, result);

	return result;
}

SymbolRef SymbolTable::insertInstance(Instance&& func)
{
	auto sym = getSymbol(func.name);
	if (sym.isValid())
		return {};

	auto& instances = scopes[currentScope()].instances;
	instances.emplace_back(std::move(func));

	auto& name = instances.back().name;
	auto index = instances.size() - 1;

	SymbolRef result{Symbol::Instance, currentScope(), index};
	insertSymbol(name, result);

	return result;
}

SymbolRef SymbolTable::insertPrototype(Prototype&& func)
{
	auto sym = getSymbol(func.name);
	if (sym.isValid())
		return {};

	auto& prototypes = scopes[currentScope()].prototypes;
	prototypes.emplace_back(std::move(func));

	auto& name = prototypes.back().name;
	auto index = prototypes.size() - 1;

	SymbolRef result{Symbol::Prototype, currentScope(), index};
	insertSymbol(name, result);

	return result;
}
} // namespace daedalus
