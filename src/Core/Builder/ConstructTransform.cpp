#include <mirv/Core/Builder/ConstructTransform.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/PointerType.hpp>
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
      ptr<Expression<Base> >
      extractLoadAddress(boost::shared_ptr<SymbolTable> symtab,
                         ptr<Expression<Base> > value) {
        ptr<Expression<Load> > load = safe_cast<Expression<Load> >(value);

        // FIXME: If the load result has a pointer type T *, we
        // probably want to use that value.  If we return the load
        // address it will have type T * * and is only good for
        // calculating an offset from it.  However, if we have an
        // object T * * a and we want to compute &a[0] we are in a
        // bind because we do want to get the address feeding the
        // load.  So we need some context telling us which value to
        // return.
        if (ptr<const Symbol<Type<Pointer> > > pointerType =
            dyn_cast<const Symbol<Type<Pointer> > >(load->type())) {
          // Indicate that the value is already what we want.
          return ptr<Expression<Base> >();
        }

        // Otherwise return the address feeding the load.
        ptr<Expression<Base> > address = load->getOperand();
        load->setOperand(ptr<Expression<Base> >());
        return address;
      }

      // Return the address of value suitable for a store.  Unlike in
      // the load case, we always want the address of the value
      // becausae we are going to store to it.
      ptr<Expression<Base> >
      extractStoreAddress(boost::shared_ptr<SymbolTable> symtab,
                          ptr<Expression<Base> > value) {
        ptr<Expression<Load> > load = safe_cast<Expression<Load> >(value);

        // Return the address feeding the load.
        ptr<Expression<Base> > address = load->getOperand();
        load->setOperand(ptr<Expression<Base> >());
        return address;
      }
    }
    
    ptr<Expression<Base> >
    ConstructAddress::operator()(boost::shared_ptr<SymbolTable> symtab,
                                 ptr<Expression<Base> > base,
                                 ptr<Expression<Base> > index) {
      result_type basePointer = detail::extractLoadAddress(symtab, base);
      if (basePointer) {
        auto offset = Expression<Reference<Constant<Base> > >::make(
          ConstructIntegerConstantSymbol<0>()(symtab));
        return Expression<TuplePointer>::make(basePointer,
                                              offset,
                                              index);
      }
      // Base is already a pointer.  Don't add an extra zero offset
      // because the indices include it.
      return Expression<TuplePointer>::make(base,
                                            index);
    }
  }
}
