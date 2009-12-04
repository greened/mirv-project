// Test printing of expressions.
//
// STDOUT:
//

#include <mirv/core/ir/symbol.hh>
#include <mirv/filter/snapshot/print.hh>

using mirv::Symbol;
using mirv::Variable;
using mirv::Type;
using mirv::Integral;
using mirv::Expression;
using mirv::Add;
using mirv::Sub;
using mirv::Multiply;
using mirv::Divide;
using mirv::Negate;
using mirv::Reference;
using mirv::ptr;
using mirv::PrintFilter;

int main(void)
{
  ptr<Symbol<Variable> >::type asym =
    Symbol<Variable>::make("a", Symbol<Type<Integral<32> > >());
   ptr<Symbol<Variable> >::type bsym =
     Symbol<Variable>::make("b", Symbol<Type<Integral<32> > >());
   ptr<Symbol<Variable> >::type csym =
     Symbol<Variable>::make("c", Symbol<Type<Integral<32> > >());
   ptr<Symbol<Variable> >::type dsym =
     Symbol<Variable>::make("d", Symbol<Type<Integral<32> > >());
   ptr<Symbol<Variable> >::type esym =
     Symbol<Variable>::make("e", Symbol<Type<Integral<32> > >());

   // a + (b - c) * d / -e)
   ptr<Expression<Base> >::type expr =
     Expression<Add>::make(
       Expression<Reference>::make(a),
       Expression<Divide>::make(
         Expression<Multiply>::make(
	   Expression<Sub>::make(Expression<Reference>::make(b),
				 Expression<Reference>::make(c))
	   Expresssion<Reference>::make(d)),
	 Expression<Negate>::make(Expression<Reference>::make(e))));
   
   PrintFilter print(std::cout);

   print(expr);

   return(0);
}
