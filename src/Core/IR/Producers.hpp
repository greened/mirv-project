#ifndef mirv_Core_IR_Producers_hpp
#define mirv_Core_IR_Producers_hpp

#include <mirv/Core/IR/DataConsumer.hpp>
#include <mirv/Core/IR/DataProducer.hpp>
#include <mirv/Core/IR/NodeName.hpp>
#include <mirv/Core/IR/NodeType.hpp>
#include <mirv/Core/IR/ValueConsumer.hpp>
#include <mirv/Core/IR/ValueProducer.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/Filter/ValueVisitor.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>

#include <utility>

namespace mirv {
  class Control;
  class ValueVisitor;

  class Expression : public ValueProducer,
                     public ValueConsumer {
    NodeName TheName;

    std::string getNameImpl(void) const override {
      return TheName.getName();
    }

  protected:
    template<typename ...Args>
    Expression(std::string N, Args ...args) :
      ValueConsumer(args...), TheName(std::move(N)) {};

  public:
    typedef TypeList<ValueConsumer, ValueProducer> VisitorBaseTypes;
  };

  class Allocate : public ValueProducer {
    friend class IRBuilder;

    NodeName TheName;
    NodeType TheType;
    NodeType ValType;

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    TypeHandle getTypeImpl(void) const override {
      return TheType.getType();
    }

    std::string getNameImpl(void) const override {
      return TheName.getName();
    }

  protected:
    Allocate(std::string N, TypeHandle T);

    static ptr<Allocate> Make(const std::string N, TypeHandle T) {
      return getHandle(new Allocate(std::move(N), T));
    }

  public:
    typedef TypeList<ValueProducer> VisitorBaseTypes;

    ptr<const Type> ValueType(void) const {
      return ValType.getType();
    }
  };

  class Unary : public Expression {
  protected:
    Unary(const std::string N, ProducerHandle V) :
      Expression(N, V) {};

    TypeHandle getTypeImpl(void) const override {
      return getOperand()->getType();
    }

  public:
    typedef TypeList<Expression> VisitorBaseTypes;

    ProducerHandle getOperand(void) const {
      checkInvariant(!OperandsEmpty(), "No operand");
      return *Operands().begin();
    }
  };

  class Negate : public Unary {
    friend class IRBuilder;

    Negate(const std::string N, ProducerHandle V) :
      Unary(std::move(N), V) {};

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Negate> Make(const std::string &Name,
                            ProducerHandle Op) {
      return getHandle(new Negate(Name, Op));
    }

  public:
    typedef TypeList<Unary> VisitorBaseTypes;
  };

  class Binary : public Expression {
  protected:
    Binary(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Expression(std::move(N), Left, Right) {
      checkInvariant(getLeft()->getType() == getRight()->getType(),
                     "Mismatched types");
    };

    TypeHandle getTypeImpl(void) const override {
      return getLeft()->getType();
    }

  public:
    typedef TypeList<Expression> VisitorBaseTypes;

    ProducerHandle getLeft(void) const {
      checkInvariant(OperandsSize() == 2, "Wrong number of operands");
      return *Operands().begin();
    }
    ProducerHandle getRight(void) const {
      checkInvariant(OperandsSize() == 2, "Wrong number of operands");
      return *(--Operands().end());
    }
  };

  class Add : public Binary {
    friend class IRBuilder;

