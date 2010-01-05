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

#include <mirv/core/ir/ir.hh>
#include <mirv/core/builder/builder.hh>
#include <mirv/filter/snapshot/print/print.hh>

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

int main(void)
{
  Builder::Variable a =
    {{Symbol<Variable>::make("a", make<Symbol<Type<Integral<32> > > >())}};
  Builder::Variable b =
    {{Symbol<Variable>::make("b", make<Symbol<Type<Integral<32> > > >())}};
  Builder::Variable c =
    {{Symbol<Variable>::make("c", make<Symbol<Type<Integral<32> > > >())}};

  boost::proto::display_expr(
      if_(a < b) [
        a = a + c
      ]
  );

  ptr<Statement<Base> >::type stmt =
    Builder::translate_statement(
      if_(a < b) [
        a = a + c
      ]
    );

  PrintFilter print(std::cout);
  
  print(stmt);

  return(0);
}
