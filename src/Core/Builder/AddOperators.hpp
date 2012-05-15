#ifndef mirv_Core_Builder_AddOperators_hpp
#define mirv_Core_Builder_AddOperators_hpp

#include <mirv/Core/Builder/BinaryOperators.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>

namespace mirv {
  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs,
            ptr<Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs,
  //           ptr<const Expression<Base> > rhs);

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  // template<typename Expr>
  // ptr<const Expression<Base> >
  // operator+(const Expr &lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Add>(lhs, rhs);
  // }

  ptr<Expression<Base> >
  operator+(std::int8_t lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(std::uint8_t lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(std::int16_t lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(std::uint16_t lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(std::int32_t lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(std::uint32_t lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(std::int64_t lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(std::uint64_t lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(float lhs, ptr<Expression<Base> > rhs);

  ptr<Expression<Base> >
  operator+(double lhs, ptr<Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(std::int8_t lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(std::uint8_t lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(std::int16_t lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(std::uint16_t lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(std::int32_t lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(std::uint32_t lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(std::int64_t lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(std::uint64_t lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(float lhs, ptr<const Expression<Base> > rhs);

  // ptr<const Expression<Base> >
  // operator+(double lhs, ptr<const Expression<Base> > rhs);

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, std::int8_t rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, std::uint8_t rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, std::int16_t rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, std::uint16_t rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, std::int32_t rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, std::uint32_t rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, std::int64_t rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, std::uint64_t rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, float rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(const Expr &lhs, double rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, std::int8_t rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, std::uint8_t rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, std::int16_t rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, std::uint16_t rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, std::int32_t rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, std::uint32_t rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, std::int64_t rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, std::uint64_t rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, float rhs);

  ptr<Expression<Base> >
  operator+(ptr<Expression<Base> > lhs, double rhs);

  // template<typename Expr>
  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, const Expr &rhs)
  // {
  //   return binaryOperator<Add>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, std::int8_t rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, std::uint8_t rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, std::int16_t rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, std::uint16_t rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, std::int32_t rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, std::uint32_t rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, std::int64_t rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, std::uint64_t rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, float rhs);

  // ptr<const Expression<Base> >
  // operator+(ptr<const Expression<Base> > lhs, double rhs);

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(std::int8_t lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(std::uint8_t lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(std::int16_t lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(std::uint16_t lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(std::int32_t lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(std::uint32_t lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(std::int64_t lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(std::uint64_t lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(float lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }

  template<typename Expr>
  ptr<Expression<Base> >
  operator+(double lhs, const Expr &rhs)
  {
    return binaryOperator<Add>(lhs, rhs);
  }
}

#endif
