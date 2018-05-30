#include <mirv/Core/Filter/ValueVisitor.hpp>
#include <mirv/Core/Filter/Visitor.hpp>
#include <mirv/Core/IR/Consumer.hpp>
#include <mirv/Core/IR/Constants.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Global.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  void ValueVisitor::visit(Producer & e) {}

  void ValueVisitor::visit(Consumer & e) {}

  void ValueVisitor::visit(ValueProducer & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(ValueConsumer & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(DataProducer & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(DataConsumer & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Expression & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Global & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Function & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(GlobalVariable & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Allocate & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Unary & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Binary & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Add & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Subtract & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Divide & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Multiply & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Modulus & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Negate & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(LogicalAnd & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(LogicalOr & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(LogicalNot & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(BitwiseAnd & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(BitwiseOr & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(BitwiseXor & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(BitwiseComplement & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(ShiftLeft & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(ArithmeticShiftRight & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(LogicalShiftRight & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(LessThan & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(LessThanOrEqual & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Equal & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(NotEqual & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(GreaterThan & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(GreaterThanOrEqual & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Load & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Store & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Phi & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(FunctionCall & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(PureFunctionCall & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(TuplePointer & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(Constant & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(IntegerConstant & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(FloatingPointConstant & e) {
    doVisit(*this, e);
  }

  void ValueVisitor::visit(TupleConstant & e) {
    doVisit(*this, e);
  }
}
