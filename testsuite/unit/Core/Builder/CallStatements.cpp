// Test building of function calls combined with another statement.
// STDOUT: fdef testfunc {
// STDOUT:    {
// STDOUT:       assign
// STDOUT:          vref b
// STDOUT:          vref a
// STDOUT:       call
// STDOUT:          fref foo
// STDOUT:          vref b
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
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
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::do_;
using Builder::if_;

int main(void)
{
  ptr<Symbol<Module> >::type module = make<Symbol<Module> >("testmodule");

  
  ptr<Symbol<Type<TypeBase> > >::type inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  std::vector<ptr<Symbol<Type<TypeBase> > >::type> argTypes;
  argTypes.push_back(inttype);

  ptr<Symbol<Type<TypeBase> > >::type functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >::type(),
                                       argTypes.begin(),
                                       argTypes.end());
  module->typePushBack(functype);

  ptr<Symbol<Variable> >::type asym = make<Symbol<Variable> >("a", inttype);
  module->variablePushBack(asym);

  ptr<Symbol<Variable> >::type bsym = make<Symbol<Variable> >("b", inttype);
  module->variablePushBack(bsym);

  ptr<Symbol<Function> >::type fsym = make<Symbol<Function> >("foo", functype);
  module->functionPushBack(fsym);

  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  ptr<Node<Base> >::type code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(module,
      func["testfunc"].type[void_()] [
        b = a,
        foo(b)
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
