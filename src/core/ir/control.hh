#ifndef MIRVControl_hh
#define MIRVControl_hh

#include <mirv/statement.hh>

namespace MIRV {

   class Block {
   private:
      typdef boost::mpl::vector<> sequence;

   public:
      typedef InnerStatement::publicize<InnerStatement> interface_base_type;

      class interface : public interface_base_type {};

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with only one child statement.  The child should be
   // a block statement if more than one statement needs to be under
   // this control point.
   template<typename Base>
   class SingleBlock {
   public:
      typedef Base interface_base_type

      class interface : public interface_base_type {
      public:
         typedef interface_base_type::statement_ptr child_ptr;
         typedef interface_base_type::const_statement_ptr const_child_ptr;
         
         void set_statement(child_ptr s) {
            if (empty()) {
               push_back(s);
            }
            else {
               *--end() = s;
            }
         };

         child_ptr get_statement(void) {
            check_invariant(!empty(), 
                            "Attempt to get statement from empty block");
            
            return(front());
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with two child statements.  The children should be
   // block statements if more than one statement needs to be under
   // this control point.
   template<typename Base>
   class DualBlock {
   public:
      typedef Base interface_base_type

      class interface : public interface_base_type {
         // Protected because these are probably bad names for subclasses
      protected:
         typedef interface_base_type::statement_ptr child_ptr;
         
         void set_left_statement(child_ptr s) {
            if (empty()) {
               push_back(s);
            }
            else {
               *begin() = s;
            }
         };

         void set_right_statement(child_ptr s) {
            if (empty()) {
               push_back(child_ptr());  // Placeholder for left operand
               push_back(s);
            }
            else {
               *--end() = s;
            }
         };

         child_ptr get_left_statement(void) {      
            check_invariant(!empty(),
                            "Attempt to get statement from empty block");
            return(front());
         };
         
         const_child_ptr get_left_statement(void) const {
            check_invariant(!empty(),
                            "Attempt to get statement from empty block");
            return(front());
         };
         
         child_ptr get_right_statement(void) {      
            check_invariant(size() > 1 && back(),
                            "Attempt to get missing statement from block");
            return(back());
         };
         
         const_child_ptr get_right_statement(void) const {
            check_invariant(size() > 1 && back(),
                            "Attempt to get missing statement from block");
            return(back());
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // A statement with a single condition
   template<typename Stmt>
   class SingleCondition {
   public:
      typedef Statement<SingleExpression<Stmt> > interface_base_type;
      
      class interface 
            : public interface_base_type {
         typedef interface_base_type::expression_ptr expression_ptr;
         typedef interface_base_type::const_expression_ptr 
         const_expression_ptr;

      public:
         void set_condition(interface_base_type::expression_ptr e) {
            set_expression(e);
         };

         expression_ptr get_condition(void) {
            return(get_expression());
         };

         const_expression_ptr get_condition(void) const {
            return(get_expression());
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   // IfThen doesn't inherit from the base_type, Statement<IfThen>
   // does.  Therefore we need a way to make base_type's protected
   // methods available.  That's why interface is set as the base_type.
   class IfThen {
   private:
      typdef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleCondition<Statement<
         SingleBlock<InnerStatement> > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class IfElse {
   private:
      typdef boost::mpl::vector<Conditional> sequence;

      typedef PropertiesType<sequence>::type properties;
      typedef Statement<SingleCondition<Statement<
         DualBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class While {
   private:
      typdef boost::mpl::vector<Conditional, Iterative> sequence;

      typedef Statement<SingleCOndition<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class DoWhile {
   private:
      typdef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleCondition<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class Case {
   private:
      typdef boost::mpl::vector<> sequence;

      typedef Statement<SingleCondition<Statement<
         SingleBlock<InnerStatement> > > > base_type;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };
   
   typedef Statement<Case> CaseStatement;
   
   // A list of case statements
   class CaseBlock {
   private:
      typdef boost::mpl::vector<> sequence;

   public:
      typedef InnerStatement::publicize<InnerStatement> interface_base_type;

      class interface : public interface_base_type {
         typedef interface_base_type::child_ptr child_ptr;
      public:
         // Override base methods
         void push_back(child_ptr c) {
            check_invariant(safe_ptr_cast<CaseStatement>(c),
                            "Attempt to insert non-case statement in case block");
            interface_base_type::push_back(c);
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   class Switch {
   private:
      typdef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleCondition<Statement<CaseBlock> > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   // A statement with a single label
   template<typename Stmt>
   class SingleLabel {
   public:
      typedef Statement<SingleExpression<Stmt> > interface_base_type;
      
      class interface 
            : public interface_base_type {
         typedef interface_base_type::expression_ptr expression_ptr;
         typedef interface_base_type::const_expression_ptr 
         const_expression_ptr;

      public:
         void set_label(expression_ptr e) {
            check_invariant(safe_ptr_cast<LabelExpression>(e),
                            "Attempt to set non-label as label");
            
            set_expression(e);
         };

         expression_ptr get_label(void) {
            return(get_expression());
         };

         const_expression_ptr get_label(void) const {
            return(get_expression());
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   class Before {
   private:
      typdef boost::mpl::vector<Iterative> sequence;  // Of a sort

      typedef Statement<SingleLabel<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class After {
   private:
      typdef boost::mpl::vector<Conditional> sequence;  // Of a sort

      typedef Statement<SingleLabel<Statement<
         SingleBlock<InnerStatement> > > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class Goto {
   private:
      typdef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleLabel<LeafStatement> > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   class Label {
   private:
      typdef boost::mpl::vector<Conditional> sequence;

      typedef Statement<SingleLabel<LeafStatement> > > root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

   /// Does not return an expression, use explicit assignment
   class Return {
   private:
      typdef boost::mpl::vector<Conditional> sequence;

      typedef LeafStatement root;

   public:
      typedef StatementBaseGenerator<sequence, root>::type base_type;
   };

};

#endif
