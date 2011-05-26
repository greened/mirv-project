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

  /// This is an action that transfers the synthesized attribute from
  /// one child to the inherited attribute of the next child.  Flows
  /// can optionally use this to change the attribute flow behavior
  /// from purely top-down/bottom-up to one that tracks an attribute
  /// value through program control (for example, dataflow).
  template<typename Action, typename FlowAttributeManager>
  class AttributeFlowSynthesizedToInheritedAction {
  private:
    Action action;
    FlowAttributeManager &attributeManager;

  public:
    typedef void result_type;

    AttributeFlowSynthesizedToInheritedAction(FlowAttributeManager &am)
        : action(am), attributeManager(am) {}
    template<typename Arg>
    void operator()(const Arg &a) {
      action(a);
      if (attributeManager.setLastSynthesizedAttribute()) {
        attributeManager.setInheritedAttribute(attributeManager.
                                               getLastSynthesizedAttribute());
      }
    }

    template<typename Arg1, typename Arg2>
    void operator()(const Arg1 &a1, const Arg2 &a2) {
      action(a1, a2);
      if (attributeManager.setLastSynthesizedAttribute()) {
        attributeManager.setInheritedAttribute(attributeManager.
                                               getLastSynthesizedAttribute());
      }
    }
  };

  // This action transers an inherited attribute to a synthesized
  // attribute.  It is useful for leaf nodes and other constructs
  // where no explicit synthesized attribute is set but we would like
  // to carry forward some information from the inherited attribute.
  template<typename Action, typename FlowAttributeManager>
  class AttributeFlowInheritedToSynthesizedAction {
  private:
    Action action;
    FlowAttributeManager &attributeManager;

  public:
    typedef void result_type;

    AttributeFlowInheritedToSynthesizedAction(FlowAttributeManager &am)
        : action(am), attributeManager(am) {}
    template<typename Arg>
    void operator()(const Arg &a) {
      action(a);
      // If we didn't set a synthesized attribute and we didn't get
      // one from a child, then propagate the inherited attribute as
      // our synthesized attribute.
      if (   !attributeManager.setLastSynthesizedAttribute()
          && !attributeManager.setLastSynthesizedAttributeForParent()) {
        attributeManager.setSynthesizedAttribute(attributeManager.
                                                 getInheritedAttribute());
      }
    }

    template<typename Arg1, typename Arg2>
    void operator()(const Arg1 &a1, const Arg2 &a2) {
      action(a1, a2);
      if (!attributeManager.setSynthesized()) {
        attributeManager.setSynthesizedAttribute(attributeManager.
                                                 getInheritedAttribute());
      }
    }
  };

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
      FlowAttributeManager<
        Inherited,
        Synthesized
        >
      >,
    detail::AttributeFlowLeaveAction<
      LeaveAction,
      FlowAttributeManager<
        Inherited,
        Synthesized
        >
      >,
        Actions...
    >::type {
  protected:
    typedef AttributeFlow<
    Inherited,
    Synthesized,
    BaseFlowGenerator,
    EnterAction,
    LeaveAction,
    Actions...
    > ThisType;

    typedef FlowAttributeManager<
      Inherited,
      Synthesized
      > FlowAttributeManagerType;

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
