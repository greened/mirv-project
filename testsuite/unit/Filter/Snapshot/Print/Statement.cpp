// Test printing of satements.
//
// STDOUT: {
// STDOUT:    allocate int32
// STDOUT:       vref a
// STDOUT:       cref int32 1
// STDOUT:    allocate int32
// STDOUT:       vref b
// STDOUT:       cref int32 1
// STDOUT:    allocate int32
// STDOUT:       vref c
// STDOUT:       cref int32 1
// STDOUT:    doWhile
// STDOUT:       {
// STDOUT:          store
// STDOUT:             vref a
// STDOUT:             +
// STDOUT:                load
// STDOUT:                   vref a
// STDOUT:                load
// STDOUT:                   vref b
// STDOUT:          ifElse
// STDOUT:             >
// STDOUT:                load
// STDOUT:                   vref b
// STDOUT:                load
// STDOUT:                   vref c
// STDOUT:             {
// STDOUT:                store
// STDOUT:                   vref a
// STDOUT:                   +
// STDOUT:                      load
// STDOUT:                         vref a
// STDOUT:                      load
// STDOUT:                         vref b
// STDOUT:             }
// STDOUT:             {
// STDOUT:                store
// STDOUT:                   vref a
// STDOUT:                   +
// STDOUT:                      load
// STDOUT:                         vref a
// STDOUT:                      load
// STDOUT:                         vref c
// STDOUT:             }
// STDOUT:       }
// STDOUT:       <
// STDOUT:          load
// STDOUT:             vref a
// STDOUT:          load
// STDOUT:             vref c
// STDOUT: }

#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Node;
using mirv::Symbol;
using mirv::Variable;
using mirv::Constant;
using mirv::Type;
using mirv::Integral;
using mirv::Pointer;
using mirv::Expression;
using mirv::Base;
using mirv::Add;
using mirv::LessThan;
using mirv::GreaterThan;
using mirv::Statement;
using mirv::Allocate;
using mirv::Block;
using mirv::IfElse;
using mirv::DoWhile;
using mirv::Load;
using mirv::Store;
using mirv::Reference;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::make;

int main(void)
{
  ptr<Symbol<Type<Integral> > > int32type =
    make<Symbol<Type<Integral> > >(32);
  ptr<const Symbol<Type<Pointer> > > ptrtype =
    make<Symbol<Type<Pointer> > >(int32type);

  ptr<Symbol<Variable> > a =
    Symbol<Variable>::make("a", ptrtype);
  ptr<Symbol<Variable> > b =
    Symbol<Variable>::make("b", ptrtype);
  ptr<Symbol<Variable> > c =
    Symbol<Variable>::make("c", ptrtype);

  ptr<Statement<Base> > dowhile =
    Statement<Block>::make(
      Statement<Allocate>::make(
        Expression<Reference<Variable> >::make(a),
        Expression<Reference<Constant<Base> > >::make(
          Symbol<Constant<std::uint64_t> >::make(
            int32type, 1)),
        int32type),
      Statement<Allocate>::make(
        Expression<Reference<Variable> >::make(b),
        Expression<Reference<Constant<Base> > >::make(
          Symbol<Constant<std::uint64_t> >::make(
            int32type, 1)),
        int32type),
      Statement<Allocate>::make(
        Expression<Reference<Variable> >::make(c),
        Expression<Reference<Constant<Base> > >::make(
          Symbol<Constant<std::uint64_t> >::make(
            int32type, 1)),
        int32type),
      Statement<DoWhile>::make(
        Expression<LessThan>::make(
          Expression<Load>::make(
            Expression<Reference<Variable> >::make(a)),
          Expression<Load>::make(
            Expression<Reference<Variable> >::make(c))),
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
                Expression<Reference<Variable> >::make(c))),
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
                    Expression<Reference<Variable> >::make(c)))))))));

  PrintFilter print(std::cout);

  print(boost::static_pointer_cast<Node<Base> >(dowhile));

  return(0);
}
