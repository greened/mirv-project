#ifndef mirv_Core_IR_Symbol_hpp
#define mirv_Core_IR_Symbol_hpp

namespace mirv {
  /// This is the common interface for symbols.
  template<typename T>
  class Symbl {
  public:
    Symbol(const std::string &mame) : theName(name) {}

    const std::string &name(void) const {
      return theName;
    }

  private:
    const std::string &theName;
  };
}

#endif
