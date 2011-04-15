// Test printing of modules.
//
// STDOUT: mdef Test {
// STDOUT:    gvdecl c int32
// STDOUT:    fdecl foo void ()
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
// STDOUT:                      tref
// STDOUT:                         cref int32 * & c
// STDOUT:                         cref int32 0
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
// STDOUT:                            tref
// STDOUT:                               cref int32 * & c
// STDOUT:                               cref int32 0
// STDOUT:                   }
// STDOUT:             }
// STDOUT:             <
// STDOUT:                vref a
// STDOUT:                tref
// STDOUT:                   cref int32 * & c
// STDOUT:                   cref int32 0
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/AddressConstant.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::Variable;
using mirv::GlobalVariable;
using mirv::Type;
using mirv::TypeBase;
using mirv::Integral;
using mirv::Pointer;
using mirv::FunctionType;
using mirv::Tuple;
using mirv::Expression;
using mirv::Constant;
using mirv::Address;
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
  ptr<Symbol<Type<Integral> > >::const_type type =
    make<Symbol<Type<Integral> > >(32);
  ptr<Symbol<Type<Pointer> > >::const_type ptrtype =
    make<Symbol<Type<Pointer> > >(type);

  ptr<Symbol<Variable> >::type a = Symbol<Variable>::make("a", type);
  ptr<Symbol<Variable> >::type b = Symbol<Variable>::make("b", type);
  ptr<Symbol<GlobalVariable> >::type c =
    Symbol<GlobalVariable>::make("c", type);

  ptr<Statement<Base> >::type dowhile =
    Statement<DoWhile>::make(
      Expression<LessThan>::make(
        Expression<Reference<Variable> >::make(a),
        Expression<Reference<Tuple> >::make(
          Expression<Reference<Constant<Base> > >::make(
            Symbol<Constant<Address> >::make(ptrtype, c)),
          Expression<Reference<Constant<Base> > >::make(
            Symbol<Constant<std::uint64_t> >::make(
              type, 0)))),
      Statement<Block>::make(
        Statement<Assignment>::make(
          Expression<Reference<Variable> >::make(a),
	  Expression<Add>::make(
	    Expression<Reference<Variable> >::make(a),
	    Expression<Reference<Variable> >::make(b))),
	Statement<IfElse>::make(
          Expression<GreaterThan>::make(
            Expression<Reference<Variable> >::make(b),
            Expression<Reference<Tuple> >::make(
              Expression<Reference<Constant<Base> > >::make(
                Symbol<Constant<Address> >::make(ptrtype, c)),
              Expression<Reference<Constant<Base> > >::make(
                Symbol<Constant<std::uint64_t> >::make(
                  type, 0)))),
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
                Expression<Reference<Tuple> >::make(
                  Expression<Reference<Constant<Base> > >::make(
                    Symbol<Constant<Address> >::make(ptrtype, c)),
                  Expression<Reference<Constant<Base> > >::make(
                    Symbol<Constant<std::uint64_t> >::make(
                      type, 0)))))))));

  ptr<Symbol<Type<TypeBase> > >::type ftype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >::type());

  ptr<Symbol<Function> >::type f =
    make<Symbol<Function> >("foo", ftype, dowhile);

  f->variablePushBack(a);
  f->variablePushBack(b);

  ptr<Symbol<Module> >::type m =
    Symbol<Module>::make("Test");

  m->typePushBack(type);
  m->typePushBack(ftype);
  m->functionPushBack(f);
  m->globalVariablePushBack(c);

  PrintFilter print(std::cout);

  print(m);

  return(0);
}
