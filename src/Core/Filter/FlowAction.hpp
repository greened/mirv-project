#ifndef mirv_Core_Filter_FlowAction_hpp
#define mirv_Core_Filter_FlowAction_hpp

#include <mirv/Core/Memory/Heap.hpp>

namespace mirv {
  template<typename ParentFlow, typename Flow>
  class FlowAction : public VisitAction<Flow> {
  private:
    ParentFlow *parentFlow;

  public:
    template<typename ...Args>
    FlowAction(Args &...args) : VisitAction<Flow>(args...) {}

    void setParentFlow(ParentFlow *pf) {
      parentFlow = pf;
    }

    // FIXME: Maybe move transfers to before/after actions.
    template<typename Expr>
    void operator()(boost::shared_ptr<Expr> child) {
      this->visitor().transfer(*parentFlow);
      VisitAction<Flow>::operator()(child);
      parentFlow->transfer(this->visitor());
    }

    // FIXME: Maybe move transfers to before/after actions.
    template<typename Stmt, typename Expr>
    void operator()(boost::shared_ptr<Stmt> parent,
                    boost::shared_ptr<Expr> child) {
      this->visitor().transfer(*parentFlow);
      VisitAction<Flow>::operator()(parent, child);
      parentFlow->transfer(this->visitor());
    }
  };
}

#endif