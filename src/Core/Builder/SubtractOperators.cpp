#include <mirv/Core/Builder/SubtractOperators.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs,
            ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs,
  //           ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  ptr<Expression<Base> >
  operator-(std::int8_t lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(std::uint8_t lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(std::int16_t lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(std::uint16_t lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(std::int32_t lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(std::uint32_t lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(std::int64_t lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(std::uint64_t lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(float lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(double lhs, ptr<Expression<Base> > rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  // ptr<const Expression<Base> >
  // operator-(std::int8_t lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(std::uint8_t lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(std::int16_t lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(std::uint16_t lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(std::int32_t lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(std::uint32_t lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(std::int64_t lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(std::uint64_t lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(float lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(double lhs, ptr<const Expression<Base> > rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, std::int8_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, std::uint8_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, std::int16_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, std::uint16_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, std::int32_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, std::uint32_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, std::int64_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, std::uint64_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, float rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<Expression<Base> >
  operator-(ptr<Expression<Base> > lhs, double rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, std::int8_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, std::uint8_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, std::int16_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, std::uint16_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, std::int32_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, std::uint32_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, std::int64_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, std::uint64_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, float rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const Expression<Base> >
  // operator-(ptr<const Expression<Base> > lhs, double rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }
}
