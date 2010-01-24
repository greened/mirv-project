#ifndef mirv_core_filter_Attribute_hpp
#define mirv_core_filter_Attribute_hpp

#include <mirv/core/ir/Node.hpp>
#include <mirv/core/mem/Heap.hpp>

#include <map>

namespace mirv {
  /// This singleton takes care of mapping IR nodes to attributes.
  /// This makes it easier to add new attributes.
  template<typename T>
  class AttributeManager {
  private:
    /// This is the singleton instance.
    static AttributeManager<T> *inst;

    typedef std::map<ptr<Node<Base> >::type, typename ptr<T>::type> AttributeMap;
    /// This is a map from IR node to attribute.
    AttributeMap attributes;

  public:
    /// Get the singleton instance.
    static AttributeManager<T> &instance(void) {
      return *(inst ? inst : inst = new AttributeManager<T>);
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
    return AttributeManager<T>::instance().setAttribute(node, attr);
  }

  /// This is a free function to get a node attribute.
  template<typename T>
  typename ptr<T>::type getAttribute(ptr<Node<Base> >::type node) {
    return AttributeManager<T>::instance().getAttribute(node);
  }
}

#endif
