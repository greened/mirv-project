#include <mirv/Core/Filter/SymbolVisitor.hpp>

#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  void Symbol<Type<Inner<detail::InnerTypeTraits> > >::accept(SymbolVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<Symbol<Type<TypeBase> > >::type p =
      safe_cast<Symbol<Type<TypeBase> > >(pn);
    V.visit(p);
  }

  void InnerType::accept(SymbolVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<InnerType>::type p = safe_cast<InnerType>(pn);
    V.visit(p);
  }
}
