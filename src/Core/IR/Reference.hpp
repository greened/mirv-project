#ifndef mirv_Core_IR_Reference_hpp
#define mirv_Core_IR_Reference_hpp

#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/Expression.hpp>

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
  template<>
  class Reference<Array> { 
  private:
    // We need to manually define the interface to override
    // InnerExpression's type() implementation.
    typedef InnerExpression RootType;
    typedef boost::mpl::vector<> PropertiesList;

    /// The metafunction result.
    typedef Properties<
      PropertyExpressionGenerator,
      RootType,
      PropertiesList,
      VisitedInherit2<ExpressionVisitor>
      >::type HierarchyType;
    
    typedef VisitedInherit2<ExpressionVisitor>::apply<
      HierarchyType,
      boost::enable_shared_from_this<
        Expression<Reference<Array> > > >::type InterfaceBaseType;

    class Interface : public InterfaceBaseType {
    public:
      template<typename Sequence>
      Interface(ChildPtr Base, const Sequence &indices)
          : InterfaceBaseType(Base) {
        // Add the indices.
        boost::fusion::for_each(indices,
                                boost::bind(&Interface::push_back, this, _1));
      }
      template<typename InputIterator>
      Interface(ChildPtr Base, InputIterator start, InputIterator end)
          : InterfaceBaseType(Base) {
        // Add the indices.
        std::for_each(start,
                      end,
                      boost::bind(&Interface::push_back, this, _1));
      }

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(this->shared_from_this());
      }

      typedef ptr<Symbol<Type<TypeBase> > >::const_type TypePtr;
      TypePtr type(void) const {
        // The type is the type of the array with each dimension
        // stripped off.
        ptr<Symbol<Type<Array> > >::const_type arrayType =
          safe_cast<const Symbol<Type<Array> > >((*this->begin())->type());

        if ((this->size() - 1) == arrayType->dimensionSize()) {
          // We completely indexed the array.
          return arrayType->getElementType();
        }

        Symbol<Type<Array> >::ConstDimensionIterator dimStart =
          arrayType->dimensionBegin();

        Symbol<Type<Array> >::ConstDimensionIterator dimEnd = dimStart;
        // Size of this expression includes the base object and we
        // only want to count dimensions.
        std::advance(dimEnd, arrayType->dimensionSize() - (this->size() - 1));

        ptr<Symbol<Module> >::type module =
          arrayType->parent<Symbol<Module> >();

        checkInvariant(module, "Could not get parent module for type");

        Symbol<Module>::TypeIterator arefType = module->
          typeFind(Array::getName(arrayType->getElementType(),
                                  dimStart,
                                  dimEnd));
        
        if (arefType != module->typeEnd()) {
          return *arefType;
        }

        // Create a new array type and add it to the module.
        TypePtr result =
          make<Symbol<Type<Array> > >(arrayType->getElementType(),
          dimStart,
          dimEnd);
        module->typePushBack(result);
        return result;
      }
    };

   public:
     typedef PropertiesList Properties;
     typedef InnerExpression VisitorBaseType;
     typedef Interface BaseType;
  };
}

#endif
