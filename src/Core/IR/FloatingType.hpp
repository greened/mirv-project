#ifndef mirv_Core_IR_FloatingType_hpp
#define mirv_Core_IR_FloatingType_hpp

#include <mirv/Core/IR/FloatingTypeFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/lexical_cast.hpp>

namespace mirv {
  struct Floating;

  namespace detail {
    /// Define the interface for floating point type symbols.
    class FloatingInterface : public Symbol<Type<Simple> >,
                              public boost::enable_shared_from_this<Symbol<Type<Floating> > > {
    private:
      typedef Symbol<Type<Simple> > BaseType;

    public:
      FloatingInterface(std::uint64_t size);

      ptr<Node<Base>> getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<const Node<Base>> getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }
    };
  }

  /// Floating point types.
  struct Floating {};
}

#endif
