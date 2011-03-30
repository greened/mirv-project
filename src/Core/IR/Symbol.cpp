#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>
#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/IR/Visitable.ipp>

namespace mirv {
  void
  Visitable<
    Symbol<Base>,
    SymbolVisitor
    >::accept(SymbolVisitor &V)
  {
    error("Symbol<Base>::accept called");
  }

  void
  ConstVisitable<
    Symbol<Base>,
    ConstSymbolVisitor,
    SymbolVisitor
    >::accept(ConstSymbolVisitor &V) const
  {
    error("Symbol<Base>::accept called");
  }

  template
  void
  Visitable<InnerSymbol, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<InnerSymbol, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<LeafSymbol, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<LeafSymbol, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Named>, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Named>, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Typed>, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Typed>, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Global>, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Global>, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Function>, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Function>, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Variable>, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Variable>, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<Base> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<Base> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<std::int8_t> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<std::int8_t> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<std::uint8_t> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<std::uint8_t> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<std::int16_t> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<std::int16_t> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<std::uint16_t> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<std::uint16_t> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<std::int32_t> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<std::int32_t> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<std::uint32_t> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<std::uint32_t> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<std::int64_t> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<std::int64_t> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<std::uint64_t> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<std::uint64_t> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<float> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<float> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Constant<double> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Constant<double> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Module>, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Module>, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<TypeBase> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<TypeBase> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<LeafType>, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<LeafType>, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<InnerType>, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<InnerType>, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<Simple> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<Simple> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<Derived> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<Derived> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<Integral> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<Integral> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<Floating> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<Floating> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<Tuple> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<Tuple> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<Pointer> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<Pointer> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<FunctionType> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<FunctionType> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;

  template
  void
  Visitable<Symbol<Type<Placeholder> >, SymbolVisitor>::accept(SymbolVisitor &);
  template
  void
  ConstVisitable<Symbol<Type<Placeholder> >, ConstSymbolVisitor, SymbolVisitor>::accept(ConstSymbolVisitor &) const;
}
