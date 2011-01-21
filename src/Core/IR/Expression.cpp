#include <mirv/Core/Filter/ExpressionVisitor.hpp>

#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Logical.hpp>
#include <mirv/Core/IR/Bitwise.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/IR/StructType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/PointerType.hpp>

namespace mirv {
  void
  Visitable<
    Expression<Base>,
    ExpressionVisitor,
    boost::mpl::empty_base
    >::accept(ExpressionVisitor &V)
  {
    error("Expression<Base>::accept called");
  }
}
