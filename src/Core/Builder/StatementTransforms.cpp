#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>

namespace mirv {
  namespace Builder {
    ptr<Statement<Base> >
    IfTransform::operator()(ptr<SymbolTable> symtab,
                            ptr<Expression<Base> > condition,
                            ptr<Statement<Base> > body) {
      return ClearPendingStatements()(
        symtab,
        ConstructBinary<
          Statement<IfThen>,
          ptr<Expression<Base> >,
          ptr<Statement<Base> >>()(symtab,
                                         condition,
                                         body));
    }

    ptr<Statement<Base> >
    IfElseTransform::operator()(ptr<SymbolTable> symtab,
                                ptr<Expression<Base> > condition,
                                ptr<Statement<Base> > thenBody,
                                ptr<Statement<Base> > elseBody) {
      return ClearPendingStatements()(
        symtab,
        ConstructTernary<
          Statement<IfElse>,
          ptr<Expression<Base> >,
          ptr<Statement<Base> >,
          ptr<Statement<Base> >>()(symtab,
                                         condition,
                                         thenBody,
                                         elseBody));
    }

    ptr<Statement<Base> >
    WhileTransform::operator()(ptr<SymbolTable> symtab,
                               ptr<Expression<Base> > condition,
                               ptr<Statement<Base> > body) {
      // Since there is not while statement, indicate a while by
      // specializing on the grammar rule.
      return ClearPendingStatementsWhileRule()(
        symtab,
        ConstructBinary<
        Statement<IfThen>,
        ptr<Expression<Base> >,
        ptr<Statement<Base> >>()(symtab,
                                       condition,
                                       DoWhileTransform()(symtab,
                                                          condition,
                                                          body)));
    }

    ptr<Statement<Base> >
    DoWhileTransform::operator()(ptr<SymbolTable> symtab,
                                 ptr<Expression<Base> > condition,
                                 ptr<Statement<Base> > body) {
        return ClearPendingStatementsDoWhile()(
          symtab,
          ConstructBinary<
            Statement<DoWhile>,
            ptr<Expression<Base> >,
            ptr<Statement<Base> >>()(symtab,
                                           condition,
                                           body));
    }

    namespace detail {
      /// Given a value, construct a statement to store to it.
      struct ConstructStore : boost::proto::callable {
        typedef ptr<Statement<Base> > result_type;
        
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Expression<Base> > address,
                               ptr<Expression<Base> > value);
      };

      /// Given a value, construct a statement to store to it.
      ptr<Statement<Base> >
      ConstructStore::operator()(boost::shared_ptr<SymbolTable> symtab,
                                 ptr<Expression<Base> > address,
                                 ptr<Expression<Base> > value) {
        // The address actually comes across as a value, so extract
        // it.
        ptr<Expression<Base> > newAddress =
          detail::extractStoreAddress(symtab, address);
        if (!newAddress) {
          std::cerr << "Offending expression:\n";
          printWithTypes(std::cerr, address);
        }

        checkInvariant(newAddress, "Invalid store address");
        return ConstructBinary<Statement<Store> >()(symtab, newAddress, value);
      }
    }

    ptr<Statement<Base> >
    AssignTransform::operator()(ptr<SymbolTable> symtab,
                                ptr<Expression<Base> > lhs,
                                ptr<Expression<Base> > rhs) {
      return ClearPendingStatements()(
        symtab,
        detail::ConstructStore()(symtab,
                                 lhs,
                                 rhs));
    }
  }
}
