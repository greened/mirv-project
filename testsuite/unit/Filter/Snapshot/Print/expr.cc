// Test printing of expressions.
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
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

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
    Symbol<Variable>::make("a", make<Symbol<Type<Integral> > >("int32", 32));
  ptr<Symbol<Variable> >::type b =
    Symbol<Variable>::make("b", make<Symbol<Type<Integral> > >("int32", 32));
  ptr<Symbol<Variable> >::type c =
    Symbol<Variable>::make("c", make<Symbol<Type<Integral> > >("int32", 32));
  ptr<Symbol<Variable> >::type d =
    Symbol<Variable>::make("d", make<Symbol<Type<Integral> > >("int32", 32));
  ptr<Symbol<Variable> >::type e =
    Symbol<Variable>::make("e", make<Symbol<Type<Integral> > >("int32", 32));

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
