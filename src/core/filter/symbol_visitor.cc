#include <mirv/core/filter/symbol_visitor.hh>
#include <mirv/core/util/cast.hh>

#include <mirv/core/ir/module.hh>
#include <mirv/core/ir/function.hh>
#include <mirv/core/ir/variable.hh>
#include <mirv/core/ir/type.hh>

namespace mirv {
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Base> >::type s) {};
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<InnerSymbol>::type s) {
    ptr<InnerSymbol::visitor_base_type>::type p =
      fast_cast<InnerSymbol::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<LeafSymbol>::type s) {
    ptr<LeafSymbol::visitor_base_type>::type p =
      fast_cast<LeafSymbol::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Typed> >::type s) {
    ptr<Symbol<Typed>::visitor_base_type>::type p =
      fast_cast<Symbol<Typed>::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Named> >::type s) {
    ptr<Symbol<Named>::visitor_base_type>::type p =
      fast_cast<Symbol<Named>::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Module> >::type s) {
    ptr<Symbol<Module>::visitor_base_type>::type p =
      fast_cast<Symbol<Module>::visitor_base_type>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Function> >::type s) {
    ptr<Symbol<Function>::visitor_base_type>::type p =
      fast_cast<Symbol<Function>::visitor_base_type>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Variable> >::type s) {
    ptr<Symbol<Variable>::visitor_base_type>::type p =
      fast_cast<Symbol<Variable>::visitor_base_type>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::type s) {
    ptr<Symbol<Type<TypeBase> >::visitor_base_type>::type p =
      fast_cast<Symbol<Type<TypeBase> >::visitor_base_type>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<InnerType>::type s) {
    ptr<InnerType::visitor_base_type>::type p =
      fast_cast<InnerType::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<LeafType>::type s) {
    ptr<LeafType::visitor_base_type>::type p =
      fast_cast<LeafType::visitor_base_type>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Simple> > >::type s) {
    ptr<Symbol<Type<Simple> >::visitor_base_type>::type p =
      fast_cast<Symbol<Type<Simple> >::visitor_base_type>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Derived> > >::type s) {
    ptr<Symbol<Type<Derived> >::visitor_base_type>::type p = fast_cast<Symbol<Type<Derived> >::visitor_base_type>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Integral> > >::type s) {
    ptr<Symbol<Type<Integral> >::visitor_base_type>::type p = fast_cast<Symbol<Type<Integral> >::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Floating> > >::type s) {
    ptr<Symbol<Type<Floating> >::visitor_base_type>::type p = fast_cast<Symbol<Type<Floating> >::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Array> > >::type s) {
    ptr<Symbol<Type<Array> >::visitor_base_type>::type p = fast_cast<Symbol<Type<Array> >::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<Pointer> > >::type s) {
    ptr<Symbol<Type<Pointer> >::visitor_base_type>::type p = fast_cast<Symbol<Type<Pointer> >::visitor_base_type>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Type<FunctionType> > >::type s) {
    ptr<Symbol<Type<FunctionType> >::visitor_base_type>::type p = fast_cast<Symbol<Type<FunctionType> >::visitor_base_type>(s);
    visit(p);
  }
}
