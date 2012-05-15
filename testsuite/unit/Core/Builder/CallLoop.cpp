// Test building of function calls in statement context.
//
// STDOUT: doWhile
// STDOUT:    {
// STDOUT:       call
// STDOUT:          fref foo
// STDOUT:    }
// STDOUT:    <
// STDOUT:       load
// STDOUT:          cref int32 * & a
// STDOUT:       load
// STDOUT:          cref int32 * & b

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
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

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::do_;

int main(void)
{
  ptr<Symbol<Module> > module = make<Symbol<Module> >("testmodule");

  ptr<Symbol<Type<TypeBase> > > functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >());
  module->typePushBack(functype);

  ptr<Symbol<Type<TypeBase> > > inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  ptr<Symbol<Function> > function =
    make<Symbol<Function> >("testfunc", functype);
  module->functionPushBack(function);

  function = make<Symbol<Function> >("foo", functype);
  module->functionPushBack(function);

  ptr<Symbol<GlobalVariable> > asym =
    make<Symbol<GlobalVariable> >("a", inttype);
  module->globalVariablePushBack(asym);

  ptr<Symbol<GlobalVariable> > bsym =
    make<Symbol<GlobalVariable> >("b", inttype);
  module->globalVariablePushBack(bsym);

  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  ptr<Node<Base> > code =
    Builder::translateWithGrammar<Builder::ConstructStatementGrammar>(
      module,
      function,
      do_[
        foo()
      ].while_(a < b)
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
