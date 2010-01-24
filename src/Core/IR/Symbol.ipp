#ifndef mirv_Core_IR_Symbol_ipp
#define mirv_Core_IR_Symbol_ipp

#include <mirv/Core/Filter/SymbolVisitor.hpp>

#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
   template<typename Tag>
   void Symbol<Tag>::accept(SymbolVisitor &V)
   {
     ptr<Node<Base> >::type pn = this->shared_from_this();
     typename ptr<Symbol<Tag> >::type p = safe_cast<Symbol<Tag> >(pn);
     V.visit(p);
   }
}

#endif
