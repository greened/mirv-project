#include <mirv/Core/Filter/SymbolVisitor.hpp>

#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/StructType.hpp>
#include <mirv/Core/Utility/Cast.hpp>

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
  Visitable<
    Symbol<Base>,
    ConstSymbolVisitor
    >::accept(ConstSymbolVisitor &V) const
  {
    error("Symbol<Base>::accept called");
  }
}
