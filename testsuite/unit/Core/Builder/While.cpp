// Test printing of satements.
//
// STDOUT: ifThen
// STDOUT:    <
// STDOUT:       vref a
// STDOUT:       vref c
// STDOUT:    {
// STDOUT:       doWhile
// STDOUT:          {
// STDOUT:             assign
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   vref a
// STDOUT:                   vref b
// STDOUT:             ifElse
// STDOUT:                >
// STDOUT:                   vref b
// STDOUT:                   vref c
// STDOUT:                {
// STDOUT:                   assign
// STDOUT:                      vref a
// STDOUT:                      +
// STDOUT:                         vref a
// STDOUT:                         vref b
// STDOUT:                }
// STDOUT:                {
// STDOUT:                   assign
// STDOUT:                      vref a
// STDOUT:                      +
// STDOUT:                         vref a
// STDOUT:                         vref c
// STDOUT:                }
// STDOUT:          }
// STDOUT:          <
// STDOUT:             vref a
// STDOUT:             vref c
// STDOUT:    }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
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

using Builder::if_;
using Builder::do_;
using Builder::while_;

int main(void)
{
  ptr<Symbol<Module> >::type module = make<Symbol<Module> >("testmodule");

  ptr<Symbol<Type<TypeBase> > >::type functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >::type());
  module->typePushBack(functype);

  ptr<Symbol<Type<TypeBase> > >::type inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  ptr<Symbol<Function> >::type function =
    make<Symbol<Function> >("testfunc", functype);

  module->functionPushBack(function);

  ptr<Symbol<Variable> >::type asym = make<Symbol<Variable> >("a", inttype);
  function->variablePushBack(asym);

  ptr<Symbol<Variable> >::type bsym = make<Symbol<Variable> >("b", inttype);
  module->variablePushBack(bsym);

  ptr<Symbol<Variable> >::type csym = make<Symbol<Variable> >("c", inttype);
  function->variablePushBack(csym);

  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};

  ptr<Node<Base> >::type stmt =
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
