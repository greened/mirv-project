#ifndef mirv_Core_Filter_AttributeManager_hpp
#define mirv_Core_Filter_AttributeManager_hpp

namespace mirv {
  /// The AttributeManager tracks inherited and synthesized attributes
  /// used during flow execution.  Each visited node can query its
  /// inherited attribute and can set synthesized attributes for its
  /// parent to inspect.  Multiple attributes may be set by packaging
  /// them in an aggregate.
  template<typename Attribute>
  class AttributeManager {
  private:
  };
}

#endif
