#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/Builder/Make.hpp>

#include <functional>

namespace mirv {
  detail::ModuleInterface::ModuleInterface(const std::string &n) : Symbol<Named>(n)
  {
  }

  void
  detail::ModuleInterface::functionPushBack(FunctionPointer f)
  {
    FunctionBaseType::push_back(f);
    f->setParent(this->getSharedHandle());
  }

  detail::ModuleInterface::FunctionIterator
  detail::ModuleInterface::functionFind(const std::string &name) 
  {
    return std::find_if(functionBegin(), functionEnd(),
                        std::bind(SymbolByName<Function>(),
                                  std::placeholders::_1, name));
  }

  void
  detail::ModuleInterface::globalVariablePushBack(GlobalVariablePointer v)
  {
    GlobalVariableBaseType::push_back(v);
    v->setParent(this->getSharedHandle());
  }

  detail::ModuleInterface::GlobalVariableIterator
  detail::ModuleInterface::globalVariableFind(const std::string &name) 
  {
    return std::find_if(globalVariableBegin(), globalVariableEnd(),
                        std::bind(SymbolByName<GlobalVariable>(),
                                  std::placeholders::_1, name));
  }

  void
  detail::ModuleInterface::typePushBack(TypePointer t)
  {
    TypeBaseType::push_back(t);
    t->setParent(this->getSharedHandle());
  }

  detail::ModuleInterface::TypeIterator
  detail::ModuleInterface::typeFind(const std::string &name) 
  {
    return std::find_if(typeBegin(), typeEnd(),
                        std::bind(SymbolByName<Type<TypeBase> >(),
                                  std::placeholders::_1, name));
  }

  void Module::initialize(ptr<Symbol<Module> >::type module)
  {
    // Every module must have at least a 64-bit integer type.  This
    // allows bootstraping of the IR since we need this type to create
    // integer constants.
    ptr<Symbol<Type<TypeBase> > >::const_type type =
      make<Symbol<Type<Integral> > >(64);
    type->setParent(module);
    module->typePushBack(type);
  }
}
