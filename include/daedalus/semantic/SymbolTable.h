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
#include <aw/types/types.h>
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

	SymbolRef()
		: kind(Symbol::Undefined)
	{}

	SymbolRef(Symbol::Kind kind, size_t scope, size_t index)
		: kind(kind), scopeId(scope), index(index)
	{}

	bool isValid()
	{
		return kind != Symbol::Undefined;
	}

	aw::u64 kind    : 3;
	aw::u64 scopeId : 24;
	aw::u64 index   : 29;
};

using SymbolRefList = std::vector<SymbolRef>;

struct SymbolTable {
	SymbolTable();

	SymbolRef getSymbol(std::string name);
	Scope*    getScope(SymbolRef ref);
	Symbol*   getSymbol(SymbolRef ref);

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
	SymbolMap table;

	void saveSymbol(std::string name);
	void insertSymbol(std::string name, SymbolRef&& ref);

	using Entry = std::pair<std::string, SymbolRef>;
	using EntryList = std::vector<Entry>;
	std::vector<EntryList> tableDeltas;

	std::vector<Scope> scopes;
	std::vector<unsigned> scopeStack;
};
} // namespace daedalus
#endif//Daedalus_SymbolTable
