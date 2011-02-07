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

#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/StructType.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Relational.hpp>
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
  ptr<Symbol<Type<Integral> > >::type int32type =
    make<Symbol<Type<Integral> > >(32);

  ptr<Symbol<Variable> >::type a =
    Symbol<Variable>::make("a", int32type);
  ptr<Symbol<Variable> >::type b =
    Symbol<Variable>::make("b", int32type);
  ptr<Symbol<Variable> >::type c =
    Symbol<Variable>::make("c", int32type);

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
