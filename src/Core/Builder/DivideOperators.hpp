#ifndef mirv_Core_Builder_DivideOperators_hpp
#define mirv_Core_Builder_DivideOperators_hpp

#include <mirv/Core/Builder/BinaryOperators.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>

namespace mirv {
  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs,
            ptr<Expression<Base> >::type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs,
  //           ptr<Expression<Base> >::const_type rhs);

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, ptr<Expression<Base> >::type rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  // template<typename Expr>
  // ptr<Expression<Base> >::const_type
  // operator/(const Expr &lhs, ptr<Expression<Base> >::const_type rhs)
  // {
  //   return binaryOperator<Divide>(lhs, rhs);
  // }

  ptr<Expression<Base> >::type
  operator/(std::int8_t lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(std::uint8_t lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(std::int16_t lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(std::uint16_t lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(std::int32_t lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(std::uint32_t lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(std::int64_t lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(std::uint64_t lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(float lhs, ptr<Expression<Base> >::type rhs);

  ptr<Expression<Base> >::type
  operator/(double lhs, ptr<Expression<Base> >::type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(std::int8_t lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(std::uint8_t lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(std::int16_t lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(std::uint16_t lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(std::int32_t lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(std::uint32_t lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(std::int64_t lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(std::uint64_t lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(float lhs, ptr<Expression<Base> >::const_type rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(double lhs, ptr<Expression<Base> >::const_type rhs);

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, std::int8_t rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, std::uint8_t rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, std::int16_t rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, std::uint16_t rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, std::int32_t rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, std::uint32_t rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, std::int64_t rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, std::uint64_t rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, float rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(const Expr &lhs, double rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, std::int8_t rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, std::uint8_t rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, std::int16_t rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, std::uint16_t rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, std::int32_t rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, std::uint32_t rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, std::int64_t rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, std::uint64_t rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, float rhs);

  ptr<Expression<Base> >::type
  operator/(ptr<Expression<Base> >::type lhs, double rhs);

  // template<typename Expr>
  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, const Expr &rhs)
  // {
  //   return binaryOperator<Divide>(lhs, rhs);
  // }

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, std::int8_t rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, std::uint8_t rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, std::int16_t rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, std::uint16_t rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, std::int32_t rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, std::uint32_t rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, std::int64_t rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, std::uint64_t rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, float rhs);

  // ptr<Expression<Base> >::const_type
  // operator/(ptr<Expression<Base> >::const_type lhs, double rhs);

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(std::int8_t lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(std::uint8_t lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(std::int16_t lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(std::uint16_t lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(std::int32_t lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(std::uint32_t lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(std::int64_t lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(std::uint64_t lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(float lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >::type
  operator/(double lhs, const Expr &rhs)
  {
    return binaryOperator<Divide>(lhs, rhs);
  }
}

#endif
