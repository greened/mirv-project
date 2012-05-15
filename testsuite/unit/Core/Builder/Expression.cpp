// Test building of expressions.
//
//
// STDOUT: +
// STDOUT:    load
// STDOUT:       vref a
// STDOUT:    /
// STDOUT:       *
// STDOUT:          -
// STDOUT:             load
// STDOUT:                cref int32 * & b
// STDOUT:             load
// STDOUT:                vref c
// STDOUT:          load
// STDOUT:             vref d
// STDOUT:       neg
// STDOUT:          load
// STDOUT:             vref e

#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
using mirv::Symbol;
using mirv::Variable;
using mirv::GlobalVariable;
using mirv::Type;
using mirv::TypeBase;
using mirv::Integral;
using mirv::Pointer;
using mirv::FunctionType;
using mirv::Node;
using mirv::Base;
using mirv::Add;
using mirv::Subtract;
using mirv::Multiply;
using mirv::Divide;
using mirv::Negate;
using mirv::Reference;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::make;

namespace Builder = mirv::Builder;

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

  ptr<Symbol<Variable> > dsym = make<Symbol<Variable> >("d", ptrtype);
  function->variablePushBack(dsym);

  ptr<Symbol<Variable> > esym = make<Symbol<Variable> >("e", ptrtype);
  function->variablePushBack(esym);

  Builder::VariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};
  Builder::VariableTerminal d = {{"d"}};
  Builder::VariableTerminal e = {{"e"}};

  ptr<Node<Base> > expr =
    Builder::translateWithGrammar<Builder::ConstructExpressionGrammar>(
      module, function, a + (b - c) * d / -e);

  PrintFilter print(std::cout);

  print(expr);

  return(0);
}
