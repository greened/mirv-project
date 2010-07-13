#ifndef mirv_Core_Filter_Attribute_hpp
#define mirv_Core_Filter_Attribute_hpp

#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <map>

namespace mirv {
  /// This singleton takes care of mapping IR nodes to attributes.
  /// This makes it easier to add new attributes.
  template<typename T>
  class NodeAttributeManager {
  private:
    /// This is the singleton instance.
    static NodeAttributeManager<T> *inst;

    typedef std::map<ptr<Node<Base> >::type, typename ptr<T>::type> AttributeMap;
    /// This is a map from IR node to attribute.
    AttributeMap attributes;

  public:
    /// Get the singleton instance.
    static NodeAttributeManager<T> &instance(void) {
      return *(inst ? inst : inst = new NodeAttributeManager<T>);
    }

  /// Set an attribute for and IR node.
    void setAttribute(ptr<Node<Base> >::type node, typename ptr<T>::type attr) {
      attributes[node] = attr;
    }

    /// Get the attribute for an IR node.
    typename ptr<T>::type getAttribute(ptr<Node<Base> >::type node) {
      typename AttributeMap::iterator i = attributes.find(node);
      checkInvariant(i != attributes.end(), "Could not find attribute");
      return i->second;
    }
  };

  /// This is a free function to set a node attribute.
  template<typename T>
  void setAttribute(ptr<Node<Base> >::type node, typename ptr<T>::type attr) {
    return NodeAttributeManager<T>::instance().setAttribute(node, attr);
  }

  /// This is a free function to get a node attribute.
  template<typename T>
  typename ptr<T>::type getAttribute(ptr<Node<Base> >::type node) {
    return NodeAttributeManager<T>::instance().getAttribute(node);
  }
}

#endif
