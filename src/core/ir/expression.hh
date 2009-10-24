#ifndef mirv_core_ir_expression_hh
#define mirv_core_ir_expression_hh

#include <mirv/ir/node.hh>
#include <mirv/ir/property.hh>

#include <mirv/mem/heap.hh>

#include <mirv/util/debug.hh>

#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/int.hpp>

namespace mirv {
  struct ExpressionVisitor;

   // Expression property semantics
   class Arithmetic { typedef boost::mpl::int_<0> order; };
   class Logical { typedef boost::mpl::int_<1> order; };
   class Bitwise { typedef boost::mpl::int_<2> order; };
   class Commutative { typedef boost::mpl::int_<3> order; };
   class NonCommutative { typedef boost::mpl::int_<4> order; };

   class Associative { typedef boost::mpl::int_<5> order; };
   class NonAssociative { typedef boost::mpl::int_<6> order; };

   class Transitive { typedef boost::mpl::int_<7> order; };
   class Intransitive { typedef boost::mpl::int_<8> order; };

   class Reflexive { typedef boost::mpl::int_<9> order; };
   class NonReflexive { typedef boost::mpl::int_<10> order; };
 
   class Reference { typedef boost::mpl::int_<11> order; };

  namespace detail {
    // Provide an ordering for properties.
    template<typename T1, typename T2>
    struct ExpressionPropertyLess :
      public boost::mpl::less<typename T1::order, typename T2::order> {};
  }

   template<
      typename Op,
      typename Base = typename BaseType<Op>::type>
   class Expression : Base {
   public:
     typedef Base base_type;
     typedef typename Op::visitor_base_type visitor_base_type;
     typedef typename boost::mpl::sort<
       typename Op::properties,
       detail::ExpressionPropertyLess<boost::mpl::_1, boost::mpl::_2>
       >::type properties;
     virtual void accept(ExpressionVisitor &V);
   };

   typedef Expression<Base> BaseExpression;

  class InnerExpression : public InnerImpl<BaseExpression, BaseExpression> {
  public:
    virtual void accept(ExpressionVisitor &V);
  };

  class LeafExpression : public LeafImpl<BaseExpression> {
  public:
    virtual void accept(ExpressionVisitor &V);
  };

   class Unary {
   private:
      typedef InnerExpression interface_base_type;

   public:
      class interface;
      typedef interface base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;

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
	virtual void accept(ExpressionVisitor &V);
      };
   };

   /// Convention: First list element is left operand, second is right operand
   class Binary {
   private:
      typedef InnerExpression interface_base_type;

   public:
      class interface;
      typedef interface base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;

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
	virtual void accept(ExpressionVisitor &V);
      };
   };

   typedef Expression<Unary> UnaryExpression;
   typedef Expression<Binary> BinaryExpression;

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
