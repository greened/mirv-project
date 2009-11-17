#ifndef mirv_core_ir_reference_hh
#define mirv_core_ir_reference_hh

#include <mirv/ir/symbol.hh>

namespace mirv {
   template<typename SymbolType>
   class Ref {
   private:
     typedef boost::mpl::vector<> sequence;
     typedef InnerImpl<Symbol<SymbolType>, LeafStatement> interface_base_type;
   public:
     class interface : public interface_base_type {
      public:
       typedef Symbol<SymbolType> child_type;
       typedef typename ptr<child_type>::type child_ptr;
       typedef typename ptr<child_type>::const_type const_child_ptr;
        
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

   class AddressOf {
   private:
      typedef boost::mpl::vector<> sequence;

     // TODO: Interface that checks for lvalues.
   public:
      typedef ExpressionBaseGenerator<sequence, Expression<Unary> >::type base_type;
   };

   class Dereference {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef ExpressionBaseGenerator<sequence, Expression<Unary> >::type base_type;
   };

   class ArrayRef {
   private:
      typedef boost::mpl::vector<> sequence;

     // TODO: Support multi-dimension arrays natively?
   public:
      typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
   };

   class Call {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef ExpressionBaseGenerator<sequence, InnerExpression>::type base_type;
   };
}

#endif
