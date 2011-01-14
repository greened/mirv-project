#ifndef mirv_Core_IR_Reference_hpp
#define mirv_Core_IR_Reference_hpp

#include <mirv/Core/IR/Symbol.hpp>

#include <boost/bind.hpp>

#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/size.hpp>

namespace mirv {
  /// Specify the interface for nodes that reference symbols.
   template<typename SymbolType>
   class Reference { 
   private:
     typedef InnerImpl<Symbol<SymbolType>, LeafExpression> InterfaceBaseType;

     class Interface
         : public InterfaceBaseType,
           public boost::enable_shared_from_this<Expression<Reference<SymbolType> > > {
     private:
       Expression<Base> *cloneImpl(void) {
         typename ptr<Expression<Reference<SymbolType> > >::type expr(
           Expression<Reference<SymbolType> >::make(this->getSymbol()));
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
  class ArrayRef { 
  public:
    // TODO: Support multi-dimension arrays natively?
    typedef Expression<Binary> BaseType;
    typedef Expression<Binary> VisitorBaseType;
    // TODO: Override type().
  };
}

#endif
