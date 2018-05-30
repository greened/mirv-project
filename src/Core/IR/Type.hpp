#ifndef mirv_Core_IR_Type_hpp
#define mirv_Core_IR_Type_hpp

#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>
#include <mirv/Library/Vector.hpp>

#include <initializer_list>

namespace mirv {
  class IRBuilder;
  class PlaceholderType;
  class Type;
  class VallueProducer;

  class Type : public Attributable {
    virtual std::string getNameImpl(void) const = 0;
    virtual void acceptImpl(SymbolVisitor &) const = 0;

  public:
    using VisitorBaseTypes = TypeList<>;

    typedef ptr<const Type> TypeHandle;

    Type() : Attributable() {}

    typedef ptr<ValueProducer> BitSizeType;

    virtual BitSizeType BitSize(void) const = 0;

    virtual void
    resolve(ptr<const PlaceholderType> placeholder,
            TypeHandle replacement) {}

    std::string getName(void) const {
      return getNameImpl();
    }

    void accept(SymbolVisitor &V) const {
      acceptImpl(V);
    }

    void dump(void) const;
  };

  class VoidType : public Type {
    friend class IRBuilder;

    virtual std::string getNameImpl(void) const override;

    void acceptImpl(SymbolVisitor &V) const override {
      V.visit(*this);
    }

    BitSizeType BitSize(void) const override;

    VoidType(void) {}

    static ptr<const VoidType> Make(void) {
      return getHandle(new VoidType);
    }

  public:
    using VisitorBaseTypes = TypeList<const Type>;
  };

  /// A type with no children that has a specific bit size, for
  /// example integer and floating point types.
  class ScalarType : public Type {
  public:
    typedef std::uint64_t SizeType;

  private:
    SizeType Size;

  protected:
    SizeType getSize(void) const {
      return Size;
    }

  public:
    ScalarType(SizeType s) : Size(s) {};

    BitSizeType BitSize(void) const override;
  };

  /// Integer types
  class IntegerType : public ScalarType {
    friend class IRBuilder;

    virtual std::string getNameImpl(void) const override;

    void acceptImpl(SymbolVisitor &V) const override {
      V.visit(*this);
    }

    static ptr<const IntegerType> Make(SizeType BitSize) {
      return getHandle(new IntegerType(BitSize));
    }

  protected:
    IntegerType(SizeType BitSize) : ScalarType(BitSize) {}

  public:
    using VisitorBaseTypes = TypeList<const Type>;
  };

  /// Floating point types
  class FloatingPointType : public ScalarType {
    friend class IRBuilder;

    virtual std::string getNameImpl(void) const override;

    void acceptImpl(SymbolVisitor &V) const override {
      V.visit(*this);
    }

    static ptr<const FloatingPointType> Make(SizeType BitSize) {
      return getHandle(new FloatingPointType(BitSize));
    }

  protected:
    FloatingPointType(SizeType BitSize) : ScalarType(BitSize) {}

  public:
    using VisitorBaseTypes = TypeList<const Type>;
  };

  /// A type that is built upon other types.  For example structures
  /// and pointers.
  class DerivedType : public Type {
    typedef Vector<TypeHandle> TypeVector;
    TypeVector MemberTypes;

  protected:
    void push_back(TypeHandle T) {
      MemberTypes.push_back(T);
    }

    DerivedType(void) = default;

    DerivedType(std::initializer_list<TypeHandle> L) : MemberTypes(L) {}

    template<typename ...Args>
    DerivedType(Args ...args) :
      MemberTypes(std::move(args)...) {}

    typedef TypeVector::iterator iterator;
    typedef TypeVector::const_iterator const_iterator;

    iterator begin(void) {
      return MemberTypes.begin();
    }

    const_iterator begin(void) const {
      return MemberTypes.begin();
    }

    iterator end(void) {
      return MemberTypes.end();
    }

    const_iterator end(void) const {
      return MemberTypes.end();
    }

  public:
    using VisitorBaseTypes = TypeList<const Type>;

    auto Types(void) {
      return MakeRange(MemberTypes.begin(), MemberTypes.end());
    }
    auto Types(void) const {
      return MakeRange(MemberTypes.begin(), MemberTypes.end());
    }

    bool empty(void) const {
      return MemberTypes.empty();
    }

    void resolve(ptr<const PlaceholderType> placeholder,
                 TypeHandle replacement) override;
  };

  /// A tuple type.  Multidimensional tuples are represented by tuples
  /// of tuples.
  class TupleType : public DerivedType {
    friend class IRBuilder;

    ptr<ValueProducer> Multiplier;
    std::string Name;

    void BuildMultiplier(std::uint64_t Count);

