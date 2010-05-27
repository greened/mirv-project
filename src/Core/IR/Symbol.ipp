// -*-C++-*-
#ifndef mirv_Core_IR_Symbol_ipp
#define mirv_Core_IR_Symbol_ipp

#include <mirv/Core/Filter/NodeVisitor.hpp>
#include <mirv/Core/Filter/SymbolVisitor.hpp>

#include <boost/type_traits.hpp>

namespace mirv {
  template<typename Tag>
  void Symbol<Tag>::accept(SymbolVisitor &V)
  {
    detail::AcceptImpl<Symbol<Tag>,
      boost::is_base_of<boost::enable_shared_from_this<Symbol<Tag> >,
      Symbol<Tag> >::value> impl;
    impl(this, V);
  }
}

#endif
