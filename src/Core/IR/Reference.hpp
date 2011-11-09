#ifndef mirv_Core_IR_Reference_hpp
#define mirv_Core_IR_Reference_hpp

#include <mirv/Core/IR/ReferenceFwd.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Expression.hpp>

#include <boost/bind.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/size.hpp>

#include <vector>

namespace mirv {
  /// Specify the interface for nodes that reference symbols.
   template<typename SymbolType>
   class Reference { 
   public:
     typedef LeafExpression VisitorBaseType;
     typedef typename detail::ReferenceBaseGenerator<SymbolType>::type BaseType;
   };

  namespace detail {
    // We need to manually define the interface to override
    // InnerExpression's type() implementation.
    class LoadInterface : public Expression<Unary>,
    //public Expression<Ref>,
          public boost::enable_shared_from_this<Expression<Load> > {
    private:
      Expression<Base> *cloneImpl(void) {
        ptr<Expression<Load> >::type expr(
          mirv::make<Expression<Load> >(getOperand()->clone()));
        Expression<Load> *result = expr.get();
        expr.reset();
        return result;
      }

      void doValidation(void) const;

    public:
      LoadInterface(ChildPtr address);

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(this->shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(this->shared_from_this());
      }

      typedef ptr<Symbol<Type<TypeBase> > >::const_type TypePtr;
      TypePtr type(void) const;
    };
  }

  /// Specify the interface for load expressions.
  class Load { 
  private:
    typedef detail::LoadInterface Interface;

   public:
    typedef Expression<Unary> VisitorBaseType;
    typedef Interface BaseType;
  };

  namespace detail {
        // TODO: Interface that checks for lvalues.
    // We need to manually define the interface to override
    // InnerExpression's type() implementation.
    /// Define the interface for tuple address expressions.
    class TuplePointerInterface
        : public InnerExpression,
    //public Expression<Ref>,
          public boost::enable_shared_from_this<Expression<TuplePointer> > {
    private:
      Expression<Base> *cloneImpl(void) {
        std::vector<ptr<Expression<Base> >::type> children;

        for (auto i = begin(); i != end(); ++i) {
          children.push_back((*i)->clone());
        }

        ptr<Expression<TuplePointer> >::type expr(
          mirv::make<Expression<TuplePointer> >(*children.begin(),
                                                children.begin() + 1,
                                                children.end()));
        Expression<TuplePointer> *result = expr.get();
        expr.reset();
        return result;
      }
 
    public:
      TuplePointerInterface(ChildPtr Base, ChildPtr Index)
          : InnerExpression(Base, Index) {}

      template<typename Sequence>
      TuplePointerInterface(ChildPtr Base, const Sequence &indices)
          : InnerExpression(Base, indices) {}

      TuplePointerInterface(ChildPtr Base, ChildPtr Index1, ChildPtr Index2)
          : InnerExpression(Base, Index1, Index2) {}

      template<typename Sequence>
      TuplePointerInterface(ChildPtr Base, ChildPtr Index, const Sequence &indices)
          : InnerExpression(Base, Index, indices) {}

      template<typename InputIterator>
      TuplePointerInterface(ChildPtr Base, InputIterator start, InputIterator end)
          : InnerExpression(Base, start, end) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(this->shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(this->shared_from_this());
      }

      typedef ptr<Symbol<Type<TypeBase> > >::const_type TypePtr;
      TypePtr type(void) const;
    };
  }
  
  /// Take the address of a tuple item.
  class TuplePointer { 
  public:
    typedef detail::TuplePointerInterface Interface;

   public:
     typedef InnerExpression VisitorBaseType;
     typedef Interface BaseType;
    // TODO: Override type().
  };
}

#endif