    template<typename InputIterator>
    void Construct(InputIterator Start, InputIterator End) {
      // See if we can optimize this.
      if (Start == End) {
        return;
      }

      TypeHandle T = *Start;
      std::uint64_t Count = 0;
      for (auto i = Start; i != End; ++i) {
        if (*i != T) {
          // Found a non-matching type, not uniform.
          while (Start != End) {
            push_back(*Start++);
          }
          return;
        }
        ++Count;
      }

      // We can optimize.
      push_back(*Start);
      BuildMultiplier(Count);
    }

    virtual std::string getNameImpl(void) const override;

    void acceptImpl(SymbolVisitor &V) const override {
      V.visit(*this);
    }

    TupleType(const std::string &N,
              ptr<ValueProducer> Count,
              TypeHandle ElementType) :
      DerivedType({ElementType}), Multiplier(Count), Name(N) {}

    TupleType(std::string &&N,
              ptr<ValueProducer> Count,
              TypeHandle ElementType) :
      DerivedType({ElementType}), Multiplier(Count), Name(std::move(N)) {}

    template<typename ...Args>
    TupleType(const std::string &N, TypeHandle T, Args ...args) :
      Multiplier(nullptr), Name(N) {
      Vector<TypeHandle> Members{T, args...};
      Construct(Members.begin(), Members.end());
    }

    static ptr<const TupleType>
    Make(const std::string &Name,
         ptr<ValueProducer> Count,
         TypeHandle ElementType) {
      return getHandle(new TupleType(Name, Count, ElementType));
    }

    static ptr<const TupleType>
    Make(std::string &&Name,
         ptr<ValueProducer> Count,
         TypeHandle ElementType) {
      return getHandle(new TupleType(std::move(Name), Count, ElementType));
    }

    template<typename ...Args>
    static ptr<const TupleType> Make(const std::string &Name,
                                     ptr<const Type> T,
                                     Args ...args) {
      return getHandle(new TupleType(Name, T, args...));
    }

  public:
    using VisitorBaseTypes = TypeList<const DerivedType>;

    TypeHandle ElementType(ptr<const ValueProducer>) const;

    BitSizeType BitSize(void) const override;

    bool isUniform(void) const {
      return Multiplier != nullptr;
    }

    ptr<ValueProducer> Count(void) const {
      checkInvariant(isUniform(), "Tuple is not uniform");
      return Multiplier;
    }
  };

  class PointerType : public DerivedType {
    friend class IRBuilder;

    virtual std::string getNameImpl(void) const override;

    void acceptImpl(SymbolVisitor &V) const override {
      V.visit(*this);
    }

    PointerType(TypeHandle Base) : DerivedType({Base}) {}

    static ptr<const PointerType> Make(TypeHandle B) {
      return getHandle(new PointerType(B));
    }

  public:
    using VisitorBaseTypes = TypeList<const DerivedType>;

    BitSizeType BitSize(void) const override;

    TypeHandle getBaseType(void) const {
      return *begin();
    }
  };

  class FunctionType : public DerivedType {
    friend class IRBuilder;

  public:
    enum VarargMark {
      Vararg = true,
      NotVararg = false
    };

  private:
    /// Mark whether this function is vararg.
    VarargMark vararg;

    virtual std::string getNameImpl(void) const override;

    void acceptImpl(SymbolVisitor &V) const override {
      V.visit(*this);
    }

    template<typename ...Args>
    FunctionType(VarargMark V,
                 TypeHandle ReturnType,
                 Args ...args) :
      DerivedType{ReturnType, std::move(args)...}, vararg(V) {}

    template<typename ...Args>
    static ptr<const FunctionType> Make(VarargMark V,
                                        TypeHandle ReturnType,
                                        Args ...args) {
      return getHandle(new FunctionType(V,
                                        ReturnType,
                                        std::move(args)...));
    }

  public:
    using VisitorBaseTypes = TypeList<const DerivedType>;

    BitSizeType BitSize(void) const override;

    bool isVararg(void) const {
      return vararg == VarargMark::Vararg;
    }

    TypeHandle getReturnType(void) const {
      return(*begin());
    }

    auto ParameterTypes(void) const {
      return MakeRange(++begin(), end());
    }
  };

  /// Placeholder types are used to stand in for another type during
  /// construction of recursive types.
  class PlaceholderType : public Type {
    friend class IRBuilder;

    std::string Name;

    virtual std::string getNameImpl(void) const override;

    void acceptImpl(SymbolVisitor &V) const override {
      V.visit(*this);
    }

    BitSizeType BitSize(void) const override;

    PlaceholderType(const std::string &name) : Name(name) {};
    PlaceholderType(std::string &&name) : Name(std::move(name)) {};

    static ptr<const PlaceholderType> Make(const std::string &Name) {
      return getHandle(new PlaceholderType(Name));
    }

    static ptr<const PlaceholderType> Make(std::string &&Name) {
      return getHandle(new PlaceholderType(Name));
    }

  public:
    using VisitorBaseTypes = TypeList<const Type>;
  };
}

#endif
