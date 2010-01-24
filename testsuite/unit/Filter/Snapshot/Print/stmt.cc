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
using mirv::LessThan;
using mirv::GreaterThan;
using mirv::Statement;
using mirv::Block;
using mirv::IfElse;
using mirv::DoWhile;
using mirv::Assignment;
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

  ptr<Statement<Base> >::type dowhile =
    Statement<DoWhile>::make(
      Expression<LessThan>::make(
        Expression<Reference<Variable> >::make(a),
	Expression<Reference<Variable> >::make(c)),
      Statement<Block>::make(
        Statement<Assignment>::make(
          Expression<Reference<Variable> >::make(a),
	  Expression<Add>::make(
	    Expression<Reference<Variable> >::make(a),
	    Expression<Reference<Variable> >::make(b))),
	Statement<IfElse>::make(
          Expression<GreaterThan>::make(
            Expression<Reference<Variable> >::make(b),
	    Expression<Reference<Variable> >::make(c)),
        Statement<Block>::make(
          Statement<Assignment>::make(
            Expression<Reference<Variable> >::make(a),
  	    Expression<Add>::make(
	      Expression<Reference<Variable> >::make(a),
	      Expression<Reference<Variable> >::make(b)))),
        Statement<Block>::make(
          Statement<Assignment>::make(
            Expression<Reference<Variable> >::make(a),
  	    Expression<Add>::make(
	      Expression<Reference<Variable> >::make(a),
	      Expression<Reference<Variable> >::make(c)))))));
   
   PrintFilter print(std::cout);

   print(dowhile);

   return(0);
}
