#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>

namespace mirv {
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Base> >::type s) {}
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

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<Base> > >::type s) {
    ptr<Symbol<Constant<Base> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<Base> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int8_t> > >::type s) {
    ptr<Symbol<Constant<std::int8_t> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::int8_t> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint8_t> > >::type s) {
    ptr<Symbol<Constant<std::uint8_t> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::uint8_t> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int16_t> > >::type s) {
    ptr<Symbol<Constant<std::int16_t> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::int16_t> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint16_t> > >::type s) {
    ptr<Symbol<Constant<std::uint16_t> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::uint16_t> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int32_t> > >::type s) {
    ptr<Symbol<Constant<std::int32_t> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::int32_t> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint32_t> > >::type s) {
    ptr<Symbol<Constant<std::uint32_t> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::uint32_t> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int64_t> > >::type s) {
    ptr<Symbol<Constant<std::int64_t> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::int64_t> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint64_t> > >::type s) {
    ptr<Symbol<Constant<std::uint64_t> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::uint64_t> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<float> > >::type s) {
    ptr<Symbol<Constant<float> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<float> >::VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<double> > >::type s) {
    ptr<Symbol<Constant<double> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<double> >::VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::string> > >::type s) {
    ptr<Symbol<Constant<std::string> >::VisitorBaseType>::type p =
      fast_cast<Symbol<Constant<std::string> >::VisitorBaseType>(s);
    visit(p);
  }
}
