#include <mirv/Core/Builder/MultiplyOperators.hpp>
#include <mirv/Core/IR/ArrayType.hpp>

namespace mirv {
  Array::Interface::BitSizeType
  Array::Interface::bitsize(void) const 
  {
    BitSizeType expr = getElementType()->bitsize();
    for (auto d = dimensionBegin(); d != dimensionEnd(); ++d) {
      expr = expr * *d;
    }
    return expr;
    // return std::accumulate(dimensionBegin(), dimensionEnd(),
    //                        getElementType()->bitsize(),
    //                        std::multiplies<BitSizeType>());
  }
}
