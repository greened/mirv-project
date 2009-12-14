#ifndef mirv_core_ir_expression_hh
#define mirv_core_ir_expression_hh

#include <mirv/core/ir/node.hh>
#include <mirv/core/ir/property.hh>

#include <mirv/core/mem/heap.hh>

#include <mirv/core/util/debug.hh>

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

    private:
      template<typename A1>
      Expression(A1 a1) : base_type(a1) {}
      template<typename A1, typename A2>
      Expression(A1 a1, A2 a2) : base_type(a1, a2) {}

    public:
     template<typename A1>
     static typename ptr<Expression<Op> >::type
     make(A1 a1) {
       return typename ptr<Expression<Op> >::type(new Expression<Op>(a1));
     }

     template<typename A1, typename A2>
     static typename ptr<Expression<Op> >::type
     make(A1 a1, A2 a2) {
       return typename ptr<Expression<Op> >::type(new Expression<Op>(a1, a2));
     }

     virtual void accept(ExpressionVisitor &V);
   };

    template<>
    class Expression<Base> : public Node<Base> {
    public:
     virtual void accept(ExpressionVisitor &V);
   };

   namespace detail {
     class InnerExpressionTraits {
     public:
       typedef Expression<Base> Child;
       typedef Expression<Base> base_type;

     private:
       typedef ptr<Child>::type child_ptr;
       typedef std::list<child_ptr> child_list;

     public:
       typedef child_list::iterator iterator;
       typedef child_list::reverse_iterator reverse_iterator;
       typedef child_list::const_iterator const_iterator;
       typedef child_list::const_reverse_iterator const_reverse_iterator;

       typedef child_list::size_type size_type;
     };
   }

    template<>
    class Expression<Inner<detail::InnerExpressionTraits> > : public Inner<detail::InnerExpressionTraits>::base_type {
    public:
     virtual void accept(ExpressionVisitor &V);
   };

   class InnerExpression : public InnerImpl<Expression<Base>, Inherit1<ExpressionVisitor>::apply<Virtual<Expression<Inner<detail::InnerExpressionTraits> > > >::type> {
   private:
     typedef InnerImpl<
       Expression<Base>,
      Inherit1<ExpressionVisitor>::apply<
	Virtual<Expression<Inner<detail::InnerExpressionTraits> > >
      >::type> BaseType;

  public:
    InnerExpression(child_ptr Child) : BaseType(Child) {}
    InnerExpression(child_ptr Child1,
		    child_ptr Child2) : BaseType(Child1, Child2) {}
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

	interface(child_ptr Child1,
		  child_ptr Child2) : interface_base_type(Child1, Child2) {}

         void set_left_operand(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         }

         void set_right_operand(child_ptr c) {
            if (empty()) {
               push_back(child_ptr());  // Placeholder for left operand
               push_back(c);
            }
            else {
               *--end() = c;
            }
         }

         child_ptr get_left_operand(void) {
            check_invariant(!empty(),
                            "Attempt to get operand from empty expression");
            return(front());
         }

         const_child_ptr get_left_operand(void) const {
            check_invariant(!empty(),
                            "Attempt to get operand from empty expression");
            return(front());
         }

         child_ptr get_right_operand(void) {
            check_invariant(size() > 1 && back(),
                            "Attempt to get missing operand from expression");
            return(back());
         }

         const_child_ptr get_right_operand(void) const {
            check_invariant(size() > 1 && back(),
                         "Attempt to get missing operand from expression");
            return(back());
         }
	virtual void accept(ExpressionVisitor &V);
      };
   };

   // Expression property semantics
   class Arithmetic {
   public:
     typedef boost::mpl::int_<0> order;
     typedef Virtual<InnerExpression> base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };
   class Logical {
   public:
     typedef boost::mpl::int_<1> order;
     typedef Virtual<InnerExpression> base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };
   class Bitwise {
   public:
     typedef boost::mpl::int_<2> order;
     typedef Virtual<InnerExpression> base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };
   class Ref {
   public:
     typedef boost::mpl::int_<11> order;
     typedef Virtual<InnerExpression> base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

   class Commutative {
   public:
     typedef boost::mpl::int_<3> order;
     typedef Virtual<InnerExpression> base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };
   class Associative {
   public:
     typedef boost::mpl::int_<5> order;
     typedef Virtual<InnerExpression> base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };
   class Transitive {
   public:
     typedef boost::mpl::int_<7> order;
     typedef Virtual<InnerExpression> base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };
   class Reflexive {
   public:
     typedef boost::mpl::int_<9> order;
     typedef Virtual<InnerExpression> base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;
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
				Inherit2<ExpressionVisitor> >::type
      type;

   };
}
 
#endif
