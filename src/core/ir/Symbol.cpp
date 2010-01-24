#include <mirv/core/filter/SymbolVisitor.hpp>

#include <mirv/core/ir/Symbol.hpp>
#include <mirv/core/ir/Module.hpp>
#include <mirv/core/ir/Function.hpp>
#include <mirv/core/ir/Variable.hpp>
#include <mirv/core/ir/Type.hpp>
#include <mirv/core/util/Cast.hpp>

namespace mirv {
  void Symbol<Base>::accept(SymbolVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<Symbol<Base> >::type p = safe_cast<Symbol<Base> >(pn);
    V.visit(p);
  }

  void InnerSymbol::accept(SymbolVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<InnerSymbol>::type p = safe_cast<InnerSymbol>(pn);
    V.visit(p);
  }

  void LeafSymbol::accept(SymbolVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<LeafSymbol>::type p = safe_cast<LeafSymbol>(pn);
    V.visit(p);
  }
}
