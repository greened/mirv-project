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
    SymbolTable::SymbolTable(ModulePointer m, FunctionPointer f)
        : module(m), function(f), tempNum(0) {}

    void SymbolTable::setModule(ModulePointer m) {
      module = m;
    }

    void SymbolTable::clearModule(void) {
      module.reset();
    }

    SymbolTable::ModulePointer SymbolTable::getModule(void) const {
      return module;
    }

    void SymbolTable::setFunction(FunctionPointer f) {
      function = f;
    }

    void SymbolTable::clearFunction(void) {
      function.reset();
    }

    SymbolTable::FunctionPointer SymbolTable::getFunction(void) const {
      return function;
    }

    ptr<SymbolTable>
    SymbolTable::make(ModulePointer m) {
      ptr<SymbolTable> result(new SymbolTable(m, FunctionPointer()));
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

    ptr<const Symbol<Type<TypeBase> > >
    SymbolTable::addPlaceholder(const std::string &name) 
    {
      // It's ok if this already exists.
      return placeholders.insert(
        std::make_pair(name, mirv::make<Symbol<Type<Placeholder> > >())).
        first->second;
    }

    ptr<const Symbol<Type<Placeholder> > >
    SymbolTable::lookupPlaceholder(const std::string &name) const
    {
      auto p = placeholders.find(name);
      if (p != placeholders.end()) {
        ptr<const Symbol<Type<Placeholder> > > result = p->second;
        return result;
      }
      return ptr<const Symbol<Type<Placeholder> > >();
    }

    ptr<const Symbol<Type<Placeholder> > >
    SymbolTable::removePlaceholder(const std::string &name)
    {
      auto p = placeholders.find(name);
      if (p != placeholders.end()) {
        ptr<const Symbol<Type<Placeholder> > > result = p->second;
        placeholders.erase(p);
        return result;
      }
      error("Missing placeholder!");
      return ptr<const Symbol<Type<Placeholder> > >();
    }

    void
    SymbolTable::resolve(const std::string &oldName,
                         ptr<const Symbol<Type<Placeholder> > > placeholder,
                         ptr<const Symbol<Type<TypeBase> > > replacement)
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
    ptr<Symbol<Variable> >
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Symbol<Variable> *) const {
      if (function) {
        Symbol<Function>::VariableIterator i = function->variableFind(name);
        if (i != function->variableEnd()) {
          return *i;
        }
      }
      return ptr<Symbol<Variable> >();
    } 

    /// Get the global variable symbol at the current scope only.
    /// Return a null pointer if the symbol does not exist.
    ptr<Symbol<GlobalVariable> >
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Symbol<GlobalVariable> *) const {
      Symbol<Module>::GlobalVariableIterator i = module->globalVariableFind(name);
      if (i != module->globalVariableEnd()) {
        return *i;
      }
      return ptr<Symbol<GlobalVariable> >();
    } 

    /// Get the globalvariable symbol at module scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Symbol<GlobalVariable> >
    SymbolTable::lookupAtModuleScope(const std::string &name,
                                     Symbol<GlobalVariable> *) const {
      Symbol<Module>::GlobalVariableIterator i =
        module->globalVariableFind(name);
      if (i != module->globalVariableEnd()) {
        return *i;
      }
      return ptr<Symbol<GlobalVariable> >();
    } 
     
    /// Get the function symbol at the module scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Symbol<Function> >
    SymbolTable::lookupAtModuleScope(const std::string &name,
                                     Symbol<Function> *) const {
      Symbol<Module>::FunctionIterator i = module->functionFind(name);
      if (i != module->functionEnd()) {
        return *i;
      }
      return ptr<Symbol<Function> >();
    }

    ptr<Symbol<Function> >
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Symbol<Function> *dummy) const {
      return lookupAtModuleScope(name, dummy);
    }

    /// Get the type symbol at the current scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<const Symbol<Type<TypeBase> > >
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

      return ptr<const Symbol<Type<TypeBase> > >();
    }

    ptr<Symbol<Variable> >
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Symbol<Variable> *) const {
      ptr<Symbol<Variable> > var =
        lookupAtCurrentScope(name, reinterpret_cast<Symbol<Variable> *>(0));
      if (!var) {
        error("Could not find variable");
      }
      return var;
    } 

    ptr<Symbol<GlobalVariable> >
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Symbol<GlobalVariable> *) const {
      // Look up at module scope
      SymbolTable ModuleScope(module, ptr<Symbol<Function> >());
      ptr<Symbol<GlobalVariable> > var = ModuleScope.lookupAtCurrentScope(
        name,
        reinterpret_cast<Symbol<GlobalVariable> *>(0));
      if (!var) {
        error("Could not find variable");
      }
      return var;
    }
     
    ptr<Symbol<Function> >
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Symbol<Function> *) const {
      ptr<Symbol<Function> > function =
        lookupAtModuleScope(name, reinterpret_cast<Symbol<Function> *>(0));
      if (!function) {
        error("Could not find function");
      }
      return function;
    }

    ptr<const Symbol<Type<TypeBase> > >
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   const Symbol<Type<TypeBase> > *) const {
      std::string realName = translateName(name);

      ptr<const Symbol<Type<TypeBase> > > type =
        lookupAtModuleScope(realName,
                            reinterpret_cast<const Symbol<Type<TypeBase> > *>(0));
      if (!type) {
        error("Could not find type");
      }
      return type;
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Symbol<Variable> > var) {
      ptr<Symbol<Variable> > result =
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
    SymbolTable::addAtCurrentScope(ptr<Symbol<GlobalVariable> > var) {
      ptr<Symbol<GlobalVariable> > result =
        lookupAtCurrentScope(var->name(),
                             reinterpret_cast<Symbol<GlobalVariable> *>(0));
      if (result) {
        error("Global variable already exists");
      }
      module->globalVariablePushBack(var);
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Symbol<Function> > func) {
      addAtModuleScope(func);
    }

    void
    SymbolTable::addAtModuleScope(ptr<Symbol<Function> > func) {
      ptr<Symbol<Function> > result =
        lookupAtModuleScope(func->name(),
                             reinterpret_cast<Symbol<Function> *>(0));
      if (result) {
        error("Function already exists");
      }
      module->functionPushBack(func);
    }

    void
    SymbolTable::addAtCurrentScope(ptr<const Symbol<Type<TypeBase> > > type) {
      addAtModuleScope(type);
    }

    void
    SymbolTable::addAtModuleScope(ptr<const Symbol<Type<TypeBase> > > type) {
      type->setParent(getModule());

      std::ostringstream name;
      print(name, type);
      ptr<const Symbol<Type<TypeBase> > > result =
        lookupAtModuleScope(name.str(),
                            reinterpret_cast<const Symbol<Type<TypeBase> > *>(0));
      if (result) {
        ptr<const Symbol<Type<Placeholder> > > placeholder =
          dyn_cast<const Symbol<Type<Placeholder> > >(result);
        if (!placeholder) {
          error("Type already exists");
        }
      }
      module->typePushBack(type);
    }

    void
    SymbolTable::addAtModuleScope(ptr<Symbol<GlobalVariable> > var)
    {
      ptr<Symbol<GlobalVariable> > result =
        lookupAtModuleScope(var->name(),
                            reinterpret_cast<Symbol<GlobalVariable> *>(0));
      if (result) {
        error("Variable already exists");
      }
      module->globalVariablePushBack(var);
    }
  }
}
