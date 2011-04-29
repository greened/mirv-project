// Test printing of satements.
//
// STDOUT: ifThen
// STDOUT:    <
// STDOUT:       tref
// STDOUT:          vref a
// STDOUT:          cref int64 0
// STDOUT:       tref
// STDOUT:          vref c
// STDOUT:          cref int64 0
// STDOUT:    {
// STDOUT:       doWhile
// STDOUT:          {
// STDOUT:             assign
// STDOUT:                tref
// STDOUT:                   vref a
// STDOUT:                   cref int64 0
// STDOUT:                +
// STDOUT:                   tref
// STDOUT:                      vref a
// STDOUT:                      cref int64 0
// STDOUT:                   tref
// STDOUT:                      cref int32 * & b
// STDOUT:                      cref int64 0
// STDOUT:             ifElse
// STDOUT:                >
// STDOUT:                   tref
// STDOUT:                      cref int32 * & b
// STDOUT:                      cref int64 0
// STDOUT:                   tref
// STDOUT:                      vref c
// STDOUT:                      cref int64 0
// STDOUT:                {
// STDOUT:                   assign
// STDOUT:                      tref
// STDOUT:                         vref a
// STDOUT:                         cref int64 0
// STDOUT:                      +
// STDOUT:                         tref
// STDOUT:                            vref a
// STDOUT:                            cref int64 0
// STDOUT:                         tref
// STDOUT:                            cref int32 * & b
// STDOUT:                            cref int64 0
// STDOUT:                }
// STDOUT:                {
// STDOUT:                   assign
// STDOUT:                      tref
// STDOUT:                         vref a
// STDOUT:                         cref int64 0
// STDOUT:                      +
// STDOUT:                         tref
// STDOUT:                            vref a
// STDOUT:                            cref int64 0
// STDOUT:                         tref
// STDOUT:                            vref c
// STDOUT:                            cref int64 0
// STDOUT:                }
// STDOUT:          }
// STDOUT:          <
// STDOUT:             tref
// STDOUT:                vref a
// STDOUT:                cref int64 0
// STDOUT:             tref
// STDOUT:                vref c
// STDOUT:                cref int64 0
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
  ptr<Symbol<Module> >::type module = make<Symbol<Module> >("testmodule");

  ptr<Symbol<Type<TypeBase> > >::type functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >::type());
  module->typePushBack(functype);

  ptr<Symbol<Type<TypeBase> > >::type inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  ptr<Symbol<Type<TypeBase> > >::type ptrtype =
    make<Symbol<Type<Pointer> > >(inttype);
  module->typePushBack(ptrtype);

  ptr<Symbol<Function> >::type function =
    make<Symbol<Function> >("testfunc", functype);

  module->functionPushBack(function);

  ptr<Symbol<Variable> >::type asym = make<Symbol<Variable> >("a", ptrtype);
  function->variablePushBack(asym);

  ptr<Symbol<GlobalVariable> >::type bsym =
    make<Symbol<GlobalVariable> >("b", inttype);
  module->globalVariablePushBack(bsym);

  ptr<Symbol<Variable> >::type csym = make<Symbol<Variable> >("c", ptrtype);
  function->variablePushBack(csym);

  Builder::VariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
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