    Add(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Add> Make(const std::string Name,
                         ProducerHandle Left,
                         ProducerHandle Right) {
      return getHandle(new Add(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class Subtract : public Binary {
    friend class IRBuilder;

    Subtract(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Subtract> Make(const std::string Name,
                              ProducerHandle Left,
                              ProducerHandle Right) {
      return getHandle(new Subtract(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class Multiply : public Binary {
    friend class IRBuilder;

    Multiply(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Multiply> Make(const std::string &Name,
                              ProducerHandle Left,
                              ProducerHandle Right) {
      return getHandle(new Multiply(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class Divide : public Binary {
    friend class IRBuilder;

    Divide(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Divide> Make(const std::string Name,
                            ProducerHandle Left,
                            ProducerHandle Right) {
      return getHandle(new Divide(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class Modulus : public Binary {
    friend class IRBuilder;

    Modulus(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Modulus> Make(const std::string Name,
                             ProducerHandle Left,
                             ProducerHandle Right) {
      return getHandle(new Modulus(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class LogicalAnd : public Binary {
    friend class IRBuilder;

    LogicalAnd(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<LogicalAnd> Make(const std::string Name,
                                ProducerHandle Left,
                                ProducerHandle Right) {
      return getHandle(new LogicalAnd(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class LogicalOr : public Binary {
    friend class IRBuilder;

    LogicalOr(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<LogicalOr> Make(const std::string Name,
                               ProducerHandle Left,
                               ProducerHandle Right) {
      return getHandle(new LogicalOr(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class LogicalNot : public Unary {
    friend class IRBuilder;

    LogicalNot(const std::string N, ProducerHandle V) :
      Unary(std::move(N), std::move(V)) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<LogicalNot> Make(const std::string Name, ProducerHandle V) {
      return getHandle(new LogicalNot(std::move(Name), V));
    }

  public:
    typedef TypeList<Unary> VisitorBaseTypes;
  };

  class BitwiseAnd : public Binary {
    friend class IRBuilder;

    BitwiseAnd(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<BitwiseAnd> Make(const std::string Name,
                                ProducerHandle Left,
                                ProducerHandle Right) {
      return getHandle(new BitwiseAnd(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class BitwiseOr : public Binary {
    friend class IRBuilder;

    BitwiseOr(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<BitwiseOr> Make(const std::string Name,
                               ProducerHandle Left,
                               ProducerHandle Right) {
      return getHandle(new BitwiseOr(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class BitwiseXor : public Binary {
    friend class IRBuilder;

    BitwiseXor(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<BitwiseXor> Make(const std::string Name,
                                ProducerHandle Left,
                                ProducerHandle Right) {
      return getHandle(new BitwiseXor(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class BitwiseComplement : public Unary {
    friend class IRBuilder;

    BitwiseComplement(std::string N, ProducerHandle V) :
      Unary(std::move(N), std::move(V)) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<BitwiseComplement> Make(const std::string Name,
                                       ProducerHandle V) {
      return getHandle(new BitwiseComplement(std::move(Name), V));
    }

  public:
    typedef TypeList<Unary> VisitorBaseTypes;
  };

  class ShiftLeft : public Binary {
    friend class IRBuilder;

    ShiftLeft(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<ShiftLeft> Make(const std::string Name,
                               ProducerHandle Left,
                               ProducerHandle Right) {
      return getHandle(new ShiftLeft(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class ArithmeticShiftRight : public Binary {
    friend class IRBuilder;

    ArithmeticShiftRight(const std::string N,
                         ProducerHandle Left,
                         ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<ArithmeticShiftRight> Make(const std::string Name,
                                          ProducerHandle Left,
                                          ProducerHandle Right) {
      return getHandle(new ArithmeticShiftRight(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class LogicalShiftRight : public Binary {
    friend class IRBuilder;

    LogicalShiftRight(const std::string N,
                      ProducerHandle Left,
                      ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<LogicalShiftRight> Make(const std::string Name,
                                       ProducerHandle Left,
                                       ProducerHandle Right) {
      return getHandle(new LogicalShiftRight(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class Relational : public Binary {
  protected:
    Relational(const std::string N,
               ProducerHandle Left,
               ProducerHandle Right) :
      Binary(std::move(N), Left, Right) {}

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class LessThan : public Relational {
    friend class IRBuilder;

    LessThan(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Relational(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<LessThan> Make(const std::string Name,
                              ProducerHandle Left,
                              ProducerHandle Right) {
      return getHandle(new LessThan(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class LessThanOrEqual : public Relational {
    friend class IRBuilder;

    LessThanOrEqual(const std::string N,
                    ProducerHandle Left,
                    ProducerHandle Right) :
      Relational(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<LessThanOrEqual> Make(const std::string Name,
                                     ProducerHandle Left,
                                     ProducerHandle Right) {
      return getHandle(new LessThanOrEqual(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class Equal : public Relational {
    friend class IRBuilder;

    Equal(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Relational(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Equal> Make(const std::string Name,
                           ProducerHandle Left,
                           ProducerHandle Right) {
      return getHandle(new Equal(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class NotEqual : public Relational {
    friend class IRBuilder;

    NotEqual(const std::string N, ProducerHandle Left, ProducerHandle Right) :
      Relational(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<NotEqual> Make(const std::string Name,
                              ProducerHandle Left,
                              ProducerHandle Right) {
      return getHandle(new NotEqual(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class GreaterThanOrEqual : public Relational {
    friend class IRBuilder;

    GreaterThanOrEqual(const std::string N,
                       ProducerHandle Left,
                       ProducerHandle Right) :
      Relational(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<GreaterThanOrEqual> Make(const std::string Name,
                                        ProducerHandle Left,
                                        ProducerHandle Right) {
      return getHandle(new GreaterThanOrEqual(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class GreaterThan : public Relational {
    friend class IRBuilder;

    GreaterThan(const std::string N,
                ProducerHandle Left,
                ProducerHandle Right) :
      Relational(std::move(N), Left, Right) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<GreaterThan> Make(const std::string Name,
                                 ProducerHandle Left,
                                 ProducerHandle Right) {
      return getHandle(new GreaterThan(std::move(Name), Left, Right));
    }

  public:
    typedef TypeList<Binary> VisitorBaseTypes;
  };

  class TuplePointer : public Expression {
    friend class IRBuilder;

    template<typename ...Args>
    TuplePointer(const std::string N,
                 Args ...args) :
      Expression(std::move(N), args...) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    TypeHandle getTypeImpl(void) const override;

    template<typename ...Args>
    static ptr<TuplePointer> Make(const std::string N,
                                  Args ...args) {
      return getHandle(new TuplePointer(std::move(N), args...));
    }
  };

  class Load : public Unary,
               public DataConsumer {
    friend class IRBuilder;

    Load(const std::string N, ptr<ValueProducer> Address) :
      Unary(std::move(N), Address) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Load>
    Make(const std::string Name, ptr<ValueProducer> Address) {
      return getHandle(new Load(std::move(Name), Address));
    }

    TypeHandle getTypeImpl(void) const override;

  public:
    typedef TypeList<Unary, DataConsumer> VisitorBaseTypes;
  };

  class Store : public ValueConsumer,
                public DataProducer {
    friend class IRBuilder;

    NodeName TheName;

    Store(const std::string N,
          ptr<ValueProducer> Address,
          ptr<ValueProducer> V) : ValueConsumer(Address, V),
                                  TheName(std::move(N)) {}

    static ptr<Store> Make(const std::string N,
                           ptr<ValueProducer> Address,
                           ptr<ValueProducer> V) {
      return getHandle(new Store(std::move(N), Address, V));
    }

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    std::string getNameImpl(void) const override {
      return TheName.getName();
    }

    TypeHandle getTypeImpl(void) const override;

  public:
    ProducerHandle getStoredValue(void) const {
      return *(++Operands().begin());
    }

  public:
    typedef TypeList<ValueConsumer, DataProducer> VisitorBaseTypes;
  };

  class Phi : public Expression {
    friend class IRBuilder;

    template<typename ...Args>
    Phi(const std::string N, Args ...args) :
      Expression(std::move(N), args...) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

  public:
    using VisitorBaseTypes = TypeList<Expression>;
  };

  // Returns a value.
  class FunctionCall : public Expression {
    friend class IRBuilder;

    template<typename Address, typename ...Arguments>
    FunctionCall(const std::string Name, Address F, Arguments ...A) :
      Expression(std::move(Name), F, A...) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

  public:
    using VisitorBaseTypes = TypeList<Expression>;
  };

  // No side effects.
  class PureFunctionCall : public FunctionCall {
    friend class IRBuilder;

    template<typename Address, typename ...Arguments>
    PureFunctionCall(const std::string Name, Address F, Arguments ...A) :
      FunctionCall(std::move(Name), F, A...) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

  public:
    typedef TypeList<FunctionCall> VisitorBaseTypes;
  };
};

#endif
