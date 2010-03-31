// Test building of variables.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a int32
// STDOUT:    {
// STDOUT:
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::Variable;
using mirv::Type;
using mirv::TypeBase;
using mirv::Integral;
using mirv::FunctionType;
using mirv::Node;
using mirv::Base;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::make;

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::function;

int main(void)
{
  ptr<Symbol<Module> >::type module = make<Symbol<Module> >("testmodule");

  ptr<Symbol<Type<TypeBase> > >::type functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >::type());
  module->typePushBack(functype);

  ptr<Symbol<Type<TypeBase> > >::type inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  ptr<Node<Base> >::type code =
    Builder::translate(module, ptr<Symbol<Function> >::type(),
      function["testfunc"].type["void ()"] [
	var["a"].type["int32"]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
