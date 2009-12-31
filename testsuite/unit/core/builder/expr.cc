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

#include <mirv/core/ir/symbol.hh>
#include <mirv/core/ir/variable.hh>
#include <mirv/core/ir/expression.hh>
#include <mirv/core/ir/arithmetic.hh>
#include <mirv/core/ir/logical.hh>
#include <mirv/core/ir/bitwise.hh>
#include <mirv/core/ir/reference.hh>
#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/control.hh>
#include <mirv/core/ir/mutating.hh>

#include <mirv/core/builder/expression.hh>
#include <mirv/core/builder/grammar.hh>
#include <mirv/core/builder/make.hh>
#include <mirv/core/builder/translate.hh>
#include <mirv/filter/snapshot/print/print.hh>
#include <mirv/filter/snapshot/print/print.hh>

using mirv::Symbol;
using mirv::Variable;
using mirv::Type;
using mirv::Integral;
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
  Builder::Variable a =
    {Symbol<Variable>::make("a", make<Symbol<Type<Integral<32> > > >())};
  Builder::Variable b =
    {Symbol<Variable>::make("b", make<Symbol<Type<Integral<32> > > >())};
  Builder::Variable c =
    {Symbol<Variable>::make("c", make<Symbol<Type<Integral<32> > > >())};
  Builder::Variable d =
    {Symbol<Variable>::make("d", make<Symbol<Type<Integral<32> > > >())};
  Builder::Variable e =
    {Symbol<Variable>::make("e", make<Symbol<Type<Integral<32> > > >())};

  ptr<Expression<Base> >::type expr =
    Builder::translate_expression(a + (b - c) * d / -e);

  PrintFilter print(std::cout);

  print(expr);

  return(0);
}
