// Test printing of satements.
//
// STDOUT: doWhile
// STDOUT:    {
// STDOUT:       assign
// STDOUT:          vref a
// STDOUT:          +
// STDOUT:             vref a
// STDOUT:             tref
// STDOUT:                cref int32 * & b
// STDOUT:                cref int64 0
// STDOUT:       ifElse
// STDOUT:          >
// STDOUT:             tref
// STDOUT:                cref int32 * & b
// STDOUT:                cref int64 0
// STDOUT:             vref c
// STDOUT:          {
// STDOUT:             assign
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   vref a
// STDOUT:                   tref
// STDOUT:                      cref int32 * & b
// STDOUT:                      cref int64 0
// STDOUT:          }
// STDOUT:          {
// STDOUT:             assign
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   vref a
// STDOUT:                   vref c
// STDOUT:          }
// STDOUT:    }
// STDOUT:    <
// STDOUT:       vref a
// STDOUT:       vref c

#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/DoWhileGrammar.hpp>
#include <mirv/Core/Builder/IfElseGrammar.hpp>
#include <mirv/Core/Builder/Domain.hpp>
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

using Builder::if_;
using Builder::do_;

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

  ptr<Symbol<GlobalVariable> >::type bsym =
    make<Symbol<GlobalVariable> >("b", inttype);
  module->globalVariablePushBack(bsym);

  ptr<Symbol<Variable> >::type csym = make<Symbol<Variable> >("c", inttype);
  function->variablePushBack(csym);

  Builder::VariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};

  ptr<Node<Base> >::type stmt =
    Builder::translateWithGrammar<Builder::ConstructStatementGrammar>(
      module, function,
      do_[
	a = a + b,
        if_(b > c) [
          a = a + b
        ].else_[
          a = a + c
        ]
      ].while_(a < c)
    );

  PrintFilter print(std::cout);
  
  print(stmt);

  return(0);
}
