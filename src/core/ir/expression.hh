#ifndef mirv_core_ir_expression_hh
#define mirv_core_ir_expression_hh

#include <mirv/ir/node.hh>
#include <mirv/ir/property.hh>

#include <mirv/mem/heap.hh>

#include <mirv/util/debug.hh>

#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>

namespace mirv {
  template<typename R>
  struct ExpressionVisitor;

   template<
      typename Op,
      typename Base = typename BaseType<Op>::type>
   class Expression : Base {
   public:
      typedef Base base_type;
      typedef typename base_type::child_type child_type;
     virtual void accept(ExpressionVisitor<void> &V);
   };

   typedef Expression<Base> BaseExpression;

  class InnerExpression : public InnerImpl<BaseExpression, BaseExpression> {
  public:
    virtual void accept(ExpressionVisitor<void> &V);
  };

  class LeafExpression : public LeafImpl<BaseExpression> {
  public:
    virtual void accept(ExpressionVisitor<void> &V);
  };

   class Unary {
   private:
      typedef InnerExpression interface_base_type;

   public:
      class interface;
      typedef interface base_type;

      class interface
            : public virtual interface_base_type {
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
	virtual void accept(ExpressionVisitor<void> &V);
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
            : public virtual interface_base_type {
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
	virtual void accept(ExpressionVisitor<void> &V);
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

  template<typename Property>
  class Expression<Property, void> : public virtual InnerExpression {};

  struct PropertyExpressionGenerator {
    template<typename Property>
    struct apply {
      typedef Expression<Property, void> type;
    };
  };
 
  template<typename Sequence, typename Root>
   class ExpressionBaseGenerator {
   public:
    typedef typename Properties<PropertyExpressionGenerator, Root, Sequence>::type
      type;

   };
}
 
#endif
