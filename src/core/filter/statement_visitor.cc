#include <mirv/core/filter/statement_visitor.hh>
#include <mirv/core/util/cast.hh>

#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/control.hh>
#include <mirv/core/ir/mutating.hh>

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
    ptr<Statement<Block>::visitor_base_type>::type p = fast_cast<Statement<Block>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<SingleBlock> >::type s) {
    ptr<Statement<SingleBlock>::visitor_base_type>::type p = fast_cast<Statement<SingleBlock>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DualBlock> >::type s) {
    ptr<Statement<DualBlock>::visitor_base_type>::type p = fast_cast<Statement<DualBlock>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfThen> >::type s) {
    ptr<Statement<IfThen>::visitor_base_type>::type p = fast_cast<Statement<IfThen>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfElse> >::type s) {
    ptr<Statement<IfElse>::visitor_base_type>::type p = fast_cast<Statement<IfElse>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<While> >::type s) {
    ptr<Statement<While>::visitor_base_type>::type p = fast_cast<Statement<While>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DoWhile> >::type s) {
    ptr<Statement<DoWhile>::visitor_base_type>::type p = fast_cast<Statement<DoWhile>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Switch> >::type s) {
    ptr<Statement<Switch>::visitor_base_type>::type p = fast_cast<Statement<Switch>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Case> >::type s) {
    ptr<Statement<Case>::visitor_base_type>::type p = fast_cast<Statement<Case>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<CaseBlock> >::type s) {
    ptr<Statement<CaseBlock>::visitor_base_type>::type p = fast_cast<Statement<CaseBlock>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Before> >::type s) {
    ptr<Statement<Before>::visitor_base_type>::type p = fast_cast<Statement<Before>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<After> >::type s) {
    ptr<Statement<After>::visitor_base_type>::type p = fast_cast<Statement<After>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Goto> >::type s) {
    ptr<Statement<Goto>::visitor_base_type>::type p = fast_cast<Statement<Goto>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Return> >::type s) {
    ptr<Statement<Return>::visitor_base_type>::type p = fast_cast<Statement<Return>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Assignment> >::type s) {
    ptr<Statement<Assignment>::visitor_base_type>::type p = fast_cast<Statement<Assignment>::visitor_base_type>(s);
    visit(p);
  }
}
