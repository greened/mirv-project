#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <iostream>

namespace mirv {
  void Type::dump(void) const {
    print(std::cout, getHandle(this));
  }

  std::string
  VoidType::getNameImpl() const {
    return "void";
  }

  VoidType::BitSizeType
  VoidType::BitSize(void) const {
    return IRBuilder::getIntegerConstant(IRBuilder::getIntegerType(64), 0);
  }

  ScalarType::BitSizeType ScalarType::BitSize(void) const {
    auto IntType = IRBuilder::getIntegerType(getSize());
    BitSizeType size = IRBuilder::getIntegerConstant(IntType, Size);
    return size;
  }

  std::string IntegerType::getNameImpl(void) const {
    return "int" + std::to_string(getSize());
  }

  std::string FloatingPointType::getNameImpl(void) const {
    return "int" + std::to_string(getSize());
  }

  void DerivedType::resolve(ptr<const PlaceholderType> placeholder,
                            ptr<const Type> replacement) {
    for (auto &member : Types()) {
      if (member == placeholder) {
        member = replacement;
        continue;
      }
      if (member == replacement) {
        continue;
      }
      std::const_pointer_cast<Type>(member)->resolve(placeholder, replacement);
    }
  }

  std::string PointerType::getNameImpl(void) const {
    return getBaseType()->getName() + "*";
  }

  PointerType::BitSizeType
  PointerType::BitSize(void) const {
    // FIXME: This is target-dependent.
    return IRBuilder::getIntegerConstant(IRBuilder::getIntegerType(64), 64);
  }

  std::string TupleType::getNameImpl(void) const {
    return Name;
  }


  ptr<const Type>
  TupleType::ElementType(ptr<const ValueProducer> Index) const {
    if (isUniform()) {
      return *begin();
    }
    // index must be an integer constant.
    ptr<const IntegerConstant> C = safe_cast<const IntegerConstant>(Index);
    auto Type = begin();
    std::advance(Type, C->getValue());
    return *Type;
  }

  TupleType::BitSizeType
  TupleType::BitSize(void) const {
    if (empty()) {
      return IRBuilder::getIntegerConstant(IRBuilder::getIntegerType(64), 0);
    }

    if (isUniform()) {
      BitSizeType BitSize = (*begin())->BitSize();
      return IRBuilder::get<Multiply>(IRBuilder::getTempName(),
                                      BitSize, Multiplier);
    }

    BitSizeType Expr =
      IRBuilder::getIntegerConstant(IRBuilder::getIntegerType(64), 0);
    for (auto m = begin(); m != end(); ++m) {
      Expr = IRBuilder::get<Add>(IRBuilder::getTempName(),
                                 Expr, (*m)->BitSize());
    }
    return Expr;
  }

  void
  TupleType::BuildMultiplier(std::uint64_t Count) {
    Multiplier = IRBuilder::getIntegerConstant(IRBuilder::getIntegerType(64),
                                               Count);
  }

  FunctionType::BitSizeType
  FunctionType::BitSize(void) const {
    return IRBuilder::getIntegerConstant(IRBuilder::getIntegerType(64), 0);
  }

  std::string
  FunctionType::getNameImpl(void) const {
    std::string Name = getReturnType()->getName() + "(";

    std::function<std::string()> delim =
      [&]() { delim = []() { return ","; }; return ""; };

    for (auto T : ParameterTypes()) {
      Name += delim();
      Name += T->getName();
    }
    Name += ")";
    return Name;
  }

  std::string
  PlaceholderType::getNameImpl() const {
    return Name;
  }

  PlaceholderType::BitSizeType
  PlaceholderType::BitSize(void) const {
    return IRBuilder::getIntegerConstant(IRBuilder::getIntegerType(64), 0);
  }
}
