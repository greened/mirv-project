// Test printing of satements.
//
// STDOUT: ifThen
// STDOUT:    <
// STDOUT:       load
// STDOUT:          vref a
// STDOUT:       load
// STDOUT:          vref c
// STDOUT:    {
// STDOUT:       doWhile
// STDOUT:          {
// STDOUT:             store
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   load
// STDOUT:                      vref a
// STDOUT:                   load
// STDOUT:                      cref int32 * & b
// STDOUT:             ifElse
// STDOUT:                >
// STDOUT:                   load
// STDOUT:                      cref int32 * & b
// STDOUT:                   load
// STDOUT:                      vref c
// STDOUT:                {
// STDOUT:                   store
// STDOUT:                      vref a
// STDOUT:                      +
// STDOUT:                         load
// STDOUT:                            vref a
// STDOUT:                         load
// STDOUT:                            cref int32 * & b
// STDOUT:                }
// STDOUT:                {
// STDOUT:                   store
// STDOUT:                      vref a
// STDOUT:                      +
// STDOUT:                         load
// STDOUT:                            vref a
// STDOUT:                         load
// STDOUT:                            vref c
// STDOUT:                }
// STDOUT:          }
// STDOUT:          <
// STDOUT:             load
// STDOUT:                vref a
// STDOUT:             load
// STDOUT:                vref c
// STDOUT:    }

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
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/IfGrammar.hpp>
#include <mirv/Core/Builder/WhileGrammar.hpp>
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
using mirv::Pointer;
using mirv::FunctionType;
using mirv::Node;
using mirv::Base;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::make;

namespace Builder = mirv::Builder;

using Builder::if_;
using Builder::do_;
using Builder::while_;

int main(void)
{
  ptr<Symbol<Module> > module = make<Symbol<Module> >("testmodule");

  ptr<Symbol<Type<TypeBase> > > functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >());
  module->typePushBack(functype);

  ptr<Symbol<Type<TypeBase> > > inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  ptr<Symbol<Type<TypeBase> > > ptrtype =
    make<Symbol<Type<Pointer> > >(inttype);
  module->typePushBack(ptrtype);

  ptr<Symbol<Function> > function =
    make<Symbol<Function> >("testfunc", functype);

  module->functionPushBack(function);

  ptr<Symbol<Variable> > asym = make<Symbol<Variable> >("a", ptrtype);
  function->variablePushBack(asym);

  ptr<Symbol<GlobalVariable> > bsym =
    make<Symbol<GlobalVariable> >("b", inttype);
  module->globalVariablePushBack(bsym);

  ptr<Symbol<Variable> > csym = make<Symbol<Variable> >("c", ptrtype);
  function->variablePushBack(csym);

  Builder::VariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};

  ptr<Node<Base> > stmt =
    Builder::translateWithGrammar<Builder::ConstructStatementGrammar>(
      module,
      function,
      while_(a < c) [
	a = a + b,
        if_(b > c) [
          a = a + b
        ].else_[
          a = a + c
        ]
      ]
    );

  PrintFilter print(std::cout);

  print(stmt);

  return(0);
}
