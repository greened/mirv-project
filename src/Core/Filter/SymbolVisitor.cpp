#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Base> >::type s) {};
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<InnerSymbol>::type s) {
    ptr<InnerSymbol::VisitorBaseType>::type p =
      fast_cast<InnerSymbol::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<LeafSymbol>::type s) {
    ptr<LeafSymbol::VisitorBaseType>::type p =
      fast_cast<LeafSymbol::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Typed> >::type s) {
    ptr<Symbol<Typed>::VisitorBaseType>::type p =
      fast_cast<Symbol<Typed>::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Named> >::type s) {
    ptr<Symbol<Named>::VisitorBaseType>::type p =
      fast_cast<Symbol<Named>::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Module> >::type s) {
    ptr<Symbol<Module>::VisitorBaseType>::type p =
      fast_cast<Symbol<Module>::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Function> >::type s) {
    ptr<Symbol<Function>::VisitorBaseType>::type p =
      fast_cast<Symbol<Function>::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Variable> >::type s) {
    ptr<Symbol<Variable>::VisitorBaseType>::type p =
      fast_cast<Symbol<Variable>::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::type s) {
    ptr<Symbol<Type<TypeBase> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Type<TypeBase> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<InnerType>::type s) {
    ptr<InnerType::VisitorBaseType>::type p =
      fast_cast<InnerType::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<LeafType>::type s) {
    ptr<LeafType::VisitorBaseType>::type p =
      fast_cast<LeafType::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Simple> > >::type s) {
    ptr<Symbol<Type<Simple> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Type<Simple> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Derived> > >::type s) {
    ptr<Symbol<Type<Derived> >::VisitorBaseType>::type p = fast_cast<Symbol<Type<Derived> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Integral> > >::type s) {
    ptr<Symbol<Type<Integral> >::VisitorBaseType>::type p = fast_cast<Symbol<Type<Integral> >::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Floating> > >::type s) {
    ptr<Symbol<Type<Floating> >::VisitorBaseType>::type p = fast_cast<Symbol<Type<Floating> >::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Array> > >::type s) {
    ptr<Symbol<Type<Array> >::VisitorBaseType>::type p = fast_cast<Symbol<Type<Array> >::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Pointer> > >::type s) {
    ptr<Symbol<Type<Pointer> >::VisitorBaseType>::type p = fast_cast<Symbol<Type<Pointer> >::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<FunctionType> > >::type s) {
    ptr<Symbol<Type<FunctionType> >::VisitorBaseType>::type p = fast_cast<Symbol<Type<FunctionType> >::VisitorBaseType>(s);
    visit(p);
  }
}
