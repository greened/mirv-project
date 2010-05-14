// Test printing of modules.
//
// STDOUT: mdef Test {
// STDOUT:    vdecl c int32
// STDOUT:    fdecl foo
// STDOUT:    fdef foo {
// STDOUT:       vdecl a int32
// STDOUT:       vdecl b int32
// STDOUT:       {
// STDOUT:          doWhile
// STDOUT:             {
// STDOUT:                assign
// STDOUT:                   vref a
// STDOUT:                   +
// STDOUT:                      vref a
// STDOUT:                      vref b
// STDOUT:                ifElse
// STDOUT:                   >
// STDOUT:                      vref b
// STDOUT:                      vref c
// STDOUT:                   {
// STDOUT:                      assign
// STDOUT:                         vref a
// STDOUT:                         +
// STDOUT:                            vref a
// STDOUT:                            vref b
// STDOUT:                   }
// STDOUT:                   {
// STDOUT:                      assign
// STDOUT:                         vref a
// STDOUT:                         +
// STDOUT:                            vref a
// STDOUT:                            vref c
// STDOUT:                   }
// STDOUT:             }
// STDOUT:             <
// STDOUT:                vref a
// STDOUT:                vref c
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::Variable;
using mirv::Type;
using mirv::TypeBase;
using mirv::Integral;
using mirv::FunctionType;
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
    Symbol<Variable>::make("a", make<Symbol<Type<Integral> > >(32));
  ptr<Symbol<Variable> >::type b =
    Symbol<Variable>::make("b", make<Symbol<Type<Integral> > >(32));
  ptr<Symbol<Variable> >::type c =
    Symbol<Variable>::make("c", make<Symbol<Type<Integral> > >(32));

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

  ptr<Symbol<Type<TypeBase> > >::type ftype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >::type());

  ptr<Symbol<Function> >::type f =
    make<Symbol<Function> >("foo", ftype, dowhile);

  f->variablePushBack(a);
  f->variablePushBack(b);

  ptr<Symbol<Module> >::type m =
    Symbol<Module>::make("Test");

  m->typePushBack(make<Symbol<Type<Integral> > >(32));
  m->typePushBack(ftype);
  m->functionPushBack(f);
  m->variablePushBack(c);

  PrintFilter print(std::cout);

  print(m);

  return(0);
}