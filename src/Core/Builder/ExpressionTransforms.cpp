#include <mirv/Core/Builder/ExpressionTransforms.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  namespace Builder {
    ptr<ValueProducer>
    VariableRefTransform::operator()(ptr<const SymbolTable> symtab,
                                     const std::string &name) {
      std::cerr << "VariableRefTransform\n";
      // Pointer
      auto Alloc = IRBuilder::FindAllocate(name);
      checkInvariant(Alloc != nullptr, "Did not find expected allocate");
      return IRBuilder::get<Load>(IRBuilder::getTempName(),
                                  Alloc);
    }

    ptr<ValueProducer>
    GlobalVariableRefTransform::operator()(ptr<SymbolTable> symtab,
                                           const std::string &name) {
      std::cerr << "GlobalVariableRefTransform\n";
      auto Global = IRBuilder::FindGlobalVariable(name);
      checkInvariant(Global != nullptr, "Did not find expected global");
      auto Address = Global;
      return IRBuilder::get<Load>(IRBuilder::getTempName(),
                                  Address);
    }

    ptr<Function>
    FunctionRefTransform::operator()(ptr<const SymbolTable> symtab,
                                     const std::string &name) {
      return IRBuilder::FindFunction(name);
    }

    ptr<Constant>
    ConstantRefTransform::operator()(ptr<const SymbolTable> symtab,
                                     ptr<Constant> constant) {
      std::cerr << "ConstantRefTransform\n";
      return constant;
    }

    ptr<ValueProducer>
    ArrayRefSequenceTransform::operator()(ptr<const SymbolTable> symtab,
                                          ptr<ValueProducer> address) {
      return address;
    }

    ptr<ValueProducer>
    ArrayRefIndexTransform::operator()(ptr<SymbolTable> symtab,
                                       ptr<ValueProducer> base,
                                       ptr<ValueProducer> index) {
      auto Pointer = IRBuilder::get<TuplePointer>(IRBuilder::getTempName(),
                                                  base,
                                                  index);
      return IRBuilder::get<Load>(IRBuilder::getTempName(),
                                  Pointer);
    }

    ptr<ValueProducer>
    ArrayAddressSequenceTransform::operator()(ptr<const SymbolTable> symtab,
                                              ptr<ValueProducer> address) {
      return address;
    }

    ptr<ValueProducer>
    ArrayAddressIndexTransform::operator()(ptr<SymbolTable> symtab,
                                           ptr<ValueProducer> base,
                                           ptr<ValueProducer> index) {
      return IRBuilder::get<TuplePointer>(IRBuilder::getTempName(),
                                          base,
                                          index);
    }
  }
}
