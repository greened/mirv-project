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

  /// Take the address of an lvalue expression.
  class AddressOf { 
  public:
    // TODO: Interface that checks for lvalues.
    typedef Expression<Unary> BaseType;
    typedef Expression<Unary> VisitorBaseType;
    // TODO: Override type().
  };

  /// Dereference the address provided by some expression.
  class Dereference { 
  public:
    typedef Expression<Unary> BaseType;
    typedef Expression<Unary> VisitorBaseType;
    // TODO: Override type().
  };

  /// Specify the interface for array index expressions.
  template<>
  class Reference<Array> { 
  private:
    // We need to manually define the interface to override
    // InnerExpression's type() implementation.
    class Interface
        : public InnerExpression,
    //public Expression<Ref>,
          public boost::enable_shared_from_this<Expression<Reference<Array> > > {
    private:
      Expression<Base> *cloneImpl(void) {
        std::vector<ptr<Expression<Base> >::type> children;

        for (auto i = begin(); i != end(); ++i) {
          children.push_back((*i)->clone());
        }

        ptr<Expression<Reference<Array> > >::type expr(
          mirv::make<Expression<Reference<Array> > >(*children.begin(),
                                                     children.begin() + 1,
                                                     children.end()));
        Expression<Reference<Array> > *result = expr.get();
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

      typedef ptr<Symbol<Type<TypeBase> > >::const_type TypePtr;
      TypePtr type(void) const;
    };

   public:
     typedef InnerExpression VisitorBaseType;
     typedef Interface BaseType;
  };
}

#endif
