#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>

namespace mirv {
  namespace Builder {
    ptr<SymbolTable>::type
    SymbolTable::make(ModulePointer m) {
      ptr<SymbolTable>::type result(new SymbolTable(m, FunctionPointer()));
      return result;
    }

    /// Get the variable symbol at the current scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Symbol<Variable> >::type
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Symbol<Variable> *) const {
      if (function) {
        Symbol<Function>::VariableIterator i = function->variableFind(name);
        if (i != function->variableEnd()) {
          return *i;
        }
      }
      Symbol<Module>::VariableIterator i = module->variableFind(name);
      if (i != module->variableEnd()) {
        return *i;
      }
      return ptr<Symbol<Variable> >::type();
    } 
     
    /// Get the function symbol at the current scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Symbol<Function> >::type
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Symbol<Function> *) const {
      Symbol<Module>::FunctionIterator i = module->functionFind(name);
      if (i != module->functionEnd()) {
        return *i;
      }
      return ptr<Symbol<Function> >::type();
    }

    /// Get the type symbol at the current scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Symbol<Type<TypeBase> > >::const_type
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      const Symbol<Type<TypeBase> > *) const {
      Symbol<Module>::ConstTypeIterator i = module->typeFind(name);
      if (i != module->typeEnd()) {
        return *i;
      }
      return ptr<Symbol<Type<TypeBase> > >::const_type();
    }

    ptr<Symbol<Variable> >::type
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Symbol<Variable> *) const {
      ptr<Symbol<Variable> >::type var =
        lookupAtCurrentScope(name, reinterpret_cast<Symbol<Variable> *>(0));
      if (function && !var) {
        // Look up at module scope
        SymbolTable ModuleScope(module, ptr<Symbol<Function> >::type());
        var = ModuleScope.lookupAtCurrentScope(
          name,
          reinterpret_cast<Symbol<Variable> *>(0));
      }
      if (!var) {
        error("Could not find variable");
      }
      return var;
    } 
     
    ptr<Symbol<Function> >::type
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Symbol<Function> *) const {
      ptr<Symbol<Function> >::type function =
        lookupAtCurrentScope(name, reinterpret_cast<Symbol<Function> *>(0));
      if (!function) {
        error("Could not find function");
      }
      return function;
    }

    ptr<Symbol<Type<TypeBase> > >::const_type
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   const Symbol<Type<TypeBase> > *) const {
      ptr<Symbol<Type<TypeBase> > >::const_type type =
        lookupAtCurrentScope(name,
                             reinterpret_cast<const Symbol<Type<TypeBase> > *>(0));
      if (!type) {
        error("Could not find type");
      }
      return type;
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Symbol<Variable> >::type var) {
      ptr<Symbol<Variable> >::type result =
        lookupAtCurrentScope(var->name(),
                             reinterpret_cast<Symbol<Variable> *>(0));
      if (result) {
        error("Variable already exists");
      }
      if (function) {
        function->variablePushBack(var);
        return;
      }
      module->variablePushBack(var);
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Symbol<Function> >::type func) {
      ptr<Symbol<Function> >::type result =
        lookupAtCurrentScope(func->name(),
                             reinterpret_cast<Symbol<Function> *>(0));
      if (result) {
        error("Function already exists");
      }
      module->functionPushBack(func);
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Symbol<Type<TypeBase> > >::const_type type) {
      ptr<Symbol<Type<TypeBase> > >::const_type result =
        lookupAtCurrentScope(type->name(),
                             reinterpret_cast<const Symbol<Type<TypeBase> > *>(0));
      if (result) {
        error("Type already exists");
      }
      module->typePushBack(type);
    }
  }
}
