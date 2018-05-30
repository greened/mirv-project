#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <utility>
#include <sstream>

namespace mirv {
  namespace Builder {
    SymbolTable::SymbolTable(ModulePointer m, FunctionPointer f)
        : module(m), function(f) {}

    void SymbolTable::setModule(ModulePointer m) {
      module = m;
    }

    // void SymbolTable::clearModule(void) {
    //   module.reset();
    // }

    SymbolTable::ModulePointer SymbolTable::getModule(void) const {
      return module;
    }

    void SymbolTable::setFunction(FunctionPointer f) {
      function = f;
    }

    // void SymbolTable::clearFunction(void) {
    //   function.reset();
    // }

    SymbolTable::FunctionPointer SymbolTable::getFunction(void) const {
      return function;
    }

    ptr<SymbolTable>
    SymbolTable::make(ModulePointer m) {
      ptr<SymbolTable> result(new SymbolTable(m, FunctionPointer()));
      return result;
    }

    std::string
    SymbolTable::translatePlaceholderName(const std::string &name) const
    {
      auto n = PlaceholderNames.find(name);
      if (n != PlaceholderNames.end()) {
        return n->second;
      }
      return name;
    }
#if 0
    ptr<const Type>
    SymbolTable::addPlaceholder(const std::string &name) 
    {
      // It's ok if this already exists.
      return placeholders.insert(std::make_pair(name,
                                                IRBuilder::getPlaceholderType(name))).first->second;
    }

    ptr<const PlaceholderType>
    SymbolTable::lookupPlaceholder(const std::string &name) const
    {
      auto p = placeholders.find(name);
      if (p != placeholders.end()) {
        ptr<const PlaceholderType> result = p->second;
        return result;
      }
      return ptr<const PlaceholderType>();
    }

    ptr<const PlaceholderType>
    SymbolTable::removePlaceholder(const std::string &name)
    {
      auto p = placeholders.find(name);
      if (p != placeholders.end()) {
        ptr<const PlaceholderType> result = p->second;
        placeholders.erase(p);
        return result;
      }
      error("Missing placeholder!");
      return ptr<const PlaceholderType>();
    }

    void
    SymbolTable::resolve(const std::string &oldName,
                         ptr<const PlaceholderType> placeholder,
                         ptr<const Type> replacement)
    {
      for (auto type : module->Types()) {
        std::const_pointer_cast<Type>(type)->resolve(placeholder, replacement);
      }
      std::ostringstream name;
      print(name, replacement);
      PlaceholderNames[oldName] = name.str();
    }

    /// Get the variable symbol at the current scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Allocate>
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Allocate *) const {
      assert(0 && "Not implemented");
      return ptr<Allocate>();
    }

    /// Get the global variable symbol at the current scope only.
    /// Return a null pointer if the symbol does not exist.
    ptr<GlobalVariable>
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      GlobalVariable *) const {
      return module->globalVariableFind(name);
    }

    /// Get the globalvariable symbol at module scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<GlobalVariable>
    SymbolTable::lookupAtModuleScope(const std::string &name,
                                     GlobalVariable *) const {
      return module->globalVariableFind(name);
    }

    /// Get the function symbol at the module scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<Function>
    SymbolTable::lookupAtModuleScope(const std::string &name,
                                     Function *) const {
      return module->functionFind(name);
    }

    ptr<Function>
    SymbolTable::lookupAtCurrentScope(const std::string &name,
                                      Function *dummy) const {
      return lookupAtModuleScope(name, dummy);
    }

    /// Get the type symbol at the current scope only.  Return a
    /// null pointer if the symbol does not exist.
    ptr<const Type>
    SymbolTable::lookupAtModuleScope(const std::string &name,
                                     const Type *) const {
      auto i = IRBuilder::findTupleType(name);
      if (i) {
        return i;
      }

      // See if there's a placeholder by this name.
      auto placeholder = lookupPlaceholder(name);
      if (placeholder) {
        return placeholder;
      }

      return ptr<const Type>();
    }

    ptr<Allocate>
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Allocate *) const {
      ptr<Allocate> var =
        lookupAtCurrentScope(name, reinterpret_cast<Allocate *>(0));
      if (!var) {
        error("Could not find variable");
      }
      return var;
    }

    ptr<GlobalVariable>
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   GlobalVariable *) const {
      // Look up at module scope
      SymbolTable ModuleScope(module, ptr<Function>());
      ptr<GlobalVariable> var = ModuleScope.lookupAtCurrentScope(
        name,
        reinterpret_cast<GlobalVariable *>(0));
      if (!var) {
        error("Could not find variable");
      }
      return var;
    }

    ptr<Function>
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   Function *) const {
      ptr<Function> function =
        lookupAtModuleScope(name, reinterpret_cast<Function *>(0));
      if (!function) {
        error("Could not find function");
      }
      return function;
    }

    ptr<const Type>
    SymbolTable::lookupAtAllScopes(const std::string &name,
                                   const Type *) const {
      std::string realName = translatePlaceholderName(name);

      ptr<const Type> type =
        lookupAtModuleScope(realName,
                            reinterpret_cast<const Type *>(0));
      if (!type) {
        error("Could not find type");
      }
      return type;
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Allocate> var) {
      ptr<Allocate> result =
        lookupAtCurrentScope(var->getName(),
                             reinterpret_cast<Allocate *>(0));
      if (result) {
        error("Variable already exists");
      }
      assert(0 && "Unimplemented");
      error("Cannot add variable to module");
    }

    void
    SymbolTable::addAtCurrentScope(ptr<GlobalVariable> var) {
      ptr<GlobalVariable> result =
        lookupAtCurrentScope(var->getName(),
                             reinterpret_cast<GlobalVariable *>(0));
      if (result) {
        error("Global variable already exists");
      }
      module->AddGlobalVariable(var);
    }

    void
    SymbolTable::addAtCurrentScope(ptr<Function> func) {
      addAtModuleScope(func);
    }

    void
    SymbolTable::addAtModuleScope(ptr<Function> func) {
      ptr<Function> result =
        lookupAtModuleScope(func->getName(),
                             reinterpret_cast<Function *>(0));
      if (result) {
        error("Function already exists");
      }
      module->AddFunction(func);
    }

    // void
    // SymbolTable::addAtCurrentScope(ptr<const Type> type) {
    //   addAtModuleScope(type);
    // }

    // void
    // SymbolTable::addAtModuleScope(ptr<const Type> type) {
    //   type->setParent(getModule());

    //   std::ostringstream name;
    //   print(name, type);
    //   ptr<const Type> result =
    //     lookupAtModuleScope(name.str(),
    //                         reinterpret_cast<const Type *>(0));
    //   if (result) {
    //     ptr<const PlaceholderType> placeholder =
    //       dyn_cast<const PlaceholderType>(result);
    //     if (!placeholder) {
    //       error("Type already exists");
    //     }
    //   }
    //   module->typePushBack(type);
    // }

    void
    SymbolTable::addAtModuleScope(ptr<GlobalVariable> var) {
      ptr<GlobalVariable> result =
        lookupAtModuleScope(var->getName(),
                            reinterpret_cast<GlobalVariable *>(0));
      if (result) {
        error("Variable already exists");
      }
      module->AddGlobalVariable(var);
    }
#endif
  }
}
