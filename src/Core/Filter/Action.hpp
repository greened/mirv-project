#ifndef mirv_Core_Filter_Action_hpp
#define mirv_Core_Filter_Action_hpp

#include <mirv/Core/Filter/ControlVisitor.hpp>
#include <mirv/Core/Filter/ValueVisitor.hpp>
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
    result_type operator()(Node &) {}
    template<typename Parent, typename InputIterator>
    result_type operator()(Parent &, InputIterator) {}
    template<typename Parent, typename InputIterator1, typename InputIterator2>
    result_type operator()(Parent &, InputIterator1, InputIterator2) {}
  };

  /// This is an action that does nothing and says never to iterate.
  class NullJoinAction {
  public:
    typedef bool result_type;

    NullJoinAction(void) {}

    template<typename Arg>
    NullJoinAction(const Arg &) {}

    template<typename Node>
    result_type operator()(Node &) {
      return false;
    }

    template<typename Parent, typename InputIterator>
    result_type operator()(Parent &, InputIterator) {
      return false;
    }

    template<typename Parent, typename InputIterator1, typename InputIterator2>
    result_type operator()(Parent &, InputIterator1, InputIterator2) {
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
    typedef void result_type;

    template<typename ...Args>
    VisitAction(Args& ...args) : visit(args...) {}

    template<typename Node>
    result_type operator()(Node & node) {
      node.accept(visitor());
    }

    template<typename Parent, typename Child>
    result_type operator()(Parent &, ptr<Child> node) {
      checkInvariant(node != nullptr, "Null child!");
      node->accept(visitor());
    }
  };
}

#endif
