#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>

namespace mirv {
  namespace Builder {
    // ptr<ValueProducer>
    // ConstructGlobalReference::operator()(ptr<SymbolTable> symtab,
    //                                      ptr<GlobalVariable> global) {
    //   auto IntType = IRBuilder::getIntegerType(64);

    //   auto GlobalAddress = global;
    //   auto Zero = IRBuilder::getIntegerConstant(IntType, 0);
    //   auto Pointer =
    //     IRBuilder::get<TuplePointer>(IRBuilder::getTempName(),
    //                                  GlobalAddress,
    //                                  Zero);

    //   return IRBuilder::get<Load>(IRBuilder::getTempName(), Pointer);
    // }
  }
}
