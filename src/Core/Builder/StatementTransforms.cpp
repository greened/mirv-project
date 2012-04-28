#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>

namespace mirv {
  namespace Builder {
    ptr<Statement<Base> >::type
    IfTransform::operator()(ptr<SymbolTable>::type symtab,
                            ptr<Expression<Base> >::type condition,
                            ptr<Statement<Base> >::type body) {
      return ClearPendingStatements()(
        symtab,
        ConstructBinary<
          Statement<IfThen>,
          ptr<Expression<Base> >::type,
          ptr<Statement<Base> >::type>()(symtab,
                                         condition,
                                         body));
    }

    ptr<Statement<Base> >::type
    IfElseTransform::operator()(ptr<SymbolTable>::type symtab,
                                ptr<Expression<Base> >::type condition,
                                ptr<Statement<Base> >::type thenBody,
                                ptr<Statement<Base> >::type elseBody) {
      return ClearPendingStatements()(
        symtab,
        ConstructTernary<
          Statement<IfElse>,
          ptr<Expression<Base> >::type,
          ptr<Statement<Base> >::type,
          ptr<Statement<Base> >::type>()(symtab,
                                         condition,
                                         thenBody,
                                         elseBody));
    }

    ptr<Statement<Base> >::type
    WhileTransform::operator()(ptr<SymbolTable>::type symtab,
                               ptr<Expression<Base> >::type condition,
                               ptr<Statement<Base> >::type body) {
      // Since there is not while statement, indicate a while by
      // specializing on the grammar rule.
      return ClearPendingStatementsWhileRule()(
        symtab,
        ConstructBinary<
        Statement<IfThen>,
        ptr<Expression<Base> >::type,
        ptr<Statement<Base> >::type>()(symtab,
                                       condition,
                                       DoWhileTransform()(symtab,
                                                          condition,
                                                          body)));
    }

    ptr<Statement<Base> >::type
    DoWhileTransform::operator()(ptr<SymbolTable>::type symtab,
                                 ptr<Expression<Base> >::type condition,
                                 ptr<Statement<Base> >::type body) {
        return ClearPendingStatementsDoWhile()(
          symtab,
          ConstructBinary<
            Statement<DoWhile>,
            ptr<Expression<Base> >::type,
            ptr<Statement<Base> >::type>()(symtab,
                                           condition,
                                           body));
    }

    namespace detail {
      /// Given a value, construct a statement to store to it.
      struct ConstructStore : boost::proto::callable {
        typedef ptr<Statement<Base> >::type result_type;
        
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Expression<Base> >::type address,
                               ptr<Expression<Base> >::type value);
      };

      /// Given a value, construct a statement to store to it.
      ptr<Statement<Base> >::type
      ConstructStore::operator()(boost::shared_ptr<SymbolTable> symtab,
                                 ptr<Expression<Base> >::type address,
                                 ptr<Expression<Base> >::type value) {
        // The address actually comes across as a value, so extract
        // it.
        ptr<Expression<Base> >::type newAddress =
          detail::extractStoreAddress(symtab, address);
        if (!newAddress) {
          std::cerr << "Offending expression:\n";
          printWithTypes(std::cerr, address);
        }

        checkInvariant(newAddress, "Invalid store address");
        return ConstructBinary<Statement<Store> >()(symtab, newAddress, value);
      }
    }

    ptr<Statement<Base> >::type
    AssignTransform::operator()(ptr<SymbolTable>::type symtab,
                                ptr<Expression<Base> >::type lhs,
                                ptr<Expression<Base> >::type rhs) {
      return ClearPendingStatements()(
        symtab,
        detail::ConstructStore()(symtab,
                                 lhs,
                                 rhs));
    }
  }
}
