#ifndef mirv_core_ir_Reference_hpp
#define mirv_core_ir_Reference_hpp

#include <mirv/core/ir/Symbol.hpp>

namespace mirv {
  /// Specify the interface for nodes that reference symbols.
   template<typename SymbolType>
   class Reference { 
   private:
     typedef InnerImpl<Symbol<SymbolType>, LeafExpression> InterfaceBaseType;
     class Interface : public InterfaceBaseType {
      public:
       typedef Symbol<SymbolType> ChildType;
       typedef typename ptr<ChildType>::type ChildPtr;
       typedef typename ptr<ChildType>::const_type ConstChildPtr;

       Interface(ChildPtr Var) : InterfaceBaseType(Var) {}

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
     };

   public:
     typedef boost::mpl::vector<> Properties;
     typedef LeafExpression VisitorBaseType;
     typedef typename ExpressionBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// Take the address of an lvalue expression.
  class AddressOf { 
  public:
    typedef boost::mpl::vector<> Properties;

    // TODO: Interface that checks for lvalues.
    typedef Expression<Unary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Unary> >::type BaseType;
  };

  /// Dereference the address provided by some expression.
  class Dereference { 
  public:
    typedef boost::mpl::vector<> Properties;
    typedef Expression<Unary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Unary> >::type BaseType;
  };

  /// Specify the interface for array index expressions.
  class ArrayRef { 
  public:
    typedef boost::mpl::vector<> Properties;
    // TODO: Support multi-dimension arrays natively?
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface for function call expressions.
  class Call {
  public:
    typedef boost::mpl::vector<> Properties;
    typedef InnerExpression VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, InnerExpression>::type BaseType;
   };
}

#endif
