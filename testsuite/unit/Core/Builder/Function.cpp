// Test building of functions.
//
// STDOUT: fdef testfunc {
// STDOUT:    {
// STDOUT:       store
// STDOUT:          cref int32 * & a
// STDOUT:          +
// STDOUT:             load
// STDOUT:                cref int32 * & a
// STDOUT:             load
// STDOUT:                cref int32 * & b
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Builder/FunctionGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::Variable;
using mirv::GlobalVariable;
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

using Builder::func;

int main(void)
{
  ptr<Symbol<Module> >::type module = make<Symbol<Module> >("testmodule");

  ptr<Symbol<Type<TypeBase> > >::type functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >::type());
  module->typePushBack(functype);

  ptr<Symbol<Type<TypeBase> > >::type inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  ptr<Symbol<GlobalVariable> >::type asym =
    make<Symbol<GlobalVariable> >("a", inttype);
  module->globalVariablePushBack(asym);

  ptr<Symbol<GlobalVariable> >::type bsym =
    make<Symbol<GlobalVariable> >("b", inttype);
  module->globalVariablePushBack(bsym);

  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};

  ptr<Node<Base> >::type code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(module,
      func["testfunc"].type["void ()"] [
       	a = a + b
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
