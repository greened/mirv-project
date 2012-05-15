#ifndef mirv_Core_Builder_BinaryOperators_hpp
#define mirv_Core_Builder_BinaryOperators_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/Expression.hpp>

namespace mirv {
  namespace {
    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs,
                   ptr<Expression<Base> > rhs) 
    {
      return make<Expression<Op> >(lhs, rhs);
    }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs,
    //                ptr<const Expression<Base> > rhs) 
    // {
    //   return make<Expression<Op> >(lhs, rhs);
    // }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(Builder::makeExpression(lhs), rhs);
    }

    // template<typename Op, typename Expr>
    // ptr<const Expression<Base> >
    // binaryOperator(const Expr &lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(Builder::makeExpression(lhs), rhs);
    // }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(std::int8_t lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(std::uint8_t lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(std::int16_t lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(std::uint16_t lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(std::int32_t lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(std::uint32_t lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(std::int64_t lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(std::uint64_t lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(float lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(double lhs, ptr<Expression<Base> > rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
        rhs); 
    }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(std::int8_t lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(std::uint8_t lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(std::int16_t lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(std::uint16_t lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(std::int32_t lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(std::uint32_t lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(std::int64_t lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(std::uint64_t lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(float lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(double lhs, ptr<const Expression<Base> > rhs)
    // {
    //   return make<Expression<Op> >(
    //     Builder::makeExpression(lhs, rhs->parent<Symbol<Module> >()),
    //     rhs); 
    // }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, std::int8_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, std::uint8_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, std::int16_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, std::uint16_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, std::int32_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, std::uint32_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, std::int64_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, std::uint64_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, float rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(const Expr &lhs, double rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return make<Expression<Op> >(
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->template parent<Symbol<Module> >()));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(lhs, Builder::makeExpression(rhs));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, std::int8_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, std::uint8_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, std::int16_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, std::uint16_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, std::int32_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, std::uint32_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, std::int64_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, std::uint64_t rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, float rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    template<typename Op>
    ptr<Expression<Base> >
    binaryOperator(ptr<Expression<Base> > lhs, double rhs)
    {
      return make<Expression<Op> >(
        lhs,  
        Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    }

    // template<typename Op, typename Expr>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, const Expr &rhs)
    // {
    //   return make<Expression<Op> >(lhs, Builder::makeExpression(rhs));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, std::int8_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, std::uint8_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, std::int16_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, std::uint16_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, std::int32_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, std::uint32_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, std::int64_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, std::uint64_t rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, float rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    // template<typename Op>
    // ptr<const Expression<Base> >
    // binaryOperator(ptr<const Expression<Base> > lhs, double rhs)
    // {
    //   return make<Expression<Op> >(
    //     lhs,  
    //     Builder::makeExpression(rhs, lhs->parent<Symbol<Module> >()));
    // }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(std::int8_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(std::uint8_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(std::int16_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(std::uint16_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(std::int32_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(std::uint32_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(std::int64_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(std::uint64_t lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(float lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
 
    template<typename Op, typename Expr>
    ptr<Expression<Base> >
    binaryOperator(double lhs, const Expr &rhs)
    {
      return make<Expression<Op> >(
        Builder::makeExpression(lhs, rhs->template parent<Symbol<Module> >()),
        Builder::makeExpression(rhs));
    }
  }
}

#endif
