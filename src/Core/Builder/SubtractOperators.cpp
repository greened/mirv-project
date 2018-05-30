#include <mirv/Core/Builder/SubtractOperators.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs,
            ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs,
  //           ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  ptr<ValueProducer>
  operator-(std::int8_t lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(std::uint8_t lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(std::int16_t lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(std::uint16_t lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(std::int32_t lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(std::uint32_t lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(std::int64_t lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(std::uint64_t lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(float lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(double lhs, ptr<ValueProducer> rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  // ptr<const ValueProducer>
  // operator-(std::int8_t lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(std::uint8_t lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(std::int16_t lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(std::uint16_t lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(std::int32_t lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(std::uint32_t lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(std::int64_t lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(std::uint64_t lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(float lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(double lhs, ptr<const ValueProducer> rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, std::int8_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, std::uint8_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, std::int16_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, std::uint16_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, std::int32_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, std::uint32_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, std::int64_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, std::uint64_t rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, float rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  ptr<ValueProducer>
  operator-(ptr<ValueProducer> lhs, double rhs)
  {
    return binaryOperator<Subtract>(lhs, rhs);
  }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, std::int8_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, std::uint8_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, std::int16_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, std::uint16_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, std::int32_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, std::uint32_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, std::int64_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, std::uint64_t rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, float rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }

  // ptr<const ValueProducer>
  // operator-(ptr<const ValueProducer> lhs, double rhs)
  // {
  //   return binaryOperator<Subtract>(lhs, rhs);
  // }
}
