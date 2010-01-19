#ifndef mirv_core_ir_reference_hh
#define mirv_core_ir_reference_hh

#include <mirv/core/ir/symbol.hh>

namespace mirv {
  /// Specify the interface for nodes that reference symbols.
   template<typename SymbolType>
   class Reference {
   private:
     typedef boost::mpl::vector<> sequence;
     typedef InnerImpl<Symbol<SymbolType>, LeafExpression> interface_base_type;

   public:
     typedef sequence properties;
     typedef LeafExpression visitor_base_type;

     class interface : public interface_base_type {
      public:
       typedef Symbol<SymbolType> child_type;
       typedef typename ptr<child_type>::type child_ptr;
       typedef typename ptr<child_type>::const_type const_child_ptr;

       interface(child_ptr Var) : interface_base_type(Var) {}

         void set_symbol(child_ptr c) {
            if (this->empty()) {
               push_back(c);
            }
            else {
               *this->begin() = c;
            }
         };
        
         child_ptr get_symbol(void) {
            return this->front();
         };
        
         const_child_ptr get_symbol(void) const {
            return this->front();
         };
      };

     typedef typename ExpressionBaseGenerator<sequence, interface>::type base_type;
   };

  /// Take the address of an lvalue expression.
   class AddressOf {
   private:
      typedef boost::mpl::vector<> sequence;

     // TODO: Interface that checks for lvalues.
   public:
     typedef sequence properties;
     typedef Expression<Unary> visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, Expression<Unary> >::type base_type;
   };

  /// Dereference the address provided by some expression.
  class Dereference {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef sequence properties;
     typedef Expression<Unary> visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, Expression<Unary> >::type base_type;
   };

  /// Specify the interface for array index expressions.
   class ArrayRef {
   private:
      typedef boost::mpl::vector<> sequence;

     // TODO: Support multi-dimension arrays natively?
   public:
     typedef sequence properties;
     typedef Expression<Binary> visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
   };

  /// Specify the interface for function call expressions.
   class Call {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef sequence properties;
     typedef InnerExpression visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, InnerExpression>::type base_type;
   };
}

#endif
