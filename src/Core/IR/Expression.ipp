#ifndef mirv_Core_IR_Expression_ipp
#define mirv_Core_IR_Expression_ipp

#include <mirv/Core/Filter/ExpressionVisitor.hpp>

#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Logical.hpp>
#include <mirv/Core/IR/Bitwise.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
   template<typename Op>
   void Expression<Op>::accept(ExpressionVisitor &V)
   {
     ptr<Node<Base> >::type pn = this->shared_from_this();
     typename ptr<Expression<Op> >::type p = safe_cast<Expression<Op> >(pn);
     V.visit(p);
   }
}

#endif
