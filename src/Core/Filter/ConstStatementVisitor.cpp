#include <mirv/Core/Filter/ConstStatementVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>

namespace mirv {
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Base> > s) {};
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const LeafStatement> s) {
    typedef detail::VisitorBaseTypeOf<LeafStatement>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const InnerStatement> s) {
    typedef detail::VisitorBaseTypeOf<InnerStatement>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  // Establish an order for properties:
  // Mutating->Iterative->Conditional.  This coresponds to the
  // property tag ordering.
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Conditional> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Conditional>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Iterative> > s) {
    if (ptr<const Statement<Conditional> > cs = 
	dyn_cast<const Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      typedef detail::VisitorBaseTypeOfStatement<Iterative>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
      visit(p);
    }
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Mutating> > s) {
    if (ptr<const Statement<Iterative> > is = 
	dyn_cast<const Statement<Iterative> >(s)) {
      visit(is);
    }
    else if (ptr<const Statement<Conditional> > cs = 
	     dyn_cast<const Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      typedef detail::VisitorBaseTypeOfStatement<Mutating>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
      visit(p);
    }
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Block> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Block>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<SingleBlock> > s) {
    typedef detail::VisitorBaseTypeOfStatement<SingleBlock>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<DualBlock> > s) {
    typedef detail::VisitorBaseTypeOfStatement<DualBlock>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<IfThen> > s) {
    typedef detail::VisitorBaseTypeOfStatement<IfThen>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<IfElse> > s) {
    typedef detail::VisitorBaseTypeOfStatement<IfElse>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<While> > s) {
    typedef detail::VisitorBaseTypeOfStatement<While>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<DoWhile> > s) {
    typedef detail::VisitorBaseTypeOfStatement<DoWhile>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Switch> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Switch>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Case> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Case>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<CaseBlock> > s) {
    typedef detail::VisitorBaseTypeOfStatement<CaseBlock>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Before> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Before>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<After> > s) {
    typedef detail::VisitorBaseTypeOfStatement<After>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Goto> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Goto>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Return> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Return>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Phi> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Phi>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Store> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Store>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Call> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Call>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstStatementVisitor::result_type ConstStatementVisitor::visit(ptr<const Statement<Allocate> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Allocate>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
}
