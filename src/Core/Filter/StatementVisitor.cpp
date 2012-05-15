#include <mirv/Core/Filter/StatementVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
// #include <mirv/Core/IR/StructType.hpp>
// #include <mirv/Core/IR/IntegralType.hpp>
// #include <mirv/Core/IR/FloatingType.hpp>
// #include <mirv/Core/IR/ArrayType.hpp>
// #include <mirv/Core/IR/FloatingType.hpp>
// #include <mirv/Core/IR/FunctionType.hpp>
// #include <mirv/Core/IR/PointerType.hpp>

namespace mirv {
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Base> > s) {};
  StatementVisitor::result_type StatementVisitor::visit(ptr<LeafStatement> s) {
    typedef detail::VisitorBaseTypeOf<LeafStatement>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<InnerStatement> s) {
    typedef detail::VisitorBaseTypeOf<InnerStatement>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  // Establish an order for properties:
  // Mutating->Iterative->Conditional.  This coresponds to the
  // property tag ordering.
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Conditional> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Conditional>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Iterative> > s) {
    if (ptr<Statement<Conditional> > cs = 
	dyn_cast<Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      typedef detail::VisitorBaseTypeOfStatement<Iterative>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
      visit(p);
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Mutating> > s) {
    if (ptr<Statement<Iterative> > is = 
	dyn_cast<Statement<Iterative> >(s)) {
      visit(is);
    }
    else if (ptr<Statement<Conditional> > cs = 
	     dyn_cast<Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      typedef detail::VisitorBaseTypeOfStatement<Mutating>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
      visit(p);
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Block> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Block>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<SingleBlock> > s) {
    typedef detail::VisitorBaseTypeOfStatement<SingleBlock>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DualBlock> > s) {
    typedef detail::VisitorBaseTypeOfStatement<DualBlock>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfThen> > s) {
    typedef detail::VisitorBaseTypeOfStatement<IfThen>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfElse> > s) {
    typedef detail::VisitorBaseTypeOfStatement<IfElse>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<While> > s) {
    typedef detail::VisitorBaseTypeOfStatement<While>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DoWhile> > s) {
    typedef detail::VisitorBaseTypeOfStatement<DoWhile>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Switch> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Switch>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Case> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Case>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<CaseBlock> > s) {
    typedef detail::VisitorBaseTypeOfStatement<CaseBlock>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Before> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Before>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<After> > s) {
    typedef detail::VisitorBaseTypeOfStatement<After>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Goto> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Goto>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Return> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Return>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Phi> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Phi>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Store> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Store>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Call> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Call>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Allocate> > s) {
    typedef detail::VisitorBaseTypeOfStatement<Allocate>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
}
