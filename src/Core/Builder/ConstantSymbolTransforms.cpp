#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>
#include <mirv/Core/Utility/Printer.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Producers.hpp>

#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>

#include <iostream>

namespace mirv {
  namespace Builder {
    namespace detail {
      ptr<IntegerConstant>
      IntegerConstantGen::operator()(size_t bitsize,
                                     int64_t value) {
        auto TheType = IRBuilder::getIntegerType(bitsize);
        return IRBuilder::getIntegerConstant(TheType, value);
      }

      ptr<FloatingPointConstant>
      FloatingPointConstantGen::operator()(size_t bitsize,
                                           double value) {
        auto TheType = IRBuilder::getFloatingPointType(bitsize);
        return IRBuilder::getFloatingPointConstant(TheType, value);
      }

      ptr<TupleConstant>
      StringConstantGen::operator()(const std::string &value) {
        auto Int64Type = IRBuilder::getIntegerType(64);

        auto Size = IRBuilder::getIntegerConstant(Int64Type, value.size());

        auto Int8Type = IRBuilder::getIntegerType(8);

        auto TheType = IRBuilder::getTupleType(Size, Int8Type);

        return IRBuilder::getTupleConstant(TheType, value);
      }

      ptr<ValueProducer>
      GetCStringReference::operator()(ptr<SymbolTable> symtab,
                                      ptr<ValueProducer> str) {
        return str;
      }
    }

    ptr<ValueProducer>
    AddStringConstant::operator()(ptr<SymbolTable> symtab,
                                  ptr<Constant> str) {
      auto String =
        IRBuilder::GetOrCreateGlobalVariable(IRBuilder::getTempName(),
                                             str->getType(),
                                             str);

      return String;
    }

    ptr<Constant>
    ConstructAddressConstantSymbol::operator()(ptr<SymbolTable> symtab,
                                               ptr<Global> symbol) {
      return symbol;
    }
  }
}
