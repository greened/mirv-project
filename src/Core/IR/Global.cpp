#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/IR/Global.hpp>

namespace mirv {
  Global::Global(const std::string N, ptr<const Type> T) :
    Constant(std::move(N), IRBuilder::getPointerType(T)), ValType(T) {}
}
