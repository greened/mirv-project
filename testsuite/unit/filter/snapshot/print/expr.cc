// Test printing of expressions.
//
// STDOUT:
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

#include <mirv/core/builder/make.hh>
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

int main(void)
{
  ptr<Symbol<Variable> >::type a =
    Symbol<Variable>::make("a", make<Symbol<Type<Integral<32> > > >());
   ptr<Symbol<Variable> >::type b =
     Symbol<Variable>::make("b", make<Symbol<Type<Integral<32> > > >());
   ptr<Symbol<Variable> >::type c =
     Symbol<Variable>::make("c", make<Symbol<Type<Integral<32> > > >());
   ptr<Symbol<Variable> >::type d =
     Symbol<Variable>::make("d", make<Symbol<Type<Integral<32> > > >());
   ptr<Symbol<Variable> >::type e =
     Symbol<Variable>::make("e", make<Symbol<Type<Integral<32> > > >());

   // a + (b - c) * d / -e)
   ptr<Expression<Base> >::type expr =
     Expression<Add>::make(
       Expression<Reference<Variable> >::make(a),
       Expression<Divide>::make(
         Expression<Multiply>::make(
           Expression<Subtract>::make(
	     Expression<Reference<Variable> >::make(b),
	     Expression<Reference<Variable> >::make(c)),
	   Expression<Reference<Variable> >::make(d)),
	 Expression<Negate>::make(Expression<Reference<Variable> >::make(e))));
   
   PrintFilter print(std::cout);

   print(expr);

   return(0);
}
