#ifndef mirv_Core_Builder_CallExpressionGrammar_hpp
#define mirv_Core_Builder_CallExpressionGrammar_hpp

#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>

#include <mirv/Core/Builder/CallExpressionRules.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/pop_front.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to add an expression-level call
    /// to a function.  It adds the call at a block-level sequence
    /// point because we don't know at this point whether it has
    /// side-effects.
    struct AddFunctionCallAsExpression : boost::proto::callable {
      typedef ptr<ValueProducer> result_type;

      template<typename Arg, typename Expr>
      result_type operator()(ptr<SymbolTable> symtab,
                             Arg functionExpr,
                             const Expr &expr) {
        ptr<FunctionCall> call =
          ConstructNary<FunctionCall>()(functionExpr,
                                        expr);

        symtab->getCurrentScope().getBlock()->
          push_back(IRBuilder::get<Sequence>(call));
        return call;
      }
    };

    /// This is the grammar for function call expressions.
    struct FunctionCallExpressionBuilder :  boost::proto::when<
      FunctionCallRule,
      AddFunctionCallAsExpression(boost::proto::_data,
                                  ConstructExpressionGrammar(boost::proto::_left),
                                  boost::proto::_expr)
      > {};
  }
}

#endif
