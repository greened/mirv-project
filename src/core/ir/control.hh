#ifndef mirv_core_ir_control_hh
#define mirv_core_ir_control_hh

#include <mirv/ir/statement.hh>
#include <mirv/ir/reference.hh>
#include <mirv/util/cast.hh>

#include <boost/mpl/vector.hpp>

namespace mirv {
   class Block {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef InnerStatement visitor_base_type;
     typedef sequence properties;

      typedef InnerStatement::publicize<InnerStatement> interface_base_type;

      class interface : public interface_base_type {};

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with only one child statement.  The child should be
   // a block statement if more than one statement needs to be under
   // this control point.
   template<typename Base>
   class SingleBlock {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef Base interface_base_type;

      class interface : public interface_base_type {
      public:
         typedef typename interface_base_type::child_ptr child_ptr;
         typedef typename interface_base_type::const_child_ptr const_child_ptr;
         
         void set_statement(child_ptr s) {
            if (this->empty()) {
               push_back(s);
            }
            else {
               *--this->end() = s;
            }
         };

         child_ptr get_statement(void) {
            check_invariant(!this->empty(), 
                            "Attempt to get statement from empty block");
            
            return(this->front());
         };
      };

      typedef typename StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with two child statements.  The children should be
   // block statements if more than one statement needs to be under
   // this control point.
   template<typename Base>
   class DualBlock {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef Base interface_base_type;

      class interface : public interface_base_type {
         // Protected because these are probably bad names for subclasses
      protected:
         typedef typename interface_base_type::child_ptr child_ptr;
         typedef typename interface_base_type::const_child_ptr const_child_ptr;
         
         void set_left_statement(child_ptr s) {
            if (this->empty()) {
               push_back(s);
            }
            else {
               *this->begin() = s;
            }
         };

         void set_right_statement(child_ptr s) {
            if (this->empty()) {
               push_back(child_ptr());  // Placeholder for left operand
               push_back(s);
            }
            else {
               *--this->end() = s;
            }
         };

         child_ptr get_left_statement(void) {      
            check_invariant(!this->empty(),
                            "Attempt to get statement from empty block");
            return(this->front());
         };
         
         const_child_ptr get_left_statement(void) const {
            check_invariant(!this->empty(),
                            "Attempt to get statement from empty block");
            return(this->front());
         };
         
         child_ptr get_right_statement(void) {      
            check_invariant(this->size() > 1 && this->back(),
                            "Attempt to get missing statement from block");
            return(this->back());
         };
         
         const_child_ptr get_right_statement(void) const {
            check_invariant(this->size() > 1 && this->back(),
                            "Attempt to get missing statement from block");
            return(this->back());
         };
      };

      typedef typename StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with a single condition
   template<typename Stmt>
   class SingleCondition {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef Statement<SingleExpression<Stmt> > interface_base_type;
      
      class interface 
            : public interface_base_type {
         typedef typename interface_base_type::expression_ptr expression_ptr;
         typedef typename interface_base_type::const_expression_ptr 
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

      typedef typename StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // IfThen doesn't inherit from the base_type, Statement<IfThen>
   // does.  Therefore we need a way to make base_type's protected
   // methods available.  That's why interface is set as the base_type.
   class IfThen {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleCondition<Statement<
			SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class IfElse {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleCondition<Statement<
         DualBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class While {
   private:
      typedef boost::mpl::vector<Conditional, Iterative> sequence;

      typedef Statement<SingleCondition<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class DoWhile {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleCondition<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class Case {
   private:
      typedef boost::mpl::vector<> sequence;

      typedef Statement<SingleCondition<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };
   
   // A list of case statements
   class CaseBlock {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef InnerStatement::publicize<InnerStatement> interface_base_type;

      class interface : public interface_base_type {
         typedef interface_base_type::child_ptr child_ptr;
      public:
         // Override base methods
         void push_back(child_ptr c) {
 	    check_invariant(safe_ptr_cast<Statement<Case> *>(c),
                            "Attempt to insert non-case statement in case block");
            interface_base_type::push_back(c);
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   class Switch {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleCondition<Statement<CaseBlock> > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   // A statement with a single label
   template<typename Stmt>
   class SingleLabel {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef Statement<SingleExpression<Stmt> > interface_base_type;
      
      class interface 
            : public interface_base_type {
         typedef typename interface_base_type::expression_ptr expression_ptr;
         typedef typename interface_base_type::const_expression_ptr 
         const_expression_ptr;

      public:
         void set_label(expression_ptr e) {
 	    check_invariant(safe_ptr_cast<Expression<Label> *>(e),
                            "Attempt to set non-label as label");
            
            set_expression(e);
         };

         expression_ptr get_label(void) {
            return(this->get_expression());
         };

         const_expression_ptr get_label(void) const {
            return(this->get_expression());
         };
      };

      typedef typename StatementBaseGenerator<sequence, interface>::type base_type;
   };

   class Before {
   private:
      typedef boost::mpl::vector<Iterative> sequence;  // Of a sort

      typedef Statement<SingleLabel<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class After {
   private:
      typedef boost::mpl::vector<Conditional> sequence;  // Of a sort

      typedef Statement<SingleLabel<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class Goto {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleLabel<LeafStatement> > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   /// Does not return an expression, use explicit assignment
   class Return {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

      typedef LeafStatement root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

};

#endif
