#ifndef mirv_Core_Builder_BinaryOperators_hpp
#define mirv_Core_Builder_BinaryOperators_hpp

#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/Producers.hpp>

namespace mirv {
  class Module;

  namespace {
    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs,
                   ptr<ValueProducer> rhs) {
      return IRBuilder::get<Op>(IRBuilder::getTempName(), lhs, rhs);
    }

    // template<typename Op>
    // ptr<const Producer>
    // binaryOperator(ptr<const Producer> lhs,
    //                ptr<const Producer> rhs) 
    // {
    //   return make<Expression<Op> >(lhs, rhs);
    // }

    template<typename Op, typename Expr>
    ptr<Producer>
    binaryOperator(const Expr &lhs, ptr<ValueProducer> rhs) {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
                                Builder::makeExpression(lhs),
                                rhs);
    }

    // template<typename Op, typename Expr>
    // ptr<const Producer>
    // binaryOperator(const Expr &lhs, ptr<const Producer> rhs)
    // {
    //   return make<Expression<Op> >(Builder::makeExpression(lhs), rhs);
    // }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(std::int8_t lhs, ptr<ValueProducer> rhs) {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(std::uint8_t lhs, ptr<ValueProducer> rhs) {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(std::int16_t lhs, ptr<ValueProducer> rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(std::uint16_t lhs, ptr<ValueProducer> rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(std::int32_t lhs, ptr<ValueProducer> rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(std::uint32_t lhs, ptr<ValueProducer> rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(std::int64_t lhs, ptr<ValueProducer> rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(std::uint64_t lhs, ptr<ValueProducer> rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(float lhs, ptr<ValueProducer> rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(double lhs, ptr<ValueProducer> rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        rhs);
    }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(std::int8_t lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(std::uint8_t lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(std::int16_t lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(std::uint16_t lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(std::int32_t lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(std::uint32_t lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(std::int64_t lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(std::uint64_t lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(float lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(double lhs, ptr<const ValueProducer> rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     Builder::makeExpression(lhs, rhs->getParentModule()),
    //     rhs);
    // }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, std::int8_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, std::uint8_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, std::int16_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, std::uint16_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, std::int32_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, std::uint32_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, std::int64_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, std::uint64_t rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, float rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(const Expr &lhs, double rhs)
    {
      auto lhsExpression = Builder::makeExpression(lhs);
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhsExpression,
        Builder::makeExpression(rhs, lhsExpression->getParentModule()));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
                                lhs,
                                Builder::makeExpression(rhs));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, std::int8_t rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, std::uint8_t rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, std::int16_t rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, std::uint16_t rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, std::int32_t rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, std::uint32_t rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, std::int64_t rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, std::uint64_t rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, float rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    template<typename Op>
    ptr<ValueProducer>
    binaryOperator(ptr<ValueProducer> lhs, double rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        lhs,
        Builder::makeExpression(rhs, lhs->getParentModule()));
    }

    // template<typename Op, typename Expr>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, const Expr &rhs)
    // {
    //   return IRBuilder::get<Op>(lhs, Builder::makeExpression(rhs));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, std::int8_t rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, std::uint8_t rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, std::int16_t rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, std::uint16_t rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, std::int32_t rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, std::uint32_t rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, std::int64_t rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, std::uint64_t rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, float rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    // template<typename Op>
    // ptr<const ValueProducer>
    // binaryOperator(ptr<const ValueProducer> lhs, double rhs)
    // {
    //   return IRBuilder::get<Op>(
    //     lhs,
    //     Builder::makeExpression(rhs, lhs->getParentModule()));
    // }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(std::int8_t lhs, const Expr &rhs)
    {
      return
        IRBuilder::get<Op>(IRBuilder::getTempName(),
                           Builder::makeExpression(lhs,
                                                   rhs->getParentModule()),
                           Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(std::uint8_t lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(std::int16_t lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(std::uint16_t lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(std::int32_t lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(std::uint32_t lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(std::int64_t lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(std::uint64_t lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(float lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }

    template<typename Op, typename Expr>
    ptr<ValueProducer>
    binaryOperator(double lhs, const Expr &rhs)
    {
      return IRBuilder::get<Op>(IRBuilder::getTempName(),
        Builder::makeExpression(lhs, rhs->getParentModule()),
        Builder::makeExpression(rhs));
    }
  }
}

#endif
