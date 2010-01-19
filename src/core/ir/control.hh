#ifndef mirv_core_ir_control_hh
#define mirv_core_ir_control_hh

#include <mirv/core/builder/make.hh>
#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/reference.hh>
#include <mirv/core/util/cast.hh>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for block/sequence statements.
   class Block {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef InnerStatement visitor_base_type;
     typedef sequence properties;

      typedef InnerStatement interface_base_type;

      class interface : public interface_base_type {
      public:
	 template<typename A1>
	 interface(A1 a1) : interface_base_type(a1)  {}
	 template<typename A1, typename A2>
	 interface(A1 a1, A2 a2) : interface_base_type(a1, a2)  {}
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

   /// This is a statement with only one child statement.  The child
   /// should be a block statement if more than one statement needs to
   /// be under this control point.
   class SingleBlock {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
     typedef InnerStatement interface_base_type;
     typedef InnerStatement visitor_base_type;
     typedef sequence properties;

      class interface : public interface_base_type {
      public:
	 template<typename A1>
	 // If this isn't a block statement, make it one.
	 interface(A1 a1) : 
	   interface_base_type(dyn_cast<Statement<Block> >(a1) ?
			       a1 : mirv::make<Statement<Block> >(a1))  {}

         typedef interface_base_type::child_ptr child_ptr;
         typedef interface_base_type::const_child_ptr const_child_ptr;
         
         void set_child_statement(child_ptr s) {
	   if (!dyn_cast<Statement<Block> >(s)) {
	     s = mirv::make<Statement<Block> >(s);
	   }
	   if (this->empty()) {
	     push_back(s);
	   }
	   else {
	     *--this->end() = s;
	   }
         }

	child_ptr get_child_statement(void) {
	  check_invariant(!this->empty(), 
			  "Attempt to get statement from empty block");
	  
	  return(this->front());
	}
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

  /// This is a statement with two child statements.  The children
  /// should be block statements if more than one statement needs to
  /// be under this control point.
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
	// If these are not blocks, make them so.
	template<typename A1, typename A2>
	interface(A1 a1, A2 a2) :
	  interface_base_type(dyn_cast<Statement<Block> >(a1)?
			      a1 : mirv::make<Statement<Block> >(a1),
			      dyn_cast<Statement<Block> >(a2) ?
			      a2 : mirv::make<Statement<Block> >(a2))  {}

         typedef interface_base_type::child_ptr child_ptr;
         typedef interface_base_type::const_child_ptr const_child_ptr;
         
	void set_left_child_statement(child_ptr s) {
	  if (!dyn_cast<Statement<Block> >(s)) {
	    s = mirv::make<Statement<Block> >(s);
	  }
	  if (this->empty()) {
	    push_back(s);
	  }
	  else {
	    *this->begin() = s;
	  }
	}

	void set_right_child_statement(child_ptr s) {
	  if (!dyn_cast<Statement<Block> >(s)) {
	    s = mirv::make<Statement<Block> >(s);
	  }
	  if (this->empty()) {
	    push_back(child_ptr());  // Placeholder for left operand
	    push_back(s);
	  }
	  else {
	    *--this->end() = s;
	  }
	}

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

  /// This is a statement with a single controlling condition
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
	 template<typename A1>
	 interface(A1 a1) : interface_base_type(a1)  {}

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

  /// Specify the if-then statement interface.
  class IfThen {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<SingleBlock> {
     public:
       template<typename S, typename E>
       Base(E e, S s) : Statement<SingleCondition>(e),
			Statement<SingleBlock>(s) {}

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
  
  /// Specify the if-then-else statement interface.
   class IfElse {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<DualBlock> {
     public:
       template<typename S1, typename S2, typename E>
       Base(E e, S1 s1, S2 s2) : Statement<SingleCondition>(e),
				 Statement<DualBlock>(s1, s2) {}

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

  /// Specify the while statement interface.
   class While {
   private:
      typedef boost::mpl::vector<Conditional, Iterative> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Base(E e, S1 s1) : Statement<SingleCondition>(e),
			  Statement<SingleBlock>(s1) {}

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

  /// Specify the do-while statement interface.
   class DoWhile {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Base(E e, S1 s1) : Statement<SingleCondition>(e),
			  Statement<SingleBlock>(s1) {}

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

  /// Specify the case statement interface.
   class Case {
   private:
      typedef boost::mpl::vector<> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Base(E e, S1 s1) : Statement<SingleCondition>(e),
			  Statement<SingleBlock>(s1) {}

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
   
  /// This is a list of case statements.  It is the statement type
  /// that appears under a switch statement.
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
       template<typename S1>
       interface(S1 s1) : Statement<SingleBlock>(s1) {}

         // Override base methods
         void push_back(child_ptr c) {
	   // check_invariant(safe_cast<Statement<Case> >(c),
           //                  "Attempt to insert non-case statement in case block");
            interface_base_type::push_back(c);
         };
      };

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };

  /// Specify the switch statement interface.
   class Switch {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleCondition>,
		  public Statement<CaseBlock> {
     public:
       template<typename S1, typename E>
       Base(E e, S1 s1) : Statement<SingleCondition>(e),
			  Statement<CaseBlock>(s1) {}

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

  /// This is a statement with a single label
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
	template<typename E>
	interface(E e) : Statement<SingleExpression>(e) {}

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

  /// A before statement specifies a block statement with an entry
  /// label.  A goto-dest statement may transfer control to this
  /// label.  This provides a structured way to return to the top of a
  /// deeply nested set of loops.
  class Before {
   private:
      typedef boost::mpl::vector<Iterative> sequence;  // Of a sort

     class Base : public Statement<SingleLabel>,
		  public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Base(E e, S1 s1) : Statement<SingleLabel>(e),
			  Statement<SingleBlock>(s1) {}

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


  /// An after statement specifies a block with a label at the end.  A
  /// goto-dest statement may transfer control to this label.  This
  /// provides a structured way to implement break- and continue-type
  /// statements.
   class After {
   private:
      typedef boost::mpl::vector<Conditional> sequence;  // Of a sort

     class Base : public Statement<SingleLabel>,
		  public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Base(E e, S1 s1) : Statement<SingleLabel>(e),
			  Statement<SingleBlock>(s1) {}

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

  /// This is an arbitrary goto statement.  It should not appear in
  /// normalized IR but is necessary to handle translation from
  /// hitgher level languages.  A filter will eliminate gotos and
  /// restructure the IR to be fully structured.
   class Goto {
   private:
      typedef boost::mpl::vector<Conditional> sequence;

     class Base : public Statement<SingleLabel>,
		    public LeafStatement {
     public:
       template<typename E>
       Base(E e) : Statement<SingleLabel>(e),
		   LeafStatement() {}

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

   /// A return does not return an expression.  Instead, assignment to
   /// a special location determines the return value.  This separates
   /// changes in data state from changes in control state.
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
