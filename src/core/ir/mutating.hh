#ifndef mirv_core_ir_mutating_hh
#define mirv_core_ir_mutating_hh

#include <mirv/core/ir/statement.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// This is the interface to statements that have two child
  /// expressions.
   class DualExpression {
   public:
      class Interface;
      typedef Interface BaseType;
      typedef Statement<Controlled> InterfaceBaseType;
     typedef boost::mpl::vector<> Properties;
     typedef Statement<Controlled> VisitorBaseType;

      class Interface : public InterfaceBaseType {
      public:
	template<typename E1, typename E2>
	Interface(E1 e1, E2 e2) : Statement<Controlled>(e1, e2) {}

         typedef InterfaceBaseType::ExpressionPtr ExpressionPtr;
         typedef InterfaceBaseType::ConstExpressionPtr 
         ConstExpressionPtr;
         
         void setLeftExpression(ExpressionPtr e) {
            if (expressionEmpty()) {
               expressionPushBack(e);
            }
            else {
               *expressionBegin() = e;
            }
         };

         void setRightExpression(ExpressionPtr e) {
            if (expressionEmpty()) {
               expressionPushBack(ExpressionPtr()); // Placeholder
               expressionPushBack(e);
            }
            else {
               *--expressionEnd() = e;
            }
         };
         
         ExpressionPtr getLeftExpression(void) {
            checkInvariant(!expressionEmpty() && *expressionBegin(),
			   "Attempt to get non-existent expression");
            return(expressionFront());
         };

         ConstExpressionPtr getLeftExpression(void) const {
            checkInvariant(!expressionEmpty() && *expressionBegin(),
			   "Attempt to get non-existent expression");
            return(expressionFront());
         };

         ExpressionPtr getRightExpression(void) {
            checkInvariant(expressionSize() > 1 && expressionBack(),
			   "Attempt to get non-existent expression");
            return(expressionBack());
         };

         ConstExpressionPtr getRightExpression(void) const {
            checkInvariant(expressionSize() > 1 && expressionBack(),
			   "Attempt to get non-existent expression");
            return(expressionBack());
         };
      };
   };

  /// Specify the interface to assignment statements.  Assignments are
  /// statements only, not expressions.  This is to cleanly separate
  /// changes in program state from general computation.  Expression
  /// trees imply assignmnets to temporary variables at some level of
  /// translate, but we are not concerned about those.
   class Assignment {
   private:
     class Interface : public Statement<DualExpression>,
		       public LeafStatement {
     public:
       template<typename E1, typename E2>
       Interface(E1 e1, E2 e2) : Statement<DualExpression>(e1, e2),
				 LeafStatement() {}
       typedef ExpressionPtr ChildPtr;
       typedef ConstExpressionPtr ConstChildPtr;

       virtual void accept(mirv::StatementVisitor &) {
	 error("Assignment::Base::accept called");
       }
     };

   public:
     typedef boost::mpl::vector<Mutating> Properties;
     typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
     typedef Statement<DualExpression> VisitorBaseType;
   };
}

#endif
