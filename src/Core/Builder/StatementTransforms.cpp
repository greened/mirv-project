#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>

namespace mirv {
  namespace Builder {
    BlockTransform::result_type
    BlockTransform::operator()(ptr<SymbolTable> symtab,
                               ptr<Control> Stmt1,
                               ptr<Control> Stmt2) {
      // symtab->getCurrentScope().getBlock()->push_back(Stmt1);
      // symtab->getCurrentScope().getBlock()->push_back(Stmt2);
      return symtab->getCurrentScope().getBlock();
    }

    IfTransform::result_type
    IfTransform::operator()(ptr<SymbolTable> symtab,
                            ptr<ValueProducer> condition,
                            ptr<Control> body) {
      auto Result = IRBuilder::get<IfThen>(safe_cast<Relational>(condition),
                                           safe_cast<Block>(body));
      symtab->getCurrentScope().getBlock()->push_back(Result);
      return Result;
    }

    IfElseTransform::result_type
    IfElseTransform::operator()(ptr<SymbolTable> symtab,
                                ptr<ValueProducer> condition,
                                ptr<Control> thenBody,
                                ptr<Control> elseBody) {
      auto Result = IRBuilder::get<IfElse>(safe_cast<Relational>(condition),
                                           safe_cast<Block>(thenBody),
                                           safe_cast<Block>(elseBody));
      symtab->getCurrentScope().getBlock()->push_back(Result);
      return Result;
    }

    WhileTransform::result_type
    WhileTransform::operator()(ptr<SymbolTable> symtab,
                               ptr<ValueProducer> condition,
                               ptr<Control> body) {
      // Since there is not while statement, indicate a while by
      // specializing on the grammar rule.
      auto Loop = IRBuilder::get<DoWhile>(safe_cast<Relational>(condition),
                                          safe_cast<Block>(body));
      auto IfBody = IRBuilder::get<Block>(Loop);
      auto Result = IRBuilder::get<IfThen>(safe_cast<Relational>(condition),
                                           IfBody);
      symtab->getCurrentScope().getBlock()->push_back(Result);
      return Result;
    }

    DoWhileTransform::result_type
    DoWhileTransform::operator()(ptr<SymbolTable> symtab,
                                 ptr<ValueProducer> condition,
                                 ptr<Control> body) {
      std::cerr << "DoWhileTransform\n";
      auto Result =
        IRBuilder::get<DoWhile>(safe_cast<Relational>(condition),
                                safe_cast<Block>(body));
      symtab->getCurrentScope().getBlock()->push_back(Result);
      return Result;
    }

    namespace detail {
      /// Given a value, construct a statement to store to it.
      struct ConstructStore : boost::proto::callable {
        typedef ptr<Control> result_type;

        result_type operator()(ptr<SymbolTable> symtab,
                               ptr<ValueProducer> address,
                               ptr<ValueProducer> value);
      };

      /// Given a value, construct a statement to store to it.
      ConstructStore::result_type
      ConstructStore::operator()(ptr<SymbolTable> symtab,
                                 ptr<ValueProducer> address,
                                 ptr<ValueProducer> value) {
        std::cerr << "ConstructStore\n";
        // The address actually comes across as a value, so extract
        // it.
        ptr<ValueProducer> newAddress =
          detail::extractStoreAddress(symtab, address);
        if (!newAddress) {
          std::cerr << "Offending expression:\n";
          printWithTypes(std::cerr, address);
        }

        checkInvariant(newAddress, "Invalid store address");
        auto St = IRBuilder::get<Store>(IRBuilder::getTempName(),
                                           newAddress,
                                           value);
        auto Seq = IRBuilder::get<Sequence>(IRBuilder::getTempName(), St);
        return Seq;
        //        return ConstructBinary<Store>()(symtab, newAddress, value);
      }
    }

    AssignTransform::result_type
    AssignTransform::operator()(ptr<SymbolTable> symtab,
                                ptr<ValueProducer> lhs,
                                ptr<ValueProducer> rhs) {
      std::cerr << "AssignTransform\n";
      auto Seq = detail::ConstructStore()(symtab, lhs, rhs);
      symtab->getCurrentScope().getBlock()->push_back(Seq);
      return Seq;
    }
  }
}
