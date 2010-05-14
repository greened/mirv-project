#ifndef mirv_Core_IR_Control_hpp
#define mirv_Core_IR_Control_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the Interface for block/sequence statements.
   class Block {
   private:
      typedef InnerStatement InterfaceBaseType;

      class Interface : public InterfaceBaseType {
      public:
	Interface(void) : InterfaceBaseType() {}
	 template<typename A1>
	 Interface(A1 a1) : InterfaceBaseType(a1)  {}
	 template<typename A1, typename A2>
	 Interface(A1 a1, A2 a2) : InterfaceBaseType(a1, a2)  {}
      };

   public:
     typedef boost::mpl::vector<> Properties;
     typedef InnerStatement VisitorBaseType;

     typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

   /// This is a statement with only one child statement.  The child
   /// should be a block statement if more than one statement needs to
   /// be under this control point.
   class SingleBlock {
   private:
     typedef InnerStatement InterfaceBaseType;

      class Interface : public InterfaceBaseType {
      public:
	 template<typename A1>
	 // If this isn't a block statement, make it one.
	 Interface(A1 a1) : 
	   InterfaceBaseType(dyn_cast<Statement<Block> >(a1) ?
			       a1 : mirv::make<Statement<Block> >(a1))  {}
 
         typedef InterfaceBaseType::ChildPtr ChildPtr;
         typedef InterfaceBaseType::ConstChildPtr ConstChildPtr;
         
         void setChildStatement(ChildPtr s) {
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

	ChildPtr getChildStatement(void) {
	  checkInvariant(!this->empty(), 
			  "Attempt to get statement from empty block");
	  
	  return(this->front());
	}
      };

   public:
      typedef boost::mpl::vector<> Properties;
     typedef InnerStatement VisitorBaseType;
     typedef Properties properties;

      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// This is a statement with two child statements.  The children
  /// should be block statements if more than one statement needs to
  /// be under this control point.
   class DualBlock {
   private:
     typedef InnerStatement InterfaceBaseType;

      class Interface : public InterfaceBaseType {
         // Protected because these are probably bad names for subclasses
      protected:
	// If these are not blocks, make them so.
	template<typename A1, typename A2>
	Interface(A1 a1, A2 a2) :
	  InterfaceBaseType(dyn_cast<Statement<Block> >(a1)?
			      a1 : mirv::make<Statement<Block> >(a1),
			      dyn_cast<Statement<Block> >(a2) ?
			      a2 : mirv::make<Statement<Block> >(a2))  {}

         typedef InterfaceBaseType::ChildPtr ChildPtr;
         typedef InterfaceBaseType::ConstChildPtr ConstChildPtr;
         
	void setLeftChildStatement(ChildPtr s) {
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

	void setRightChildStatement(ChildPtr s) {
	  if (!dyn_cast<Statement<Block> >(s)) {
	    s = mirv::make<Statement<Block> >(s);
	  }
	  if (this->empty()) {
	    push_back(ChildPtr());  // Placeholder for left operand
	    push_back(s);
	  }
	  else {
	    *--this->end() = s;
	  }
	}

         ChildPtr getLeftChildStatement(void) {      
            checkInvariant(!this->empty(),
                            "Attempt to get statement from empty block");
            return(this->front());
         };
         
         ConstChildPtr getLeftChildStatement(void) const {
            checkInvariant(!this->empty(),
                            "Attempt to get statement from empty block");
            return(this->front());
         };
         
         ChildPtr getRightChildStatement(void) {      
            checkInvariant(this->size() > 1 && this->back(),
                            "Attempt to get missing statement from block");
            return(this->back());
         };
         
         ConstChildPtr getRightChildStatement(void) const {
            checkInvariant(this->size() > 1 && this->back(),
                            "Attempt to get missing statement from block");
            return(this->back());
         };
      };

   public:
      typedef boost::mpl::vector<> Properties;
     typedef InnerStatement VisitorBaseType;

      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// This is a statement with a single controlling condition
   class SingleCondition {
   private:
      typedef Statement<SingleExpression> InterfaceBaseType;

      class Interface 
            : public InterfaceBaseType {
         typedef InterfaceBaseType::ExpressionPtr ExpressionPtr;
         typedef InterfaceBaseType::ConstExpressionPtr 
         ConstExpressionPtr;

      public:
	 template<typename A1>
	 Interface(A1 a1) : InterfaceBaseType(a1)  {}

         void setCondition(ExpressionPtr e) {
            setExpression(e);
         };

         ExpressionPtr getCondition(void) {
            return(this->getExpression());
         };

         ConstExpressionPtr getCondition(void) const {
            return(this->getExpression());
         };
      };

   public:
      typedef boost::mpl::vector<> Properties;
     typedef Statement<SingleExpression> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// Specify the if-then statement Interface.
  class IfThen {
   private:
     class Interface : public Statement<SingleCondition>,
		       public Statement<SingleBlock> {
     public:
       template<typename S, typename E>
       Interface(E e, S s) : Statement<SingleCondition>(e),
			     Statement<SingleBlock>(s) {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("IfThen::Base::accept called");
       }
     };

  public:
    typedef boost::mpl::vector<Conditional> Properties;
    typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
    typedef Statement<SingleBlock> VisitorBaseType;
  };
  
  /// Specify the if-then-else statement Interface.
   class IfElse {
   private:
     class Interface : public Statement<SingleCondition>,
		       public Statement<DualBlock> {
     public:
       template<typename S1, typename S2, typename E>
       Interface(E e, S1 s1, S2 s2) : Statement<SingleCondition>(e),
				      Statement<DualBlock>(s1, s2) {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("IfElse::Base::accept called");
       }
     };

   public:
      typedef boost::mpl::vector<Conditional> Properties;
     typedef Statement<DualBlock> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// Specify the while statement Interface.
   class While {
   private:
     class Interface : public Statement<SingleCondition>,
		       public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Interface(E e, S1 s1) : Statement<SingleCondition>(e),
			       Statement<SingleBlock>(s1) {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("While::Base::accept called");
       }
     };

   public:
      typedef boost::mpl::vector<Conditional, Iterative> Properties;
     typedef Statement<SingleBlock> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// Specify the do-while statement Interface.
   class DoWhile {
   private:
     class Interface : public Statement<SingleCondition>,
		       public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Interface(E e, S1 s1) : Statement<SingleCondition>(e),
			       Statement<SingleBlock>(s1) {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("DoWhile::Base::accept called");
       }
     };

   public:
      typedef boost::mpl::vector<Conditional> Properties;
     typedef Statement<SingleBlock> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// Specify the case statement Interface.
   class Case {
   private:
     class Interface : public Statement<SingleCondition>,
		       public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Interface(E e, S1 s1) : Statement<SingleCondition>(e),
			       Statement<SingleBlock>(s1) {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("Case::Base::accept called");
       }
     };

   public:
      typedef boost::mpl::vector<> Properties;
     typedef Statement<SingleBlock> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };
   
  /// This is a list of case statements.  It is the statement type
  /// that appears under a switch statement.
   class CaseBlock {
   private:
     typedef Statement<SingleBlock> InterfaceBaseType;

      class Interface : public virtual InterfaceBaseType {
         typedef InterfaceBaseType::ChildPtr ChildPtr;
      public:
       template<typename S1>
       Interface(S1 s1) : Statement<SingleBlock>(s1) {}

         // Override base methods
         void push_back(ChildPtr c) {
	   // check_invariant(safe_cast<Statement<Case> >(c),
           //                  "Attempt to insert non-case statement in case block");
            InterfaceBaseType::push_back(c);
         };
      };

   public:
      typedef boost::mpl::vector<> Properties;
     typedef Statement<SingleBlock> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// Specify the switch statement Interface.
   class Switch {
   private:
     class Interface : public Statement<SingleCondition>,
		       public Statement<CaseBlock> {
     public:
       template<typename S1, typename E>
       Interface(E e, S1 s1) : Statement<SingleCondition>(e),
			       Statement<CaseBlock>(s1) {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("Switch::Base::accept called");
       }
     };

   public:
      typedef boost::mpl::vector<Conditional> Properties;
     typedef Statement<CaseBlock> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// This is a statement with a single label
   class SingleLabel {
   private:
      typedef Statement<SingleExpression> InterfaceBaseType;

      class Interface 
            : public InterfaceBaseType {
         typedef InterfaceBaseType::ExpressionPtr ExpressionPtr;
         typedef InterfaceBaseType::ConstExpressionPtr 
         ConstExpressionPtr;

      public:
	template<typename E>
	Interface(E e) : Statement<SingleExpression>(e) {}

         void setLabel(ExpressionPtr e) {
	   // check_invariant(safe_cast<Expression<Label> >(e),
           //                  "Attempt to set non-label as label");
            
            setExpression(e);
         };

         ExpressionPtr getLabel(void) {
            return(this->getExpression());
         };

         ConstExpressionPtr getLabel(void) const {
            return this->getExpression();
         };
      };

   public:
      typedef boost::mpl::vector<> Properties;
     typedef Statement<SingleExpression> VisitorBaseType;      
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// A before statement specifies a block statement with an entry
  /// label.  A goto-dest statement may transfer control to this
  /// label.  This provides a structured way to return to the top of a
  /// deeply nested set of loops.
  class Before {
   private:
     class Interface : public Statement<SingleLabel>,
		       public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Interface(E e, S1 s1) : Statement<SingleLabel>(e),
			       Statement<SingleBlock>(s1) {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("Before::Base::accept called");
       }
     };

   public:
      typedef boost::mpl::vector<Iterative> Properties;  // Of a sort
     typedef Statement<SingleBlock> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };


  /// An after statement specifies a block with a label at the end.  A
  /// goto-dest statement may transfer control to this label.  This
  /// provides a structured way to implement break- and continue-type
  /// statements.
   class After {
   private:
     class Interface : public Statement<SingleLabel>,
		       public Statement<SingleBlock> {
     public:
       template<typename S1, typename E>
       Interface(E e, S1 s1) : Statement<SingleLabel>(e),
			       Statement<SingleBlock>(s1) {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("After::Base::accept called");
       }
     };

   public:
      typedef boost::mpl::vector<Conditional> Properties;  // Of a sort
     typedef Statement<SingleBlock> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

  /// This is an arbitrary goto statement.  It should not appear in
  /// normalized IR but is necessary to handle translation from
  /// hitgher level languages.  A filter will eliminate gotos and
  /// restructure the IR to be fully structured.
   class Goto {
   private:
     class Interface : public Statement<SingleLabel>,
		       public LeafStatement {
     public:
       template<typename E>
       Interface(E e) : Statement<SingleLabel>(e),
			LeafStatement() {}

       virtual void accept(mirv::StatementVisitor &) {
	 error("Goto::Base::accept called");
       }
     };

   public:
      typedef boost::mpl::vector<Conditional> Properties;
     typedef LeafStatement VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };

   /// A return does not return an expression.  Instead, assignment to
   /// a special location determines the return value.  This separates
   /// changes in data state from changes in control state.
   class Return {
   public:
      typedef boost::mpl::vector<> Properties;
     typedef LeafStatement VisitorBaseType;
      typedef StatementBaseGenerator<Properties, LeafStatement>::type BaseType;
   };
};

#endif