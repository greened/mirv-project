#ifndef mirv_core_ir_Symbol_ipp
#define mirv_core_ir_Symbol_ipp

#include <mirv/core/filter/SymbolVisitor.hpp>

#include <mirv/core/ir/TypeFwd.hpp>
#include <mirv/core/util/Cast.hpp>

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
