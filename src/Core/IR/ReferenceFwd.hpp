#ifndef mirv_Core_IR_ReferenceFwd_hpp
#define mirv_Core_IR_ReferenceFwd_hpp

#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/Builder/Make.hpp>

namespace mirv {
  template<typename SymbolType>
  class Reference;
  class Load;
  class TuplePointer;

  namespace detail {
    template<typename SymbolType>
    class ReferenceBaseGenerator {
    private:
      typedef InnerImpl<Symbol<SymbolType>, LeafExpression> InterfaceBaseType;

      class Interface : public InterfaceBaseType,
                        public boost::enable_shared_from_this<Expression<Reference<SymbolType> > > {
      private:
        Expression<Base> *cloneImpl(void) {
          ptr<Expression<Reference<SymbolType> > > expr(
            make<Expression<Reference<SymbolType> > >(this->getSymbol()));
          Expression<Reference<SymbolType> > *result = expr.get();
          expr.reset();
          return result;
        }

      protected:
        void setParents(void) {}

      public:
        typedef Symbol<SymbolType> ChildType;
        typedef typename ChildType::TypePtr TypePtr;
        typedef ptr<ChildType> ChildPtr;
        typedef ptr<const ChildType> ConstChildPtr;

        Interface(ChildPtr Var) : InterfaceBaseType(Var) {}

        ptr<Node<Base> > getSharedHandle(void) {
          return fast_cast<Node<Base>>(this->shared_from_this());
        }
        ptr<const Node<Base> > getSharedHandle(void) const {
          return fast_cast<const Node<Base>>(this->shared_from_this());
        }

        void setSymbol(ChildPtr c) {
          if (this->empty()) {
            push_back(c);
          }
          else {
            *this->begin() = c;
          }
        }
        
        ChildPtr getSymbol(void) {
          return this->front();
        }
        
        ConstChildPtr getSymbol(void) const {
          return this->front();
        }

        TypePtr type(void) const {
          return (*this->begin())->type();
        }
      };
    public:
      typedef Interface type;
    };

    /// Define the base type of reference expressions.
    template<typename SymbolType>
    struct BaseTypeOfReferenceExpression {
      typedef typename ReferenceBaseGenerator<SymbolType>::type BaseType;
    };

    /// Define the visitation base type for reference expressions.
    template<typename SymbolType>
    struct VisitorBaseTypeOfExpression<Reference<SymbolType> > {
      typedef LeafExpression VisitorBaseType;
    };
    /// Define the base type of reference expressions.
    template<typename SymbolType>
    struct BaseTypeOfExpression<Reference<SymbolType> > {
      typedef typename BaseTypeOfReferenceExpression<SymbolType>::BaseType BaseType;
    };

    class LoadInterface;

    /// Define the visitation base type for load expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Load> {
      typedef Expression<Unary> VisitorBaseType;
    };
    /// Define the base type of load expressions.
    template<>
    struct BaseTypeOfExpression<Load> {
      typedef LoadInterface BaseType;
    };

    class TuplePointerInterface;

    /// Define the visitation base type for tuple pointer expressions.
    template<>
    struct VisitorBaseTypeOfExpression<TuplePointer> {
      typedef InnerExpression VisitorBaseType;
    };
    /// Define the base type of tuple pointer expressions.
    template<>
    struct BaseTypeOfExpression<TuplePointer> {
      typedef TuplePointerInterface BaseType;
    };
  }
}

#endif
