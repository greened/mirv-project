#ifndef mirv_Library_List_hpp
#define mirv_Library_List_hpp

#include <list>

#include <boost/intrusive/list.hpp>

namespace mirv {
  template<typename T>
  using List = std::list<T>;

  using namespace boost::intrusive;

  template<typename Tag>
  using IntrusiveListBaseHook =
    boost::intrusive::list_base_hook<boost::intrusive::tag<Tag>>;

  template<typename Type, typename Tag>
  using IntrusiveList =
    boost::intrusive::list<Type, boost::intrusive::base_hook<
                                   IntrusiveListBaseHook<Tag>>>;
}

#endif
