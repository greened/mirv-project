#ifndef mirv_Core_Filter_Attribute_hpp
#define mirv_Core_Filter_Attribute_hpp

#include <mirv/Core/IR/Attributable.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Library/Map.hpp>
#include <mirv/Library/Singleton.hpp>

namespace mirv {
  /// This singleton takes care of mapping IR nodes to attributes.
  /// This makes it easier to add new attributes.
  template<typename T>
  class NodeAttributeManager : public Singleton<NodeAttributeManager<T>> {
    typedef NodeAttributeManager<T> This;
    typedef Map<ptr<Attributable>, T> AttributeMap;
    AttributeMap Attributes;

    /// Set an attribute for and IR node.
    void setAttributeImpl(ptr<Attributable> node, ptr<T> attr) {
      Attributes[node] = attr;
    }

    /// Get the attribute for an IR node.
    ptr<T> getAttributeImpl(ptr<Attributable> node) {
      typename AttributeMap::iterator i = Attributes.find(node);
      checkInvariant(i != Attributes.end(), "Could not find attribute");
      return i->second;
    }

  public:
    static void setAttribute(ptr<Attributable> node, ptr<T> attr) {
      This::instance().setAttributeImpl(node, attr);
    }
    static ptr<T> getAttribute(ptr<Attributable> node, ptr<T> attr) {
      return This::instance().getAttributeImpl(node, attr);
    }
  };

  /// This is a free function to set a node attribute.
  template<typename T>
  void setAttribute(ptr<Attributable> node, ptr<T> attr) {
    return NodeAttributeManager<T>::setAttribute(node, attr);
  }

  /// This is a free function to get a node attribute.
  template<typename T>
  ptr<T> getAttribute(ptr<Attributable> node) {
    return NodeAttributeManager<T>::getAttribute(node);
  }
}

#endif
