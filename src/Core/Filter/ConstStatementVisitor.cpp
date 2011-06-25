#include <mirv/Core/Filter/ConstStatementVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>

namespace mirv {
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Base> >::const_type s) {};
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<LeafStatement>::const_type s) {
    ptr<Statement<Base> >::const_type p = fast_cast<const Statement<Base> >(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<InnerStatement>::const_type s) {
    ptr<Statement<Base> >::const_type p = fast_cast<const Statement<Base> >(s);
    visit(p);
  }
  // Establish an order for properties:
  // Mutating->Iterative->Conditional.  This coresponds to the
  // property tag ordering.
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Conditional> >::const_type s) {
    ptr<Statement<Base> >::const_type p = fast_cast<const Statement<Base> >(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Iterative> >::const_type s) {
    if (ptr<Statement<Conditional> >::const_type cs = 
	dyn_cast<const Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      ptr<Statement<Base> >::const_type p = fast_cast<const Statement<Base> >(s);
      visit(p);
    }
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Mutating> >::const_type s) {
    if (ptr<Statement<Iterative> >::const_type is = 
	dyn_cast<const Statement<Iterative> >(s)) {
      visit(is);
    }
    else if (ptr<Statement<Conditional> >::const_type cs = 
	     dyn_cast<const Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      ptr<Statement<Base> >::const_type p = fast_cast<const Statement<Base> >(s);
      visit(p);
    }
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Block> >::const_type s) {
    ptr<Statement<Block>::VisitorBaseType>::const_type p = fast_cast<const Statement<Block>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<SingleBlock> >::const_type s) {
    ptr<Statement<SingleBlock>::VisitorBaseType>::const_type p = fast_cast<const Statement<SingleBlock>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<DualBlock> >::const_type s) {
    ptr<Statement<DualBlock>::VisitorBaseType>::const_type p = fast_cast<const Statement<DualBlock>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<IfThen> >::const_type s) {
    ptr<Statement<IfThen>::VisitorBaseType>::const_type p = fast_cast<const Statement<IfThen>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<IfElse> >::const_type s) {
    ptr<Statement<IfElse>::VisitorBaseType>::const_type p = fast_cast<const Statement<IfElse>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<While> >::const_type s) {
    ptr<Statement<While>::VisitorBaseType>::const_type p = fast_cast<const Statement<While>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<DoWhile> >::const_type s) {
    ptr<Statement<DoWhile>::VisitorBaseType>::const_type p = fast_cast<const Statement<DoWhile>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Switch> >::const_type s) {
    ptr<Statement<Switch>::VisitorBaseType>::const_type p = fast_cast<const Statement<Switch>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Case> >::const_type s) {
    ptr<Statement<Case>::VisitorBaseType>::const_type p = fast_cast<const Statement<Case>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<CaseBlock> >::const_type s) {
    ptr<Statement<CaseBlock>::VisitorBaseType>::const_type p = fast_cast<const Statement<CaseBlock>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Before> >::const_type s) {
    ptr<Statement<Before>::VisitorBaseType>::const_type p = fast_cast<const Statement<Before>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<After> >::const_type s) {
    ptr<Statement<After>::VisitorBaseType>::const_type p = fast_cast<const Statement<After>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Goto> >::const_type s) {
    ptr<Statement<Goto>::VisitorBaseType>::const_type p = fast_cast<const Statement<Goto>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Return> >::const_type s) {
    ptr<Statement<Return>::VisitorBaseType>::const_type p = fast_cast<const Statement<Return>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Phi> >::const_type s) {
    ptr<Statement<Phi>::VisitorBaseType>::const_type p = fast_cast<const Statement<Phi>::VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Assignment> >::const_type s) {
    ptr<Statement<Assignment>::VisitorBaseType>::const_type p = fast_cast<const Statement<Assignment>::VisitorBaseType>(s);
    visit(p);
  }

  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Call> >::const_type s) {
    ptr<Statement<Call>::VisitorBaseType>::const_type p =
      fast_cast<const Statement<Call>::VisitorBaseType>(s);
    visit(p);
  }

  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Allocate> >::const_type s) {
    ptr<Statement<Allocate>::VisitorBaseType>::const_type p =
      fast_cast<const Statement<Allocate>::VisitorBaseType>(s);
    visit(p);
  }
}
