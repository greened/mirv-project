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

#include <mirv/core/ir/IR.hpp>
#include <mirv/core/builder/Builder.hpp>
#include <mirv/filter/snapshot/print/Print.hpp>

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
    {{Symbol<Variable>::make("a", make<Symbol<Type<Integral> > >("int32", 32))}};
  Builder::Variable b =
    {{Symbol<Variable>::make("b", make<Symbol<Type<Integral> > >("int32", 32))}};
  Builder::Variable c =
    {{Symbol<Variable>::make("c", make<Symbol<Type<Integral> > >("int32", 32))}};
  Builder::Variable d =
    {{Symbol<Variable>::make("d", make<Symbol<Type<Integral> > >("int32", 32))}};
  Builder::Variable e =
    {{Symbol<Variable>::make("e", make<Symbol<Type<Integral> > >("int32", 32))}};

  ptr<Expression<Base> >::type expr =
    Builder::translate_expression(a + (b - c) * d / -e);

  PrintFilter print(std::cout);

  print(expr);

  return(0);
}
