#ifndef mirv_Core_Filter_ValueVisitor_hpp
#define mirv_Core_Filter_ValueVisitor_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Library/TypeList.hpp>

namespace mirv {
  class Producer;
  class Consumer;
  class ValueProducer;
  class ValueConsumer;
  class DataProducer;
  class DataConsumer;
  class Expression;
  class Global;
  class Function;
  class GlobalVariable;
  class Allocate;
  class Unary;
  class Binary;
  class Add;
  class Subtract;
  class Divide;
  class Multiply;
  class Modulus;
  class Negate;
  class LogicalAnd;
  class LogicalOr;
  class LogicalNot;
  class BitwiseAnd;
  class BitwiseOr;
  class BitwiseXor;
  class BitwiseComplement;
  class ShiftLeft;
  class ArithmeticShiftRight;
  class LogicalShiftRight;
  class LessThan;
  class LessThanOrEqual;
  class Equal;
  class NotEqual;
  class GreaterThanOrEqual;
  class GreaterThan;
  class Load;
  class Store;
  class Phi;
  class FunctionCall;
  class PureFunctionCall;
  class TuplePointer;
  class Constant;
  class IntegerConstant;
  class FloatingPointConstant;
  class TupleConstant;

  /// This is the base class for all visitors that know about value
  /// producers.  This includes flows, visitor actions and anything
  /// else that wants to examine or operate on value producers.
  class ValueVisitor {
  public:
    virtual ~ValueVisitor(void) = default;

    virtual void visit(Producer &);
    virtual void visit(Consumer &);
    virtual void visit(DataProducer &);
    virtual void visit(DataConsumer &);
    virtual void visit(ValueProducer &);
    virtual void visit(ValueConsumer &);
    virtual void visit(Expression &);
    virtual void visit(Global &);
    virtual void visit(Function &);
    virtual void visit(GlobalVariable &);
    virtual void visit(Allocate &);
    virtual void visit(Unary &);
    virtual void visit(Binary &);
    virtual void visit(Add &);
    virtual void visit(Subtract &);
    virtual void visit(Divide &);
    virtual void visit(Multiply &);
    virtual void visit(Modulus &);
    virtual void visit(Negate &);
    virtual void visit(LogicalAnd &);
    virtual void visit(LogicalOr &);
    virtual void visit(LogicalNot &);
    virtual void visit(BitwiseAnd &);
    virtual void visit(BitwiseOr &);
    virtual void visit(BitwiseXor &);
    virtual void visit(BitwiseComplement &);
    virtual void visit(ShiftLeft &);
    virtual void visit(ArithmeticShiftRight &);
    virtual void visit(LogicalShiftRight &);
    virtual void visit(LessThan &);
    virtual void visit(LessThanOrEqual &);
    virtual void visit(Equal &);
    virtual void visit(NotEqual &);
    virtual void visit(GreaterThanOrEqual &);
    virtual void visit(GreaterThan &);
    virtual void visit(Load &);
    virtual void visit(Store &);
    virtual void visit(Phi &);
    virtual void visit(FunctionCall &);
    virtual void visit(PureFunctionCall &);
    virtual void visit(TuplePointer &);
    virtual void visit(Constant &);
    virtual void visit(IntegerConstant &);
    virtual void visit(FloatingPointConstant &);
    virtual void visit(TupleConstant &);
  };
}

#endif
