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

using namespace Builder = mirv::Builder;

int main(void)
{
  Builder::Variable a;
  Builder::Variable b;
  Builder::Variable c;
  Builder::Variable d;
  Builder::Variable e;

  ptr<Expression<Base> >::type expr = (a + (b - c) * d / -e).translate();

  PrintFilter print(std::cout);

  print(expr);

  return(0);
}
