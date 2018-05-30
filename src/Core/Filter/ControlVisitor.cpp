#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/Filter/ControlVisitor.hpp>
#include <mirv/Core/Filter/Visitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  void ControlVisitor::visit(Control & s) {};

  void ControlVisitor::visit(ValueControl & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(Block & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(CIBlock & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(IfThen & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(IfElse & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(DoWhile & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(Switch & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(Case & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(Goto & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(Sequence & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(Return & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(SubroutineCall & s) {
    doVisit(*this, s);
  }

  void ControlVisitor::visit(PureSubroutineCall & s) {
    doVisit(*this, s);
  }
}
