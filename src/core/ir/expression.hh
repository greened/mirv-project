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
   namespace detail {
     // Provide an ordering for properties.
     template<typename T1, typename T2>
     struct ExpressionPropertyLess :
       public boost::mpl::less<typename T1::order, typename T2::order> {};
   }

   struct ExpressionVisitor;
    template<typename Op>
    class Expression : public Op::base_type {
    public:
      typedef typename Op::base_type base_type;
      typedef typename Op::visitor_base_type visitor_base_type;
      typedef typename boost::mpl::sort<
	typename Op::properties,
	detail::ExpressionPropertyLess<boost::mpl::_1, boost::mpl::_2>
	>::type properties;
     virtual void accept(ExpressionVisitor &V);
   };

  class InnerExpression : public InnerImpl<Expression<Base>, Expression<Base> > {
  public:
    virtual void accept(ExpressionVisitor &V);
  };

  class LeafExpression : public LeafImpl<Expression<Base> > {
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
	typedef Expression<Base> child_type;
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
	typedef Expression<Base> child_type;
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

   // Expression property semantics
   class Arithmetic {
     typedef boost::mpl::int_<0> order;
   public:
     typedef Virtual<InnerExpression> base_type;
   };
   class Logical {
     typedef boost::mpl::int_<1> order;
   public:
     typedef Virtual<InnerExpression> base_type;
   };
   class Bitwise {
     typedef boost::mpl::int_<2> order;
   public:
     typedef Virtual<InnerExpression> base_type;
   };
   class Reference {
     typedef boost::mpl::int_<11> order;
   public:
     typedef Virtual<InnerExpression> base_type;
   };

   class Commutative {
     typedef boost::mpl::int_<3> order;
   public:
     typedef Virtual<InnerExpression> base_type;
   };
   class Associative {
     typedef boost::mpl::int_<5> order;
   public:
     typedef Virtual<InnerExpression> base_type;
   };
   class Transitive {
     typedef boost::mpl::int_<7> order;
   public:
     typedef Virtual<InnerExpression> base_type;
   };
   class Reflexive {
     typedef boost::mpl::int_<9> order;
   public:
     typedef Virtual<InnerExpression> base_type;
   }; 

  struct PropertyExpressionGenerator {
    template<typename Property>
    struct apply {
      typedef Expression<Property> type;
    };
  };
 
  template<typename Sequence, typename Root>
   class ExpressionBaseGenerator {
   public:
    typedef typename Properties<PropertyExpressionGenerator, Root, Sequence,
				Inherit<ExpressionVisitor> >::type
      type;

   };
}
 
#endif
