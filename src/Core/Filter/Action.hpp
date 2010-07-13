#ifndef mirv_Core_Filter_Action_hpp
#define mirv_Core_Filter_Action_hpp

#include <mirv/Core/Filter/ExpressionVisitor.hpp>
#include <mirv/Core/Filter/StatementVisitor.hpp>
#include <mirv/Core/Filter/SymbolVisitor.hpp>

namespace mirv {
  /// This is an action that does nothing.
  class NullAction {
  public:
    typedef void result_type;

    NullAction(void) {}

    template<typename Arg>
    NullAction(const Arg &) {}

    template<typename Node>
    result_type operator()(boost::shared_ptr<Node>) {}
    template<typename Node1, typename Node2>
    result_type operator()(boost::shared_ptr<Node1>,
                           boost::shared_ptr<Node2>) {}
    template<typename Node1, typename Node2, typename Node3>
    result_type operator()(boost::shared_ptr<Node1>,
                           boost::shared_ptr<Node2>,
                           boost::shared_ptr<Node3>) {}
  };

  /// This is an action that does nothing and says never to iterate.
  class NullJoinAction {
  public:
    typedef bool result_type;

    NullJoinAction(void) {}

    template<typename Arg>
    NullJoinAction(const Arg &) {}

    template<typename Node>
    result_type operator()(boost::shared_ptr<Node>) {
      return false;
    }

    template<typename Node1, typename Node2>
    result_type operator()(boost::shared_ptr<Node1>,
                           boost::shared_ptr<Node2>) {
      return false;
    }

    template<typename Node1, typename Node2, typename Node3>
    result_type operator()(boost::shared_ptr<Node1>,
                           boost::shared_ptr<Node2>,
                           boost::shared_ptr<Node3>) {
      return false;
    }
  };

  /// This action takes a visitor and sends it to the
  /// node for execution.
  template<typename Visitor>
  class VisitAction {
  private:
    Visitor visit;

  protected:
    Visitor &visitor(void) {
      return visit;
    }

  public:
    typedef typename Visitor::result_type result_type;

    template<typename ...Args>
    VisitAction(Args& ...args) : visit(args...) {}

    template<typename Node>
    result_type operator()(boost::shared_ptr<Node> node) {
      return node->accept(visitor());
    }

    template<typename Parent, typename Node>
    result_type operator()(boost::shared_ptr<Parent>,
                           boost::shared_ptr<Node> node) {
      return node->accept(visitor());
    }
  };
}

#endif
