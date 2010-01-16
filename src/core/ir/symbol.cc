#include <mirv/core/filter/symbol_visitor.hh>

#include <mirv/core/ir/symbol.hh>
#include <mirv/core/ir/module.hh>
#include <mirv/core/ir/function.hh>
#include <mirv/core/ir/variable.hh>
#include <mirv/core/ir/type.hh>
#include <mirv/core/util/cast.hh>

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
