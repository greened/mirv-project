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
  void Expression<Base>::accept(ExpressionVisitor &V)
  {
    error("Expression<Base>::accept called");
  }

  void Expression<Inner<detail::InnerExpressionTraits> >::accept(ExpressionVisitor &V)
  {
    error("Expression<Inner>::accept called");
  }

  void InnerExpression::accept(ExpressionVisitor &V)
  {
    error("InnerExpression::accept called");
  }

  void LeafExpression::accept(ExpressionVisitor &V)
  {
    error("LeafExpression::accept called");
  }

  void Unary::Interface::accept(ExpressionVisitor &V)
  {
    error("Unary::Interface::accept called");
  }

  void Binary::Interface::accept(ExpressionVisitor &V)
  {
    error("Binary::Interface::accept called");
  }
}
