#ifndef mirv_Core_IR_Reference_hpp
#define mirv_Core_IR_Reference_hpp

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
   private:
     typedef InnerImpl<Symbol<SymbolType>, LeafExpression> InterfaceBaseType;

     class Interface : public InterfaceBaseType,
                       public boost::enable_shared_from_this<Expression<Reference<SymbolType> > > {
     private:
       Expression<Base> *cloneImpl(void) {
         typename ptr<Expression<Reference<SymbolType> > >::type expr(
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
       typedef typename ptr<ChildType>::type ChildPtr;
       typedef typename ptr<ChildType>::const_type ConstChildPtr;

       Interface(ChildPtr Var) : InterfaceBaseType(Var) {}

       ptr<Node<Base> >::type getSharedHandle(void) {
         return fast_cast<Node<Base>>(this->shared_from_this());
       }
       ptr<Node<Base> >::const_type getSharedHandle(void) const {
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
     typedef LeafExpression VisitorBaseType;
     typedef Interface BaseType;
   };

  /// Specify the interface for array index expressions.
  template<>
  class Reference<Tuple> { 
  private:
    // We need to manually define the interface to override
    // InnerExpression's type() implementation.
    class Interface
        : public InnerExpression,
    //public Expression<Ref>,
          public boost::enable_shared_from_this<Expression<Reference<Tuple> > > {
    private:
      Expression<Base> *cloneImpl(void) {
        std::vector<ptr<Expression<Base> >::type> children;

        for (auto i = begin(); i != end(); ++i) {
          children.push_back((*i)->clone());
        }

        ptr<Expression<Reference<Tuple> > >::type expr(
          mirv::make<Expression<Reference<Tuple> > >(*children.begin(),
                                                     children.begin() + 1,
                                                     children.end()));
        Expression<Reference<Tuple> > *result = expr.get();
        expr.reset();
        return result;
      }
 
    public:
      Interface(ChildPtr Base, ChildPtr Index)
          : InnerExpression(Base, Index) {}

      template<typename ExprType>
      Interface(ChildPtr Base,
                const boost::shared_ptr<Expression<ExprType> > &Index)
          : InnerExpression(Base, fast_cast<Expression<mirv::Base> >(Index)) {}

      template<typename Sequence>
      Interface(ChildPtr Base, const Sequence &indices)
          : InnerExpression(Base, indices) {}

      template<typename InputIterator>
      Interface(ChildPtr Base, InputIterator start, InputIterator end)
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

   public:
     typedef InnerExpression VisitorBaseType;
     typedef Interface BaseType;
  };

  /// Take the address of a tuple item.
  class TuplePointer { 
  public:
    // TODO: Interface that checks for lvalues.
    // We need to manually define the interface to override
    // InnerExpression's type() implementation.
    /// Define the interface for tuple address expressions.
    class Interface
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
      Interface(ChildPtr Base, ChildPtr Index)
          : InnerExpression(Base, Index) {}

      template<typename Sequence>
      Interface(ChildPtr Base, const Sequence &indices)
          : InnerExpression(Base, indices) {}

      template<typename InputIterator>
      Interface(ChildPtr Base, InputIterator start, InputIterator end)
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

   public:
     typedef InnerExpression VisitorBaseType;
     typedef Interface BaseType;
    // TODO: Override type().
  };
}

#endif
