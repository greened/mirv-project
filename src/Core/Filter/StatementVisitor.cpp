#include <mirv/Core/Filter/StatementVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/StructType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/PointerType.hpp>

namespace mirv {
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Base> >::type s) {};
  StatementVisitor::result_type StatementVisitor::visit(ptr<LeafStatement>::type s) {
    ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<InnerStatement>::type s) {
    ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
    visit(p);
  }
  // Establish an order for properties:
  // Mutating->Iterative->Conditional.  This coresponds to the
  // property tag ordering.
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Conditional> >::type s) {
    ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Iterative> >::type s) {
    if (ptr<Statement<Conditional> >::type cs = 
	dyn_cast<Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
      visit(p);
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Mutating> >::type s) {
    if (ptr<Statement<Iterative> >::type is = 
	dyn_cast<Statement<Iterative> >(s)) {
      visit(is);
    }
    else if (ptr<Statement<Conditional> >::type cs = 
	     dyn_cast<Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
      visit(p);
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Block> >::type s) {
    ptr<Statement<Block>::VisitorBaseType>::type p = fast_cast<Statement<Block>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<SingleBlock> >::type s) {
    ptr<Statement<SingleBlock>::VisitorBaseType>::type p = fast_cast<Statement<SingleBlock>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DualBlock> >::type s) {
    ptr<Statement<DualBlock>::VisitorBaseType>::type p = fast_cast<Statement<DualBlock>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfThen> >::type s) {
    ptr<Statement<IfThen>::VisitorBaseType>::type p = fast_cast<Statement<IfThen>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfElse> >::type s) {
    ptr<Statement<IfElse>::VisitorBaseType>::type p = fast_cast<Statement<IfElse>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<While> >::type s) {
    ptr<Statement<While>::VisitorBaseType>::type p = fast_cast<Statement<While>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DoWhile> >::type s) {
    ptr<Statement<DoWhile>::VisitorBaseType>::type p = fast_cast<Statement<DoWhile>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Switch> >::type s) {
    ptr<Statement<Switch>::VisitorBaseType>::type p = fast_cast<Statement<Switch>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Case> >::type s) {
    ptr<Statement<Case>::VisitorBaseType>::type p = fast_cast<Statement<Case>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<CaseBlock> >::type s) {
    ptr<Statement<CaseBlock>::VisitorBaseType>::type p = fast_cast<Statement<CaseBlock>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Before> >::type s) {
    ptr<Statement<Before>::VisitorBaseType>::type p = fast_cast<Statement<Before>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<After> >::type s) {
    ptr<Statement<After>::VisitorBaseType>::type p = fast_cast<Statement<After>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Goto> >::type s) {
    ptr<Statement<Goto>::VisitorBaseType>::type p = fast_cast<Statement<Goto>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Return> >::type s) {
    ptr<Statement<Return>::VisitorBaseType>::type p = fast_cast<Statement<Return>::VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Assignment> >::type s) {
    ptr<Statement<Assignment>::VisitorBaseType>::type p = fast_cast<Statement<Assignment>::VisitorBaseType>(s);
    visit(p);
  }

  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Call> >::type s) {
    ptr<Statement<Assignment>::VisitorBaseType>::type p = fast_cast<Statement<Call>::VisitorBaseType>(s);
    visit(p);
  }
}
