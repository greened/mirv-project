#ifndef MIRVExpression_hh
#define MIRVExpression_hh

#include <mirv/ir/node.hh>
#include <mirv/ir/property.hh>

#include <mirv/mem/heap.hh>

#include <mirv/util/debug.hh>

#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>

namespace MIRV {
   template<
      typename Op,
      typename Base = typename BaseType<Op>::type>
   class Expression : public Base {
   public:
      typedef Base base_type;
      typedef typename base_type::child_type child_type;
   };

   typedef Expression<Base> BaseExpression;

   typedef Inner<BaseExpression> InnerExpression;

   typedef Leaf<BaseExpression> LeafExpression;

   class Unary {
   private:
      typedef InnerExpression interface_base_type;

   public:
      class interface;
      typedef interface base_type;

      class interface
            : public interface_base_type {
      public:
         typedef BaseExpression child_type;
         typedef ptr<child_type>::type child_ptr;
         typedef ptr<child_type>::const_type const_child_ptr;

         void set_operand(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         };

         child_ptr get_operand(void) {
            return(front());
         };

         const_child_ptr get_operand(void) const {
            return(front());
         };
      };
   };

   /// Convention: First list element is left operand, second is right operand
   class Binary {
   private:
      typedef InnerExpression interface_base_type;

   public:
      class interface;
      typedef interface base_type;

      class interface
            : public interface_base_type {
      public:
         typedef BaseExpression child_type;
         typedef ptr<child_type>::type child_ptr;
         typedef ptr<child_type>::const_type const_child_ptr;

         void set_left_operand(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         };

         void set_right_operand(child_ptr c) {
            if (empty()) {
               push_back(child_ptr());  // Placeholder for left operand
               push_back(c);
            }
            else {
               *--end() = c;
            }
         };

         child_ptr get_left_operand(void) {
            check_invariant(!empty(),
                            "Attempt to get operand from empty expression");
            return(front());
         };

         const_child_ptr get_left_operand(void) const {
            check_invariant(!empty(),
                            "Attempt to get operand from empty expression");
            return(front());
         };

         child_ptr get_right_operand(void) {
            check_invariant(size() > 1 && back(),
                            "Attempt to get missing operand from expression");
            return(back());
         };

         const_child_ptr get_right_operand(void) const {
            check_invariant(size() > 1 && back(),
                         "Attempt to get missing operand from expression");
            return(back());
         };
      };
   };

   typedef Expression<Unary> UnaryExpression;
   typedef Expression<Binary> BinaryExpression;

   // Expression property semantics
   class Arithmetic {};
   class Logical {};
   class Bitwise {};
   class Commutative {};
   class NonCommutative {};

   class Associative {};
   class NonAssociative {};

   class Transitive {};
   class Intransitive {};

   class Reflexive {};
   class NonReflexive {};
 
   class Reference {};

   // A metafunction class to generate expression hierarchies.  This
   // makes sure that each property expression specifies Base as its
   // base type so that visit(Expreesion<>::base_type) works
   // correctly.
   class ExpressionGenerator {
   public:
      template<typename Property, typename Base>
      struct apply {
         typedef Expression<Property, Base> type;
      };
   };

   template<typename Sequence, typename Root>
   class ExpressionBaseGenerator {
   public:
      typedef typename Properties<ExpressionGenerator, Root, Sequence>::type
      type;

   };
}

#endif
