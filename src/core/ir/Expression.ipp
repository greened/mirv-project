#ifndef mirv_core_ir_Expression_ipp
#define mirv_core_ir_Expression_ipp

#include <mirv/core/filter/ExpressionVisitor.hpp>

#include <mirv/core/ir/Arithmetic.hpp>
#include <mirv/core/ir/Logical.hpp>
#include <mirv/core/ir/Bitwise.hpp>
#include <mirv/core/ir/Relational.hpp>
#include <mirv/core/ir/Reference.hpp>
#include <mirv/core/util/Cast.hpp>

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
