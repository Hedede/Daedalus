/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_SymbolTable
#define Daedalus_SymbolTable
#include <limits>
#include <unordered_map>
#include <daedalus/semantic/Symbol.h>
namespace daedalus {
struct Scope {
	std::vector<Variable> variables;

	// daedalus doesn't support non-top-level class/func,
	// but interpreter doesn't care about that
	std::vector<Function> functions;
	std::vector<Class>    classes;

	std::vector<Instance>  instances;
	std::vector<Prototype> prototypes;
};

struct SymbolRef {
	static constexpr unsigned undefined = std::numeric_limits<unsigned>::max();

	SymbolRef() = default;
	SymbolRef(size_t scope, size_t index)
		: scopeId(unsigned(scope)), index(unsigned(index))
	{}

	bool isValid()
	{
		return scopeId != undefined && index != undefined;
	}

	unsigned scopeId = undefined;
	unsigned index   = undefined;
};

using SymbolRefList = std::vector<SymbolRef>;

struct SymbolTable {
	SymbolTable();

	SymbolRef getSymbol(std::string name);
	Scope*    getScope(unsigned ref);

	unsigned pushScope();
	unsigned currentScope();
	void popScope();

	enum InsertResult {
		Success,
		AlreadyDefined,
		NotAllowedHere,
	};

	InsertResult insertVariable(Variable&& var);
	InsertResult insertClass(Class&& type);
	InsertResult insertFunction(Function&& func);
	InsertResult insertInstance(Instance&& func);
	InsertResult insertPrototype(Prototype&& func);

private:
	using SymbolMap = std::unordered_map<std::string, SymbolRef>;

	// TODO: custom open-addressed hash-table
	std::vector<SymbolMap> mapStack;
	std::vector<size_t> scopeStack;
	// TODO: keep deltas instead of copying whole ht
	// using Entry = std::pair<std::string, SymbolRef>;
	// using EntryList = std::vector<Entry>;
	// std::vector<EntryList> tableDeltas;

	SymbolMap& top()
	{
		return mapStack.back();
	}

	std::vector<Scope> scopes;

};
} // namespace daedalus
#endif//Daedalus_SymbolTable