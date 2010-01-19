#ifndef mirv_core_filter_attribute_hh
#define mirv_core_filter_attribute_hh

#include <mirv/core/ir/node.hh>
#include <mirv/core/mem/heap.hh>

#include <map>

namespace mirv {
  /// This singleton takes care of mapping IR nodes to attributes.
  /// This makes it easier to add new attributes.
  template<typename T>
  class AttributeManager {
  private:
    /// This is the singleton instance.
    static AttributeManager<T> *instance;

    typedef std::map<ptr<Node<Base> >::type, typename ptr<T>::type> AttributeMap;
    /// This is a map from IR node to attribute.
    AttributeMap attributes;

  public:
    /// Get the singleton instance.
    static AttributeManager<T> &Instance(void) {
      return *(instance ? instance : instance = new AttributeManager<T>);
    }

  /// Set an attribute for and IR node.
    void set_attribute(ptr<Node<Base> >::type node, typename ptr<T>::type attr) {
      attributes[node] = attr;
    }

    /// Get the attribute for an IR node.
    typename ptr<T>::type get_attribute(ptr<Node<Base> >::type node) {
      typename AttributeMap::iterator i = attributes.find(node);
      check_invariant(i != attributes.end(), "Could not find attribute");
      return i->second;
    }
  };

  /// This is a free function to set a node attribute.
  template<typename T>
  void set_attribute(ptr<Node<Base> >::type node, typename ptr<T>::type attr) {
    return AttributeManager<T>::Instance().set_attribute(node, attr);
  }

  /// This is a free function to get a node attribute.
  template<typename T>
  typename ptr<T>::type get_attribute(ptr<Node<Base> >::type node) {
    return AttributeManager<T>::Instance().get_attribute(node);
  }
}

#endif
