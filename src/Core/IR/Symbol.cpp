#include <mirv/Core/Filter/SymbolVisitor.hpp>

#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Utility/Cast.hpp>

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
