#ifndef mirv_Core_Builder_CallExpressionGrammar_hpp
#define mirv_Core_Builder_CallExpressionGrammar_hpp

#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>

#include <mirv/Core/Builder/CallExpressionRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/Reference.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/pop_front.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to add an expression-level call
    /// to a function.
    struct AddCallAsExpression : boost::proto::callable {
      typedef ptr<Expression<Base> >::type result_type;

      template<typename Arg, typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg functionExpr,
                             const Expr &expr) {
        ptr<Symbol<Variable> >::type temp =
          BinaryConstructSymbol<Symbol<Variable>, CurrentScope>()(
            symtab, "__ct"
            + boost::lexical_cast<std::string>(symtab->getNextTempNum())
            + "__",
            safe_cast<const Symbol<Type<FunctionType> > >(functionExpr->type())->
            getReturnType());

      ptr<Expression<Reference<Variable> > >::type returnValue =
        make<Expression<Reference<Variable> > >(temp);

      ptr<Statement<Call> >::type call =
        ConstructNary<Statement<Call> >()(symtab,
                                          functionExpr,
                                          returnValue,
                                          expr);

      ptr<Symbol<Function> >::type function = symtab->getFunction();
	function->statementPushBack(call);
	return make<Expression<Reference<Variable> > >(temp);
      }
    };

    /// This is the grammar for function call expressions.
    struct CallExpressionBuilder :  boost::proto::when<
      CallRule,
      AddCallAsExpression(boost::proto::_data,
                          ConstructExpressionGrammar(boost::proto::_left),
                          boost::proto::_expr)
      > {};
  }
}

#endif
