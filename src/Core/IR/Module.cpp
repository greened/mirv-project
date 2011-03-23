#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/Builder/Make.hpp>

namespace mirv {
  Module::Interface::Interface(const std::string &n) : Symbol<Named>(n)
  {
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
