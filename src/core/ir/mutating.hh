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
         // Protected because subclasses might want to rename
      protected:
         typedef interface_base_type::expression_ptr expression_ptr;
         typedef interface_base_type::const_expression_ptr 
         const_expression_ptr;
         
         void set_left(expression_ptr e) {
            if (expression_empty()) {
               expression_push_back(e);
            }
            else {
               *expression_begin() = e;
            }
         };

         void set_right(expression_ptr e) {
            if (expression_empty()) {
               expression_push_back(expression_ptr()); // Placeholder
               expression_push_back(e);
            }
            else {
               *--expression_end() = e;
            }
         };
         
         expression_ptr get_left(void) {
            check_invariant(!expression_empty() && *expression_begin(),
                            "Attempt to get non-existent expression");
            return(expression_front());
         };

         const_expression_ptr get_left(void) const {
            check_invariant(!expression_empty() && *expression_begin(),
                            "Attempt to get non-existent expression");
            return(expression_front());
         };

         expression_ptr get_right(void) {
            check_invariant(expression_size() > 1 && expression_back(),
                            "Attempt to get non-existent expression");
            return(expression_back());
         };

         const_expression_ptr get_right(void) const {
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
		  public LeafStatement {};
     typedef Base root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };
}

#endif
