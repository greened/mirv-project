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
     typedef InnerImpl<Symbol<SymbolType>, LeafExpression> RootType;
     typedef boost::mpl::vector<> PropertiesList;

    /// The metafunction result.
     typedef typename Properties<
       PropertyExpressionGenerator,
       RootType,
       PropertiesList,
       VisitedInherit2<ExpressionVisitor>
       >::type HierarchyType;
     
    typedef typename VisitedInherit2<ExpressionVisitor>::template apply<
      HierarchyType,
      boost::enable_shared_from_this<Expression<Reference<SymbolType> > > >::type InterfaceBaseType;

     class Interface : public InterfaceBaseType {
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
     typedef PropertiesList Properties;
     typedef LeafExpression VisitorBaseType;
     typedef Interface BaseType;
   };

  /// Take the address of an lvalue expression.
  class AddressOf { 
  public:
    typedef boost::mpl::vector<> Properties;

    // TODO: Interface that checks for lvalues.
    typedef Expression<Unary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Unary>,
      AddressOf>::type BaseType;
    // TODO: Override type().
  };

  /// Dereference the address provided by some expression.
  class Dereference { 
  public:
    typedef boost::mpl::vector<> Properties;
    typedef Expression<Unary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Unary>,
      Dereference>::type BaseType;
    // TODO: Override type().
  };

  /// Specify the interface for array index expressions.
  struct Array {};
  
  template<>
  class Reference<Array> { 
  private:
    // We need to manually define the interface to override
    // InnerExpression's type() implementation.
    typedef InnerExpression RootType;
    typedef boost::mpl::vector<> PropertiesList;

    /// The metafunction result.
    typedef typename Properties<
      PropertyExpressionGenerator,
      RootType,
      PropertiesList,
      VisitedInherit2<ExpressionVisitor>
      >::type HierarchyType;
    
    typedef typename VisitedInherit2<ExpressionVisitor>::template apply<
      HierarchyType,
      boost::enable_shared_from_this<
        Expression<Reference<SymbolType> > > >::type InterfaceBaseType;

    class Interface : public InterfaceBaseType {
    public:
      Interface(ChildPtr Base) : InterfaceBaseType(Base) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(this->shared_from_this());
      }

      typedef ptr<Symbol<Type<TypeBase> > >::const_type TypePtr;
      TypePtr type(void) const {
        // The type is the type of the array with each dimension
        // stripped off.
        ptr<Symbol<Type<Array> > >::const_type arrayType =
          safe_cast<Symbol<Type<Array> > >(*this->begin())->type();

        if ((this->size() - 1) == arrayType->dimensionSize()) {
          // We completely indexed the array.
          return arrayType->getElementType();
        }

        Symbol<Type<Array> >::DimensionIterator dimStart =
          arrayType->dimensionBegin();

        Symbol<Type<Array> >::DimensionIterator dimEnd = dimStart;
        std::advance(dimEnd, this->size() - 1);

        
      }
    };

   public:
     typedef PropertiesList Properties;
     typedef InnerExpression VisitorBaseType;
     typedef Interface BaseType;
  };
}

#endif
