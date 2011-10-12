#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  namespace Builder {
    ptr<Statement<Base> >::type
    AddAllocateStatement::operator()(boost::shared_ptr<SymbolTable> symtab,
                                     boost::shared_ptr<Statement<Base> > stmt)
    {
      auto function = symtab->getFunction();
      function->statementPushBack(stmt);
      return stmt;
    }

    ptr<Symbol<Variable> >::type
    ExtractVariable::operator()(boost::shared_ptr<SymbolTable> symtab,
                                ptr<Statement<Base> >::type stmt)
    {
      ptr<Statement<Allocate > >::type allocate =
        safe_cast<Statement<Allocate> >(stmt);

      ptr<Expression<Reference<Variable> > >::type vref =
        safe_cast<Expression<Reference<Variable> > >(
          allocate->getLeftExpression());

      return vref->getSymbol();
    }

    ClearPendingStatements::result_type
    ClearPendingStatements::operator()(boost::shared_ptr<SymbolTable> symtab,
                                       StatementPointer stmt)
    {
      // Add the pending statements to the function.

      // TODO: Should use splice but it exposes the symtab statement
      // list.
      for (SymbolTable::StatementIterator s =
             symtab->pendingStatementsBegin();
           s != symtab->pendingStatementsEnd();
           ++s) {
        symtab->getFunction()->statementPushBack(*s);
      }
        
      symtab->clearPendingStatements();

      // This will get added by the caller.
      return stmt;
    }

    ClearPendingStatementsDoWhile::result_type
    ClearPendingStatementsDoWhile::operator()(boost::shared_ptr<SymbolTable> symtab,
                                              StatementPointer stmt)
    {
      // Add the pending statements to the loop body.

      // TODO: Should use splice but it exposes the symtab statement
      // lst.
      for (SymbolTable::StatementIterator s =
             symtab->pendingStatementsBegin();
           s != symtab->pendingStatementsEnd();
           ++s) {
        safe_cast<Statement<Block> >(stmt->getChildStatement())->
          push_back(*s);
      }
        
      symtab->clearPendingStatements();

      // This will get added by the caller.
      return stmt;
    }

    ClearPendingStatementsWhileRule::result_type
    ClearPendingStatementsWhileRule::operator()(boost::shared_ptr<SymbolTable> symtab,
                                                StatementPointer stmt)
    {
      for (SymbolTable::StatementIterator s =
             symtab->pendingStatementsBegin();
           s != symtab->pendingStatementsEnd();
           ++s) {
        // Add the pending statements before the top test.
        safe_cast<Statement<Block> >(stmt->parent<Statement<Base> >())->
          push_back(*s);

        // Add the pending statements to the loop body.
        safe_cast<Statement<Block> > (
          safe_cast<Statement<DoWhile> >(
            *(safe_cast<Statement<Block> >
              (
                stmt->getChildStatement()
               )->begin()
              )
                                         )->
          getChildStatement())->push_back((*s)->clone());
      }

      symtab->clearPendingStatements();

      // This will get added by the caller.
      return stmt;
    }

    namespace detail {
      // Return a pointer to the value from which to load.  In the
      // case where the load returns a pointer, return an empty
      // expression indicating the value itself is a pointer.  This
      // handles indexing off pointers.  Array index processing uses
      // this routine to construct TuplePointer expressions suitable
      // for a load operation.
      ptr<Expression<Base> >::type
      extractLoadAddress(boost::shared_ptr<SymbolTable> symtab,
                         ptr<Expression<Base> >::type value) {
        ptr<Expression<Load> >::type load = safe_cast<Expression<Load> >(value);

        // FIXME: If the load result has a pointer type T *, we
        // probably want to use that value.  If we return the load
        // address it will have type T * * and is only good for
        // calculating an offset from it.  However, if we have an
        // object T * * a and we want to compute &a[0] we are in a
        // bind because we do want to get the address feeding the
        // load.  So we need some context telling us which value to
        // return.
        if (ptr<Symbol<Type<Pointer> > >::const_type pointerType =
            dyn_cast<const Symbol<Type<Pointer> > >(load->type())) {
          // Indicate that the value is already what we want.
          return ptr<Expression<Base> >::type();
        }

        // Otherwise return the address feeding the load.
        ptr<Expression<Base> >::type address = load->getOperand();
        load->setOperand(ptr<Expression<Base> >::type());
        return address;
      }

      // Return the address of value suitable for a store.  Unlike in
      // the load case, we always want the address of the value
      // becausae we are going to store to it.
      ptr<Expression<Base> >::type
      extractStoreAddress(boost::shared_ptr<SymbolTable> symtab,
                          ptr<Expression<Base> >::type value) {
        ptr<Expression<Load> >::type load = safe_cast<Expression<Load> >(value);

        // Return the address feeding the load.
        ptr<Expression<Base> >::type address = load->getOperand();
        load->setOperand(ptr<Expression<Base> >::type());
        return address;
      }
    }
    
    ptr<Expression<Base> >::type
    ConstructAddress::operator()(boost::shared_ptr<SymbolTable> symtab,
                                 ptr<Expression<Base> >::type base,
                                 ptr<Expression<Base> >::type index) {
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
