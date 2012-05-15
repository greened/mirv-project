// Test printing of modules.
//
// STDOUT: mdef Test {
// STDOUT:    gvdecl c int32
// STDOUT:    fdecl foo void ()
// STDOUT:    fdef foo {
// STDOUT:       vdecl a int32 *
// STDOUT:       vdecl b int32 *
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref a
// STDOUT:             cref int32 1
// STDOUT:          allocate int32
// STDOUT:             vref b
// STDOUT:             cref int32 1
// STDOUT:          doWhile
// STDOUT:             {
// STDOUT:                store
// STDOUT:                   vref a
// STDOUT:                   +
// STDOUT:                      load
// STDOUT:                         vref a
// STDOUT:                      load
// STDOUT:                         vref b
// STDOUT:                ifElse
// STDOUT:                   >
// STDOUT:                      load
// STDOUT:                         vref b
// STDOUT:                      load
// STDOUT:                         &
// STDOUT:                            cref int32 * & c
// STDOUT:                            cref int32 0
// STDOUT:                   {
// STDOUT:                      store
// STDOUT:                         vref a
// STDOUT:                         +
// STDOUT:                            load
// STDOUT:                               vref a
// STDOUT:                            load
// STDOUT:                               vref b
// STDOUT:                   }
// STDOUT:                   {
// STDOUT:                      store
// STDOUT:                         vref a
// STDOUT:                         +
// STDOUT:                            load
// STDOUT:                               vref a
// STDOUT:                            load
// STDOUT:                               &
// STDOUT:                                  cref int32 * & c
// STDOUT:                                  cref int32 0
// STDOUT:                   }
// STDOUT:             }
// STDOUT:             <
// STDOUT:                load
// STDOUT:                   vref a
// STDOUT:                load
// STDOUT:                   &
// STDOUT:                      cref int32 * & c
// STDOUT:                      cref int32 0
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
#include <mirv/Core/Utility/Cast.hpp>
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
using mirv::TuplePointer;
using mirv::Expression;
using mirv::Constant;
using mirv::Address;
using mirv::Base;
using mirv::Add;
using mirv::LessThan;
using mirv::Load;
using mirv::GreaterThan;
using mirv::Statement;
using mirv::Allocate;
using mirv::Block;
using mirv::IfElse;
using mirv::DoWhile;
using mirv::Store;
using mirv::Reference;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::make;

int main(void)
{
  ptr<const Symbol<Type<Integral> > > type =
    make<Symbol<Type<Integral> > >(32);
  ptr<const Symbol<Type<Pointer> > > ptrtype =
    make<Symbol<Type<Pointer> > >(type);
  ptr<Symbol<Type<TypeBase> > > ftype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >());

  ptr<Symbol<Module> > m =
    Symbol<Module>::make("Test");

  m->typePushBack(type);
  m->typePushBack(ptrtype);
  m->typePushBack(ftype);

  ptr<Symbol<Variable> > a = Symbol<Variable>::make("a", ptrtype);
  ptr<Symbol<Variable> > b = Symbol<Variable>::make("b", ptrtype);
  ptr<Symbol<GlobalVariable> > c =
    Symbol<GlobalVariable>::make("c", type);

  ptr<Statement<Base> > dowhile =
    Statement<Block>::make(
      Statement<Allocate>::make(
        Expression<Reference<Variable> >::make(a),
        Expression<Reference<Constant<Base> > >::make(
          Symbol<Constant<std::uint64_t> >::make(
            type, 1)),
        type),
      Statement<Allocate>::make(
        Expression<Reference<Variable> >::make(b),
        Expression<Reference<Constant<Base> > >::make(
          Symbol<Constant<std::uint64_t> >::make(
            type, 1)),
        type),
      Statement<DoWhile>::make(
        Expression<LessThan>::make(
          Expression<Load>::make(
            Expression<Reference<Variable> >::make(a)),
          Expression<Load>::make(
            Expression<TuplePointer>::make(
              mirv::fast_cast<Expression<Base> >(
                Expression<Reference<Constant<Base> > >::make(
                  Symbol<Constant<Address> >::make(ptrtype, c))),
              mirv::fast_cast<Expression<Base> >(
                Expression<Reference<Constant<Base> > >::make(
                  Symbol<Constant<std::uint64_t> >::make(
                    type, 0)))))),
        Statement<Block>::make(
          Statement<Store>::make(
            Expression<Reference<Variable> >::make(a),
            Expression<Add>::make(
              Expression<Load>::make(
                Expression<Reference<Variable> >::make(a)),
              Expression<Load>::make(
                Expression<Reference<Variable> >::make(b)))),
          Statement<IfElse>::make(
            Expression<GreaterThan>::make(
              Expression<Load>::make(
                Expression<Reference<Variable> >::make(b)),
              Expression<Load>::make(
                Expression<TuplePointer>::make(
                  mirv::fast_cast<Expression<Base> >(
                    Expression<Reference<Constant<Base> > >::make(
                      Symbol<Constant<Address> >::make(ptrtype, c))),
                  mirv::fast_cast<Expression<Base> >(
                    Expression<Reference<Constant<Base> > >::make(
                      Symbol<Constant<std::uint64_t> >::make(
                        type, 0)))))),
            Statement<Block>::make(
              Statement<Store>::make(
                Expression<Reference<Variable> >::make(a),
                Expression<Add>::make(
                  Expression<Load>::make(
                    Expression<Reference<Variable> >::make(a)),
                  Expression<Load>::make(
                    Expression<Reference<Variable> >::make(b))))),
            Statement<Block>::make(
              Statement<Store>::make(
                Expression<Reference<Variable> >::make(a),
                Expression<Add>::make(
                  Expression<Load>::make(
                    Expression<Reference<Variable> >::make(a)),
                  Expression<Load>::make(
                    Expression<TuplePointer>::make(
                      mirv::fast_cast<Expression<Base> >(
                        Expression<Reference<Constant<Base> > >::make(
                          Symbol<Constant<Address> >::make(ptrtype, c))),
                      mirv::fast_cast<Expression<Base> >(
                        Expression<Reference<Constant<Base> > >::make(
                          Symbol<Constant<std::uint64_t> >::make(
                            type, 0))))))))))));

  ptr<Symbol<Function> > f =
    make<Symbol<Function> >("foo", ftype, dowhile);

  f->variablePushBack(a);
  f->variablePushBack(b);

  m->functionPushBack(f);
  m->globalVariablePushBack(c);

  PrintFilter print(std::cout);

  print(m);

  return(0);
}
