#ifndef mirv_Library_TypeList_hpp
#define mirv_Library_TypeList_hpp

namespace mirv {
  template<typename ...Types>
  struct TypeList{
    TypeList(void) = delete;
    TypeList(const TypeList &O) = delete;
    TypeList &operator=(const TypeList &O) = delete;
  };
}

#endif
