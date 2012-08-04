#ifndef mirv_Core_IR_Index_hpp
#define mirv_Core_IR_Index_hpp

namespace mirv {
  /// A generic table index wrapper.  This provides some type safety for
  /// table lookups.
  template<typename T>
  class Index {
  public:
    typedef long long ValueType;

  private:
    ValueTyps val;

  public:
    Index(ValueType v) : val(v) {}

    ValueType get(void) const 
    {
      return val;
    }
  };
}

#endif
