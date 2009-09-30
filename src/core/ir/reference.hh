#ifndef MIRV_Core_IR_Reference_hh
#define MIRV_Core_IR_Reference_hh

namespace MIRV {

   template<typename SymbolType>
   class Ref {
   private:
      typedef boost::mpl::vector<ReferenceSemantic> sequence;

     typedef Inner<Symbol<SymbolType>, BaseExpression> >::type
     interface_base_type;

   public:
     class interface 
            : public interface_base_type {
      public:
         typedef Symbol<SymbolType> child_type;
         typedef ptr<child_type>::type child_ptr
         typedef ptr<child_type>::const_type const_child_ptr;
        
         void set_symbol(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         };
        
         child_ptr get_symbol(void) {
            return(front());
         };
        
         const_child_ptr get_symbol(void) const {
            return(front());
         };
      };

      typedef ExpressionBaseGenerator<sequence, interface>::type base_type;
   };

   class AddressOf {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
   };

   class Dereference {
   private:
      typedef boost::mpl::vector<ReferenceSemantic> sequence;

   public:
      typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
   };

   class ArrayRef {
   private:
      typedef boost::mpl::vector<ReferenceSemantic> sequence;

   public:
      typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
   };

   class Label {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
   };

   class Call {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef ExpressionBaseGenerator<sequence, InnerExpression>::type base_type;
   };
}

#endif
