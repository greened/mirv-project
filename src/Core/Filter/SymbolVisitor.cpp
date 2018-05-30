#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/Visitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  void SymbolVisitor::visit(Symbol & s) {}

  void SymbolVisitor::visit(Module & s) {
    doVisit(*this, s);
  }

  void SymbolVisitor::visit(Function & s) {
    doVisit(*this, s);
  }

  void SymbolVisitor::visit(GlobalVariable & s) {
    doVisit(*this, s);
  }

  void
  SymbolVisitor::visit(const Type & s) {}

  void
  SymbolVisitor::visit(const DerivedType & s) {
    doVisit(*this, s);
  }

  void
  SymbolVisitor::visit(const VoidType & s) {
    doVisit(*this, s);
  }

  void
  SymbolVisitor::visit(const IntegerType & s) {
    doVisit(*this, s);
  }

  void
  SymbolVisitor::visit(const FloatingPointType & s) {
    doVisit(*this, s);
  }

  void
  SymbolVisitor::visit(const PointerType & s) {
    doVisit(*this, s);
  }

  void
  SymbolVisitor::visit(const FunctionType & s) {
    doVisit(*this, s);
  }

  void
  SymbolVisitor::visit(const TupleType & s) {
    doVisit(*this, s);
  }

  void
  SymbolVisitor::visit(const PlaceholderType & s) {
    doVisit(*this, s);
  }
}
