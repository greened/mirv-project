#ifndef mirv_Core_Builder_BinaryOperators_hpp
#define mirv_Core_Builder_BinaryOperators_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/Expression.hpp>

namespace mirv {
  namespace {
    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs,
                   ptr<Expression<Base> >::type rhs) 
    {
      return make<Expression<Op> >(lhs, rhs);
    }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs,
    //                ptr<Expression<Base> >::const_type rhs) 
    // {
    //   return make<Expression<Op> >(lhs, rhs);
    // }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(Builder::makeExpression(lhs), rhs);
    }

    // template<typename Op, typename Expr>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(const Expr &lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(Builder::makeExpression(lhs), rhs);
    // }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(std::int8_t lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(std::uint8_t lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(std::int16_t lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(std::uint16_t lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(std::int32_t lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(std::uint32_t lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(std::int64_t lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(std::uint64_t lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(float lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(double lhs, ptr<Expression<Base> >::type rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(std::int8_t lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(std::uint8_t lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(std::int16_t lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(std::uint16_t lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(std::int32_t lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(std::uint32_t lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(std::int64_t lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(std::uint64_t lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(float lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(double lhs, ptr<Expression<Base> >::const_type rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, std::int8_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, std::uint8_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, std::int16_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, std::uint16_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, std::int32_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, std::uint32_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, std::int64_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, std::uint64_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, float rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(const Expr &lhs, double rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(lhs, Builder::makeExpression(rhs));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, std::int8_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, std::uint8_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, std::int16_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, std::uint16_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, std::int32_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, std::uint32_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, std::int64_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, std::uint64_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, float rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >::type
    binaryOperator(ptr<Expression<Base> >::type lhs, double rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    // template<typename Op, typename Expr>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, const Expr &rhs)
    // {
    //   return make<Expression<Op> >(lhs, Builder::makeExpression(rhs));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, std::int8_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, std::uint8_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, std::int16_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, std::uint16_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, std::int32_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, std::uint32_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, std::int64_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, std::uint64_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, float rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<Expression<Base> >::const_type
    // binaryOperator(ptr<Expression<Base> >::const_type lhs, double rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(std::int8_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(std::uint8_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(std::int16_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(std::uint16_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(std::int32_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(std::uint32_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(std::int64_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(std::uint64_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(float lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >::type
    binaryOperator(double lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
  }
}

#endif
