#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <utility>
#include <sstream>

namespace mirv {
  namespace Builder {
    ptr<SymbolTable>::type
    SymbolTable::make(ModulePointer m) {
      ptr<SymbolTable>::type result(new SymbolTable(m, FunctionPointer()));
      return result;
    }

    std::string
    SymbolTable::translateName(const std::string &name) const
    {
      auto n = names.find(name);
      if (n != names.end()) {
        return n->second;
      }
      return name;
    }

    ptr<Symbol<Type<TypeBase> > >::const_type
    SymbolTable::addPlaceholder(const std::string &name) 
    {
      // It's ok if this already exists.
      return placeholders.insert(
        std::make_pair(name, mirv::make<Symbol<Type<Placeholder> > >())).
        first->second;
    }

    ptr<Symbol<Type<Placeholder> > >::const_type
    SymbolTable::lookupPlaceholder(const std::string &name) const
    {
      auto p = placeholders.find(name);
      if (p != placeholders.end()) {
        ptr<Symbol<Type<Placeholder> > >::const_type result = p->second;
        return result;
      }
      return ptr<Symbol<Type<Placeholder> > >::const_type();
    }

    ptr<Symbol<Type<Placeholder> > >::const_type
    SymbolTable::removePlaceholder(const std::string &name)
    {
      auto p = placeholders.find(name);
      if (p != placeholders.end()) {
        ptr<Symbol<Type<Placeholder> > >::const_type result = p->second;
        placeholders.erase(p);
        return result;
      }
      error("Missing placeholder!");
      return ptr<Symbol<Type<Placeholder> > >::const_type();
    }

    void
    SymbolTable::resolve(const std::string &oldName,
                         ptr<Symbol<Type<Placeholder> > >::const_type placeholder,
                         ptr<Symbol<Type<TypeBase> > >::const_type replacement)
    {
      for (auto type = module->typeBegin();
           type != module->typeEnd();
           ++type) {
        boost::const_pointer_cast<Symbol<Type<TypeBase> > >((*type))->
          resolve(placeholder, replacement);
      }
      std::ostringstream name;
      print(name, replacement);
      names[oldName] = name.str();
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
      return ptr<Symbol<Variable> >::type();
    } 

    /// Get the global variable symbol at the current scope only.
    /// Return a null pointer if the symbol does not exist.
    ptr<Symbol<GlobalVariable> >::type
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Symbol<GlobalVariable> *) const {
      Symbol<Module>::GlobalVariableIterator i = module->globalVariableFind(name);
      if (i != module->globalVariableEnd()) {
        return *i;
      }
      return ptr<Symbol<GlobalVariable> >::type();
    } 

    /// Get the variable symbol at module scope only.  Return a null
    /// pointer if the symbol does not exist.
    ptr<Symbol<Variable> >::type
    SymbolTable::lookupAtModuleScope(const std::string &name,
                                     Symbol<Variable> *) const {
      Symbol<Module>::VariableIterator i = module->variableFind(name);
      if (i != module->variableEnd()) {
        return *i;
      }
      return ptr<Symbol<Variable> >::type();
    } 
     
    /// Get the function symbol at the module scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Symbol<Function> >::type
    SymbolTable::lookupAtModuleScope(const std::string &name,
                                     Symbol<Function> *) const {
      Symbol<Module>::FunctionIterator i = module->functionFind(name);
      if (i != module->functionEnd()) {
        return *i;
      }
      return ptr<Symbol<Function> >::type();
    }

    ptr<Symbol<Function> >::type
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Symbol<Function> *dummy) const {
      return lookupAtModuleScope(name, dummy);
    }

    /// Get the type symbol at the current scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Symbol<Type<TypeBase> > >::const_type
    SymbolTable::lookupAtModuleScope(const std::string &name,
                                     const Symbol<Type<TypeBase> > *) const {
      Symbol<Module>::ConstTypeIterator i = module->typeFind(name);
      if (i != module->typeEnd()) {
        return *i;
      }

      // See if there's a placeholder by this name.
      auto placeholder = lookupPlaceholder(name);
      if (placeholder) {
        return placeholder;
      }

      return ptr<Symbol<Type<TypeBase> > >::const_type();
    }

    ptr<Symbol<Variable> >::type
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Symbol<Variable> *) const {
      ptr<Symbol<Variable> >::type var =
        lookupAtCurrentScope(name, reinterpret_cast<Symbol<Variable> *>(0));
      if (!var) {
        error("Could not find variable");
      }
      return var;
    } 

    ptr<Symbol<GlobalVariable> >::type
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Symbol<GlobalVariable> *) const {
      // Look up at module scope
      SymbolTable ModuleScope(module, ptr<Symbol<Function> >::type());
      ptr<Symbol<GlobalVariable> >::type var = ModuleScope.lookupAtCurrentScope(
        name,
        reinterpret_cast<Symbol<GlobalVariable> *>(0));
      if (!var) {
        error("Could not find variable");
      }
      return var;
    }
     
    ptr<Symbol<Function> >::type
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Symbol<Function> *) const {
      ptr<Symbol<Function> >::type function =
        lookupAtModuleScope(name, reinterpret_cast<Symbol<Function> *>(0));
      if (!function) {
        error("Could not find function");
      }
      return function;
    }

    ptr<Symbol<Type<TypeBase> > >::const_type
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   const Symbol<Type<TypeBase> > *) const {
      std::string realName = translateName(name);

      ptr<Symbol<Type<TypeBase> > >::const_type type =
        lookupAtModuleScope(realName,
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
      error("Cannot add variable to module");
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Symbol<GlobalVariable> >::type var) {
      ptr<Symbol<GlobalVariable> >::type result =
        lookupAtCurrentScope(var->name(),
                             reinterpret_cast<Symbol<GlobalVariable> *>(0));
      if (result) {
        error("Global variable already exists");
      }
      module->globalVariablePushBack(var);
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Symbol<Function> >::type func) {
      addAtModuleScope(func);
    }

    void
    SymbolTable::addAtModuleScope(ptr<Symbol<Function> >::type func) {
      ptr<Symbol<Function> >::type result =
        lookupAtModuleScope(func->name(),
                             reinterpret_cast<Symbol<Function> *>(0));
      if (result) {
        error("Function already exists");
      }
      module->functionPushBack(func);
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Symbol<Type<TypeBase> > >::const_type type) {
      addAtModuleScope(type);
    }

    void
    SymbolTable::addAtModuleScope(ptr<Symbol<Type<TypeBase> > >::const_type type) {
      std::ostringstream name;
      print(name, type);
      ptr<Symbol<Type<TypeBase> > >::const_type result =
        lookupAtModuleScope(name.str(),
                            reinterpret_cast<const Symbol<Type<TypeBase> > *>(0));
      if (result) {
        ptr<Symbol<Type<Placeholder> > >::const_type placeholder =
          dyn_cast<const Symbol<Type<Placeholder> > >(result);
        if (!placeholder) {
          error("Type already exists");
        }
      }
      module->typePushBack(type);
    }

    void
    SymbolTable::addAtModuleScope(ptr<Symbol<Variable> >::type var)
    {
      ptr<Symbol<Variable> >::type result =
        lookupAtModuleScope(var->name(),
                            reinterpret_cast<Symbol<Variable> *>(0));
      if (result) {
        error("Variable already exists");
      }
      module->variablePushBack(var);
    }
  }
}
