#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>

namespace mirv {
  namespace Builder {
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
  }
}
