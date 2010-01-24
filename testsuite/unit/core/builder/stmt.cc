// Test printing of satements.
//
// STDOUT: doWhile
// STDOUT:    {
// STDOUT:       assign
// STDOUT:          vref a
// STDOUT:          +
// STDOUT:             vref a
// STDOUT:             vref b
// STDOUT:       ifElse
// STDOUT:          >
// STDOUT:             vref b
// STDOUT:             vref c
// STDOUT:          {
// STDOUT:             assign
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   vref a
// STDOUT:                   vref b
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

#include <mirv/core/ir/IR.hpp>
#include <mirv/core/builder/Builder.hpp>
#include <mirv/filter/snapshot/print/Print.hpp>

using mirv::Symbol;
using mirv::Variable;
using mirv::Type;
using mirv::Integral;
using mirv::Statement;
using mirv::Base;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::make;

namespace Builder = mirv::Builder;

using Builder::if_;
using Builder::do_;

int main(void)
{
  Builder::Variable a =
    {{Symbol<Variable>::make("a", make<Symbol<Type<Integral> > >("int32", 32))}};
  Builder::Variable b =
    {{Symbol<Variable>::make("b", make<Symbol<Type<Integral> > >("int32", 32))}};
  Builder::Variable c =
    {{Symbol<Variable>::make("c", make<Symbol<Type<Integral> > >("int32", 32))}};

  ptr<Statement<Base> >::type stmt =
    Builder::translate_statement(
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
