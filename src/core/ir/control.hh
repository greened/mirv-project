#ifndef mirv_core_ir_control_hh
#define mirv_core_ir_control_hh

#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/reference.hh>
#include <mirv/core/util/cast.hh>

#include <boost/mpl/vector.hpp>

namespace mirv {
   class Block {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef InnerStatement visitor_base_type;
     typedef sequence properties;

      typedef InnerStatement interface_base_type;

      class interface : public virtual interface_base_type {};

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with only one child statement.  The child should be
   // a block statement if more than one statement needs to be under
   // this control point.
   class SingleBlock {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef InnerStatement interface_base_type;
     typedef InnerStatement visitor_base_type;
     typedef sequence properties;

      class interface : public interface_base_type {
      public:
         typedef interface_base_type::child_ptr child_ptr;
         typedef interface_base_type::const_child_ptr const_child_ptr;
         
         void set_child_statement(child_ptr s) {
            if (this->empty()) {
               push_back(s);
            }
            else {
               *--this->end() = s;
             }
         };

         child_ptr get_child_statement(void) {
            check_invariant(!this->empty(), 
                            "Attempt to get statement from empty block");
            
            return(this->front());
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with two child statements.  The children should be
   // block statements if more than one statement needs to be under
   // this control point.
   class DualBlock {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef InnerStatement interface_base_type;
     typedef InnerStatement visitor_base_type;
     typedef sequence properties;

      class interface : public interface_base_type {
         // Protected because these are probably bad names for subclasses
      protected:
         typedef interface_base_type::child_ptr child_ptr;
         typedef interface_base_type::const_child_ptr const_child_ptr;
         
         void set_left_child_statement(child_ptr s) {
            if (this->empty()) {
               push_back(s);
            }
            else {
               *this->begin() = s;
            }
         };

         void set_right_child_statement(child_ptr s) {
            if (this->empty()) {
               push_back(child_ptr());  // Placeholder for left operand
               push_back(s);
            }
            else {
               *--this->end() = s;
            }
         };

         child_ptr get_left_child_statement(void) {      
            check_invariant(!this->empty(),
                            "Attempt to get statement from empty block");
            return(this->front());
         };
         
         const_child_ptr get_left_child_statement(void) const {
            check_invariant(!this->empty(),
                            "Attempt to get statement from empty block");
            return(this->front());
         };
         
         child_ptr get_right_child_statement(void) {      
            check_invariant(this->size() > 1 && this->back(),
                            "Attempt to get missing statement from block");
            return(this->back());
         };
         
         const_child_ptr get_right_child_statement(void) const {
            check_invariant(this->size() > 1 && this->back(),
                            "Attempt to get missing statement from block");
            return(this->back());
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with a single condition
   class SingleCondition {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef Statement<SingleExpression> interface_base_type;
     typedef Statement<SingleExpression> visitor_base_type;
     typedef sequence properties;
      
      class interface 
            : public interface_base_type {
         typedef interface_base_type::expression_ptr expression_ptr;
         typedef interface_base_type::const_expression_ptr 
         const_expression_ptr;

      public:
         void set_condition(expression_ptr e) {
            set_expression(e);
         };

         expression_ptr get_condition(void) {
            return(this->get_expression());
         };

         const_expression_ptr get_condition(void) const {
            return(this->get_expression());
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // IfThen doesn't inherit from the base_type, Statement<IfThen>
   // does.  Therefore we need a way to make base_type's protected
   // methods available.  That's why interface is set as the base_type.
  class IfThen {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<SingleBlock> {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("IfThen::Base::accept called");
       }
     };
     typedef Base root;

  public:
    typedef sequence properties;
    typedef StatementBaseGenerator<sequence, root>::type base_type;
    typedef Statement<SingleBlock> visitor_base_type;
  };
  
   class IfElse {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<DualBlock> {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("IfElse::Base::accept called");
       }
     };
     typedef Base root;

   public:
     typedef Statement<DualBlock> visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class While {
   private:
      typedef boost::mpl::vector<Conditional, Iterative> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<SingleBlock> {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("While::Base::accept called");
       }
     };
     typedef Base root;

   public:
     typedef Statement<SingleBlock> visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class DoWhile {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<SingleBlock> {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("DoWhile::Base::accept called");
       }
     };
     typedef Base root;

   public:
     typedef Statement<SingleBlock> visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class Case {
   private:
      typedef boost::mpl::vector<> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<SingleBlock> {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("Case::Base::accept called");
       }
     };
     typedef Base root;

   public:
     typedef Statement<SingleBlock> visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };
   
   // A list of case statements
   class CaseBlock {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef Statement<SingleBlock> visitor_base_type;
     typedef Statement<SingleBlock> interface_base_type;
     typedef sequence properties;

      class interface : public virtual interface_base_type {
         typedef interface_base_type::child_ptr child_ptr;
      public:
         // Override base methods
         void push_back(child_ptr c) {
	   // check_invariant(safe_cast<Statement<Case> >(c),
           //                  "Attempt to insert non-case statement in case block");
            interface_base_type::push_back(c);
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   class Switch {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<CaseBlock> {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("Switch::Base::accept called");
       }
     };
     typedef Base root;

   public:
     typedef Statement<CaseBlock> visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   // A statement with a single label
   class SingleLabel {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef Statement<SingleExpression> visitor_base_type;
     typedef sequence properties;
      typedef Statement<SingleExpression> interface_base_type;
      
      class interface 
            : public interface_base_type {
         typedef interface_base_type::expression_ptr expression_ptr;
         typedef interface_base_type::const_expression_ptr 
         const_expression_ptr;

      public:
         void set_label(expression_ptr e) {
	   // check_invariant(safe_cast<Expression<Label> >(e),
           //                  "Attempt to set non-label as label");
            
            set_expression(e);
         };

         expression_ptr get_label(void) {
            return(this->get_expression());
         };

         const_expression_ptr get_label(void) const {
            return(this->get_expression());
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   class Before {
   private:
      typedef boost::mpl::vector<Iterative> sequence;  // Of a sort

     class Base : public Statement<SingleLabel>,
		  public Statement<SingleBlock> {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("Before::Base::accept called");
       }
     };
     typedef Base root;

   public:
     typedef Statement<SingleBlock> visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };


   class After {
   private:
      typedef boost::mpl::vector<Conditional> sequence;  // Of a sort

     class Base : public Statement<SingleLabel>,
		  public Statement<SingleBlock> {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("After::Base::accept called");
       }
     };
     typedef Base root;

   public:
     typedef Statement<SingleBlock> visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class Goto {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleLabel>,
		    public LeafStatement {
     public:
       virtual void accept(mirv::StatementVisitor &) {
	 error("Goto::Base::accept called");
       }
     };
     typedef Base root;

   public:
     typedef LeafStatement visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   /// Does not return an expression, use explicit assignment
   class Return {
   private:
      typedef boost::mpl::vector<> sequence;

      typedef LeafStatement root;

   public:
     typedef root visitor_base_type;
     typedef sequence properties;
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };
};

#endif
