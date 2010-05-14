// Test building of expressions.
//
// STDOUT: +
// STDOUT:    vref a
// STDOUT:    /
// STDOUT:       *
// STDOUT:          -
// STDOUT:             vref b
// STDOUT:             vref c
// STDOUT:          vref d
// STDOUT:       neg
// STDOUT:          vref e
//

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
using mirv::Symbol;
using mirv::Variable;
using mirv::Type;
using mirv::TypeBase;
using mirv::Integral;
using mirv::FunctionType;
using mirv::Expression;
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

  ptr<Symbol<Variable> >::type dsym = make<Symbol<Variable> >("d", inttype);
  function->variablePushBack(dsym);

  ptr<Symbol<Variable> >::type esym = make<Symbol<Variable> >("e", inttype);
  function->variablePushBack(esym);

  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};
  Builder::VariableTerminal d = {{"d"}};
  Builder::VariableTerminal e = {{"e"}};

  ptr<Expression<Base> >::type expr =
    Builder::translate_expression(module, function, a + (b - c) * d / -e);

  PrintFilter print(std::cout);

  print(expr);

  return(0);
}