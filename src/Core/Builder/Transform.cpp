#include <mirv/Core/Builder/ConstructTransform.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  namespace Builder {
    ptr<Producer>
    AddAllocateStatement::operator()(ptr<SymbolTable> symtab,
                                     ptr<Producer> Alloc) {
      symtab->getCurrentScope().getBlock()->
        push_back(IRBuilder::get<Sequence>(Alloc));
      return Alloc;
    }
#if 0
    ClearPendingStatements::result_type
    ClearPendingStatements::operator()(ptr<SymbolTable> symtab,
                                       StatementPointer stmt) {
      // Add the pending statements to the function.
      checkInvariant(symtab->pendingStatementsEmpty(),
                     "Pending statements exist");

      // TODO: Should use splice but it exposes the symtab statement
      // list.
      // for (SymbolTable::StatementIterator s =
      //        symtab->pendingStatementsBegin();
      //      s != symtab->pendingStatementsEnd();
      //      ++s) {
      //   symtab->getFunction()->getBlock()->push_back(*s);
      // }

      // symtab->clearPendingStatements();

      // This will get added by the caller.
      return stmt;
    }

    ClearPendingStatementsDoWhile::result_type
    ClearPendingStatementsDoWhile::operator()(ptr<SymbolTable> symtab,
                                              StatementPointer stmt) {
      // Add the pending statements to the loop body.
      checkInvariant(symtab->pendingStatementsEmpty(),
                     "Pending statements exist");

      // TODO: Should use splice but it exposes the symtab statement
      // lst.
      // for (SymbolTable::StatementIterator s =
      //        symtab->pendingStatementsBegin();
      //      s != symtab->pendingStatementsEnd();
      //      ++s) {
      //   safe_cast<Block>(safe_cast<DoWhile>(stmt)->getBlock())->push_back(*s);
      // }

      // symtab->clearPendingStatements();

      // This will get added by the caller.
      return stmt;
    }

    ClearPendingStatementsWhileRule::result_type
    ClearPendingStatementsWhileRule::operator()(ptr<SymbolTable> symtab,
                                                StatementPointer stmt) {
      checkInvariant(symtab->pendingStatementsEmpty(),
                     "Pending statements exist");
      // for (SymbolTable::StatementIterator s =
      //        symtab->pendingStatementsBegin();
      //      s != symtab->pendingStatementsEnd();
      //      ++s) {
      //   // Add the pending statements before the top test.
      //   stmt->getControlParent<Block>()->push_back(*s);

      //   // Add the pending statements to the loop body.
      //   safe_cast<Block> (
      //     safe_cast<DoWhile> (
      //       *(safe_cast<Block> (
      //         stmt->getThenBlock()
      //       )->begin()
      //     )
      //   )->getChildStatement())->push_back((*s)->clone());
      // }

      // symtab->clearPendingStatements();

      // This will get added by the caller.
      return stmt;
    }
#endif
  }
}
