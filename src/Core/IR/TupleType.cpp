// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Builder/MultiplyOperators.hpp>
#include <mirv/Core/Builder/AddOperators.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/ConstExpressionFlow.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/ConstSymbolFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/Action.hpp>

#include <sstream>

namespace mirv {
  void
  Tuple::Interface::construct_optimized(ptr<Symbol<Type<TypeBase> > >::const_type t,
                                        std::uint64_t count) 
  {
    push_back(t);
    multiplier =
      Builder::makeExpression(count, this->parent<Symbol<Module> >());
  }

  ptr<Symbol<Type<TypeBase> > >::const_type
  Tuple::Interface::elementType(ptr<Expression<Base> >::const_type index) const
  {
    if (multiplier) {
      // Types are uniform.
      return *begin();
    }
    // index must be an integer constant.
    ptr<Expression<Reference<Constant<Base> > > >::const_type cref =
      safe_cast<const Expression<Reference<Constant<Base> > > >(index);
    ptr<Symbol<Constant<std::uint64_t> > >::const_type constant =
      safe_cast<const Symbol<Constant<std::uint64_t> > >(cref->getSymbol());
    auto type = begin();
    std::advance(type, constant->value());
    return *type;
  }

  Tuple::Interface::BitSizeType Tuple::Interface::bitsize(void) const
  {
    if (empty()) {
      return Builder::makeExpression(0, this->parent<Symbol<Module> >());
    }

    if (multiplier) {
      BitSizeType bitsize = (*begin())->bitsize();
      return bitsize * multiplier;
    }
    BitSizeType expr =
      Builder::makeExpression(0, this->parent<Symbol<Module> >());
    for (auto m = begin(); m != end(); ++m) {
      expr = expr + (*m)->bitsize();
    }
    return expr;
  }
}
