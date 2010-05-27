#include <mirv/Core/Filter/SymbolVisitor.hpp>

#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/StructType.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  void Symbol<Type<Inner<detail::InnerTypeTraits> > >::accept(SymbolVisitor &V)
  {
    error("Symbol<Type<Inner<>>>::accept called");
  }

  void InnerType::accept(SymbolVisitor &V)
  {
    error("InnerType::accept called");
  }
}
