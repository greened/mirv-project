#ifndef mirv_core_ir_mutating_hh
#define mirv_core_ir_mutating_hh

#include <mirv/core/ir/statement.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
   class DualExpression {
   public:
      class interface;
      typedef interface base_type;
      typedef Statement<Controlled> interface_base_type;
     typedef boost::mpl::vector<> properties;
     typedef Statement<Controlled> visitor_base_type;

      class interface 
            : public interface_base_type {
      public:
	template<typename E1, typename E2>
	interface(E1 e1, E2 e2) : Statement<Controlled>(e1, e2) {}

         typedef interface_base_type::expression_ptr expression_ptr;
         typedef interface_base_type::const_expression_ptr 
         const_expression_ptr;
         
         void set_left_expression(expression_ptr e) {
            if (expression_empty()) {
               expression_push_back(e);
            }
            else {
               *expression_begin() = e;
            }
         };

         void set_right_expression(expression_ptr e) {
            if (expression_empty()) {
               expression_push_back(expression_ptr()); // Placeholder
               expression_push_back(e);
            }
            else {
               *--expression_end() = e;
            }
         };
         
         expression_ptr get_left_expression(void) {
            check_invariant(!expression_empty() && *expression_begin(),
                            "Attempt to get non-existent expression");
            return(expression_front());
         };

         const_expression_ptr get_left_expression(void) const {
            check_invariant(!expression_empty() && *expression_begin(),
                            "Attempt to get non-existent expression");
            return(expression_front());
         };

         expression_ptr get_right_expression(void) {
            check_invariant(expression_size() > 1 && expression_back(),
                            "Attempt to get non-existent expression");
            return(expression_back());
         };

         const_expression_ptr get_right_expression(void) const {
            check_invariant(expression_size() > 1 && expression_back(),
                            "Attempt to get non-existent expression");
            return(expression_back());
         };
      };
   };

   class Assignment {
   private:
      typedef boost::mpl::vector<Mutating> sequence;

     class Base : public Statement<DualExpression>,
		  public LeafStatement {
     public:
       template<typename E1, typename E2>
       Base(E1 e1, E2 e2) : Statement<DualExpression>(e1, e2),
			    LeafStatement() {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("Assignment::Base::accept called");
       }
     };
     typedef Base root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
     typedef sequence properties;
     typedef Statement<DualExpression> visitor_base_type;
   };
}

#endif
