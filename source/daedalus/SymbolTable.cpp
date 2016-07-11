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
	mapStack.emplace_back();
	scopeStack.emplace_back(0);
}

SymbolRef SymbolTable::getSymbol(std::string name)
{
	auto iter = top().find(name);
	if (iter == std::end(top()))
		return {};

	return iter->second;
}

Scope* SymbolTable::getScope(unsigned ref)
{
	if (ref == SymbolRef::undefined)
		return nullptr;

	assert(ref < scopes.size());

	return &scopes[ref];
}

unsigned SymbolTable::pushScope()
{
	scopes.emplace_back();

	mapStack.emplace_back(mapStack.back());
	scopeStack.emplace_back(scopes.size() - 1);

	return currentScope();
}

unsigned SymbolTable::currentScope()
{
	// scopeStack should NEVER by empty, if it's empty, I screwed up
	assert(!scopeStack.empty());
	assert(scopeStack.size() < std::numeric_limits<unsigned>::max());
	return scopeStack.size() - 1;
}

void SymbolTable::popScope()
{
	assert(currentScope() != 0);
	if (currentScope() == 0)
		return;

	mapStack.pop_back();
	scopeStack.pop_back();
}

auto SymbolTable::insertVariable(Variable&& var) -> InsertResult
{
	auto sym = getSymbol(var.name);
	if (sym.isValid()) {
		if (sym.scopeId == currentScope())
			return AlreadyDefined;
	}

	auto& variables = scopes[currentScope()].variables;
	variables.emplace_back(std::move(var));

	top()[var.name] = {currentScope(), variables.size() - 1};
	return Success;
}

auto SymbolTable::insertClass(Class&& type) -> InsertResult
{
	if (currentScope() != 0)
		return NotAllowedHere;

	auto sym = getSymbol(type.name);
	if (sym.isValid())
		return AlreadyDefined;

	auto& classes = scopes[currentScope()].classes;
	classes.emplace_back(std::move(type));

	top()[type.name] = {currentScope(), classes.size() - 1};
	return Success;
}

auto SymbolTable::insertFunction(Function&& func) -> InsertResult
{
	if (currentScope() != 0)
		return NotAllowedHere;

	auto sym = getSymbol(func.name);
	if (sym.isValid())
		return AlreadyDefined;

	auto& functions = scopes[currentScope()].functions;
	functions.emplace_back(std::move(func));

	top()[func.name] = {currentScope(), functions.size() - 1};
	return Success;
}

auto SymbolTable::insertInstance(Instance&& func) -> InsertResult
{
	if (currentScope() != 0)
		return NotAllowedHere;

	auto sym = getSymbol(func.name);
	if (sym.isValid())
		return AlreadyDefined;

	auto& instances = scopes[currentScope()].instances;
	instances.emplace_back(std::move(func));

	top()[func.name] = {currentScope(), instances.size() - 1};
	return Success;
}

auto SymbolTable::insertPrototype(Prototype&& func) -> InsertResult
{
	if (currentScope() != 0)
		return NotAllowedHere;

	auto sym = getSymbol(func.name);
	if (sym.isValid())
		return AlreadyDefined;

	auto& prototypes = scopes[currentScope()].prototypes;
	prototypes.emplace_back(std::move(func));

	top()[func.name] = {currentScope(), prototypes.size() - 1};
	return Success;
}
} // namespace daedalus
