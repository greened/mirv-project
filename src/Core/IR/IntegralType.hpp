#ifndef mirv_Core_IR_IntegralType_hpp
#define mirv_Core_IR_IntegralType_hpp

#include <mirv/Core/IR/IntegralTypeFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/lexical_cast.hpp>

namespace mirv {
  struct Integral;

  /// Integer types.
  namespace detail {
    class IntegralInterface : public Symbol<Type<Simple> >,
                              public boost::enable_shared_from_this<Symbol<Type<Integral> > > {
    private:
      typedef Symbol<Type<Simple> > BaseType;
      std::uint64_t typeSize;
 
    public:
      IntegralInterface(std::uint64_t size);

      ptr<Node<Base>>::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<Node<Base>>::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }
    };
  }

  struct Integral {};
}

#endif
