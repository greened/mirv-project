#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      // Return a pointer to the value from which to load.  In the
      // case where the load returns a pointer, return an empty
      // expression indicating the value itself is a pointer.  This
      // handles indexing off pointers.  Array index processing uses
      // this routine to construct TuplePointer expressions suitable
      // for a load operation.
      ptr<ValueProducer>
      extractLoadAddress(ptr<SymbolTable> symtab,
                         ptr<ValueProducer> value) {
        ptr<Load> load = safe_cast<Load>(value);

        // FIXME: If the load result has a pointer type T *, we
        // probably want to use that value.  If we return the load
        // address it will have type T * * and is only good for
        // calculating an offset from it.  However, if we have an
        // object T * * a and we want to compute &a[0] we are in a
        // bind because we do want to get the address feeding the
        // load.  So we need some context telling us which value to
        // return.
        ptr<const PointerType> pointerType =
          dyn_cast<const PointerType>(load->getType());
        if (pointerType) {
          // Indicate that the value is already what we want.
          return ptr<ValueProducer>();
        }

        // Otherwise return the address feeding the load.
        ptr<ValueProducer> address = load->getOperand();
        return address;
      }

      // Return the address of value suitable for a store.  Unlike in
      // the load case, we always want the address of the value
      // becausae we are going to store to it.
      ptr<ValueProducer>
      extractStoreAddress(ptr<SymbolTable> symtab,
                          ptr<ValueProducer> value) {
        ptr<Load> load = safe_cast<Load>(value);

        // Return the address feeding the load.
        ptr<ValueProducer> address = load->getOperand();
        return address;
      }
    }

    ptr<ValueProducer>
    ConstructAddress::operator()(ptr<SymbolTable> symtab,
                                 ptr<ValueProducer> base,
                                 ptr<ValueProducer> index) {
      result_type basePointer = detail::extractLoadAddress(symtab, base);
      if (basePointer) {
        auto IntType = IRBuilder::getIntegerType(64);
        auto Zero = IRBuilder::getIntegerConstant(IntType, 0);
        return IRBuilder::get<TuplePointer>(IRBuilder::getTempName(),
                                            basePointer,
                                            Zero,
                                            index);
      }
      // Base is already a pointer.  Don't add an extra zero offset
      // because the indices include it.
      return IRBuilder::get<TuplePointer>(IRBuilder::getTempName(),
                                          base,
                                          index);
    }
  }
}
