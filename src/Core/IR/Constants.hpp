#ifndef mirv_Core_IR_Constants_hpp
#define mirv_Core_IR_Constants_hpp

#include <mirv/Core/Filter/ValueVisitor.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Library/TypeList.hpp>

namespace mirv {
  template<typename ScalarType>
  class ScalarConstant : public Constant {
  public:
    typedef ScalarType ValueType;
    typedef Producer ProducerVisitorBaseType;

  private:
    ValueType Value;

  protected:
    ScalarConstant(ptr<const Type> T, ValueType V) :
      Constant(T->getName() + "." + std::to_string(V), T), Value(V) {}

  public:
    using VisitorBaseTypes = TypeList<Constant>;

    ValueType getValue(void) const {
      return Value;
    }
  };

  class IntegerConstant : public ScalarConstant<int64_t> {
    friend class IRBuilder;

  public:
    using VisitorBaseTypes = TypeList<Constant>;

  private:
    IntegerConstant(ptr<const Type> T, ValueType V) :
      ScalarConstant(T, V) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<IntegerConstant> Make(ptr<const Type> T, ValueType V) {
      return getHandle(new IntegerConstant(T, V));
    }
  };

  class FloatingPointConstant : public ScalarConstant<double> {
    friend class IRBuilder;

  public:
    typedef double ValueType;
    using VisitorBaseTypes = TypeList<Constant>;

  private:
    FloatingPointConstant(ptr<const Type> T, ValueType V) :
      ScalarConstant(T, V) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    static ptr<FloatingPointConstant> Make(ptr<const Type> T, ValueType V) {
      return getHandle(new FloatingPointConstant(T, V));
    }
  };

  class TupleConstant : public Constant,
                        public ValueConsumer {
    friend class IRBuilder;

    template<typename ...Args>
    TupleConstant(ptr<const Type> T, Args &&...args) :
      Constant("Tuple( " + (... + (args->getName() + " ")) + " )", T),
      ValueConsumer(std::forward<Args>(args)...) {}

    template<typename Iterator>
    TupleConstant(ptr<const Type> T, Range<Iterator> R) :
      Constant("Tuple( " + [] (Range<Iterator> R) {
          std::string Result;
          for (auto V : R){
            Result += V->getName() + " ";
          }
          return Result;
        } (R) + " )", T),
      ValueConsumer(R) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    template<typename ...Args>
    static ptr<TupleConstant>
    Make(ptr<const Type> T, Args &&...args) {
      return getHandle(new TupleConstant(T, std::forward<Args>(args)...));
    }

    template<typename Iterator>
    static ptr<TupleConstant>
    Make(ptr<const Type> T, Range<Iterator> R) {
      return getHandle(new TupleConstant(T, R));
    }

  public:
    using VisitorBaseTypes = TypeList<Constant, ValueConsumer>;
  };
}

#endif
