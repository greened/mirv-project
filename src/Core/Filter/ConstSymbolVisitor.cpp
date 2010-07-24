#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/StructType.hpp>

namespace mirv {
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Base> >::const_type s) {};

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<InnerSymbol>::const_type s) {
    ptr<InnerSymbol::VisitorBaseType>::const_type p =
      fast_cast<const InnerSymbol::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<LeafSymbol>::const_type s) {
    ptr<LeafSymbol::VisitorBaseType>::const_type p =
      fast_cast<const LeafSymbol::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Typed> >::const_type s) {
    ptr<Symbol<Typed>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Typed>::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Named> >::const_type s) {
    ptr<Symbol<Named>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Named>::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Module> >::const_type s) {
    ptr<Symbol<Module>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Module>::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Function> >::const_type s) {
    ptr<Symbol<Function>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Function>::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type s) {
    ptr<Symbol<Variable>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Variable>::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::const_type s) {
    ptr<Symbol<Type<TypeBase> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<TypeBase> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<InnerType>::const_type s) {
    ptr<InnerType::VisitorBaseType>::const_type p =
      fast_cast<const InnerType::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<LeafType>::const_type s) {
    ptr<LeafType::VisitorBaseType>::const_type p =
      fast_cast<const LeafType::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Simple> > >::const_type s) {
    ptr<Symbol<Type<Simple> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Simple> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Derived> > >::const_type s) {
    ptr<Symbol<Type<Derived> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Derived> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Integral> > >::const_type s) {
    ptr<Symbol<Type<Integral> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Integral> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Floating> > >::const_type s) {
    ptr<Symbol<Type<Floating> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Floating> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Array> > >::const_type s) {
    ptr<Symbol<Type<Array> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Array> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Pointer> > >::const_type s) {
    ptr<Symbol<Type<Pointer> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Pointer> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<FunctionType> > >::const_type s) {
    ptr<Symbol<Type<FunctionType> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<FunctionType> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<StructType> > >::const_type s) {
    ptr<Symbol<Type<StructType> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<StructType> >::VisitorBaseType>(s);
    visit(p);
  }
}
