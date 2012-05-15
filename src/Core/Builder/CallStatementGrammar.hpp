#ifndef mirv_Core_Builder_CallStatementGrammar_hpp
#define mirv_Core_Builder_CallStatementGrammar_hpp

#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>

#include <mirv/Core/Builder/CallExpressionRules.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/pop_front.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to add a statement-level call to
    /// a function.
    struct AddCallAsStatement : boost::proto::callable {
      typedef ptr<Statement<Base> > result_type;

      template<typename Arg, typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg functionExpr,
                             const Expr &expr) {
        ptr<Statement<Call> > call;

        // If this function has a return type, we're ignoring it.
        // Create a temporary to hold the result and pass it to the
        // call statement.
        ptr<const Symbol<Type<FunctionType> > > functionType =
          safe_cast<const Symbol<Type<FunctionType> > >(functionExpr->type());
        if (functionType->getReturnType()) {
          ptr<Symbol<Variable> > temp =
            BinaryConstructSymbol<Symbol<Variable>, CurrentScope>()(
              symtab, "__ct"
              + boost::lexical_cast<std::string>(symtab->getNextTempNum())
              + "__",
              functionType->getReturnType());

          ptr<Expression<Reference<Variable> > > returnValue =
            make<Expression<Reference<Variable> > >(temp);

          call = ConstructNary<Statement<Call> >()(symtab,
                                                   functionExpr,
                                                   returnValue,
                                                   expr);
        }
        else {
          call = ConstructNary<Statement<Call> >()(symtab,
                                                   functionExpr,
                                                   expr);
        }        
        return call;
      }
    };

    /// This is the grammar for function call statements.
    struct CallStatementBuilder :  boost::proto::when<
      CallRule,
      AddCallAsStatement(
        boost::proto::_data,
        ConstructExpressionGrammar(boost::proto::_left),
        boost::proto::_expr)
      > {};
  }
}

#endif
