#ifndef mirv_Core_Filter_AttributeFlow_hpp
#define mirv_Core_Filter_AttributeFlow_hpp

#include <mirv/Core/Filter/FlowAttributeManager.hpp>

namespace mirv {
  namespace detail {
    /// This is the Enter action for AttributeFlows.
    template<typename Action, typename FlowAttributeManager>
    class AttributeFlowEnterAction {
    private:
      Action enter;
      FlowAttributeManager &attributeManager;

    public:
      typedef void result_type;

      AttributeFlowEnterAction(FlowAttributeManager &am)
          : enter(am), attributeManager(am) {}
      template<typename Arg>
      void operator()(const Arg &a) {
        attributeManager.pushContext();
        enter(a);
      }
    };

    /// This is the Leave action for AttributeFlows.
    template<typename Action, typename FlowAttributeManager>
    class AttributeFlowLeaveAction {
    private:
      Action leave;
      FlowAttributeManager &attributeManager;

    public:
      typedef void result_type;

      AttributeFlowLeaveAction(FlowAttributeManager &am)
          : leave(am), attributeManager(am) {}
      template<typename Arg>
      void operator()(const Arg &a) {
        leave(a);
        // Do the pop last as we may still need our inherited
        // attribute.
        attributeManager.popContext();
      }
    };
  }

  /// This is a flow that adds attribute management to a base flow.
  template<
    typename Inherited,
    typename Synthesized,
    typename BaseFlowGenerator,
    typename EnterAction,
    typename LeaveAction,
    typename... Actions
    >
  class AttributeFlow : public BaseFlowGenerator::template apply<
    detail::AttributeFlowEnterAction<
      EnterAction,
      FlowAttributeManager<Inherited, Synthesized>
      >,
    detail::AttributeFlowLeaveAction<
      LeaveAction,
      FlowAttributeManager<Inherited, Synthesized>
      >,
        Actions...
    >::type {
  protected:
    typedef FlowAttributeManager<Inherited, Synthesized> FlowAttributeManagerType;

  private:
    typedef typename BaseFlowGenerator::template apply<
    detail::AttributeFlowEnterAction<EnterAction, FlowAttributeManagerType>,
    detail::AttributeFlowLeaveAction<LeaveAction, FlowAttributeManagerType>,
    Actions...>::type BaseType;

    FlowAttributeManagerType attributeManager;

#if 0
    // This doesn't work.  gcc complains about a partial
    // specialization being a friend.
    template<
      typename OtherInherited,
      typename OtherSynthesized,
      typename OtherBaseFlowGenerator,
      typename OtherEnterAction,
      typename OtherLeaveAction,
      typename ...OtherActions
      > friend class AttributeFlow<
      OtherInherited,
      OtherSynthesized,
      OtherBaseFlowGenerator,
      OtherEnterAction,
      OtherLeaveAction,
      OtherActions...
      >;
#endif

    // This is non-copyable and non-assignable.
    //AttributeFlow(const AttributeFlow &) = delete;
    //AttributeFlow(AttributeFlow &&) = delete;
    //AttributeFlow &operator=(const AttributeFlow &) = delete;
    //AttributeFlow &operator=(AttributeFlow &&) = delete;

  public:
    AttributeFlow(const Inherited &inherited)
        : BaseType(attributeManager),
            attributeManager(inherited) {}
    // FIXME: We'd really rather not allow this.  This is forced by
    // the gcc error noted above.
    FlowAttributeManagerType &getAttributeManager(void) {
      return attributeManager;
    }

    template<typename Flow>
    void transfer(Flow &other) {
      attributeManager.swap(other.getAttributeManager());
      BaseType::transfer(other);
    }
  };
}

#endif
