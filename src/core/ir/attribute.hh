#ifndef mirv_core_filter_attribute_hh
#define mirv_core_filter_attribute_hh

#include <mirv/core/ir/node.hh>
#include <mirv/core/mem/heap.hh>

#include <map>

namespace mirv {
  template<typename T>
  class AttributeManager {
  private:
    static AttributeManager<T> *instance;

    typedef std::map<ptr<Node<Base> >::type, typename ptr<T>::type> AttributeMap;
    AttributeMap attributes;

  public:
    static AttributeManager<T> &Instance(void) {
      return *(instance ? instance : instance = new AttributeManager<T>);
    }

    void set_attribute(ptr<Node<Base> >::type node, typename ptr<T>::type attr) {
      attributes[node] = attr;
    }
    typename ptr<T>::type get_attribute(ptr<Node<Base> >::type node) {
      typename AttributeMap::iterator i = attributes.find(node);
      check_invariant(i != attributes.end(), "Could not find attribute");
      return i->second;
    }
  };

  template<typename T>
  void set_attribute(ptr<Node<Base> >::type node, typename ptr<T>::type attr) {
    return AttributeManager<T>::Instance().set_attribute(node, attr);
  }

  template<typename T>
  typename ptr<T>::type get_attribute(ptr<Node<Base> >::type node) {
    return AttributeManager<T>::Instance().get_attribute(node);
  }
}

#endif
