#include <mirv/Core/Filter/ConstStatementVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>

namespace mirv {
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Base> >::const_type s) {};
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<LeafStatement>::const_type s) {
    typedef detail::VisitorBaseTypeOf<LeafStatement>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<InnerStatement>::const_type s) {
    typedef detail::VisitorBaseTypeOf<InnerStatement>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  // Establish an order for properties:
  // Mutating->Iterative->Conditional.  This coresponds to the
  // property tag ordering.
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Conditional> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Conditional>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Iterative> >::const_type s) {
    if (ptr<Statement<Conditional> >::const_type cs = 
	dyn_cast<const Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      typedef detail::VisitorBaseTypeOfStatement<Iterative>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
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
      typedef detail::VisitorBaseTypeOfStatement<Mutating>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
      visit(p);
    }
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Block> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Block>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<SingleBlock> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<SingleBlock>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<DualBlock> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<DualBlock>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<IfThen> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<IfThen>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<IfElse> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<IfElse>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<While> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<While>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<DoWhile> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<DoWhile>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Switch> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Switch>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Case> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Case>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<CaseBlock> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<CaseBlock>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Before> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Before>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<After> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<After>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Goto> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Goto>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Return> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Return>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Phi> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Phi>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Store> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Store>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Call> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Call>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<Statement<Allocate> >::const_type s) {
    typedef detail::VisitorBaseTypeOfStatement<Allocate>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
}
