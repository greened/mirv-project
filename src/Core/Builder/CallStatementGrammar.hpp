#ifndef mirv_Core_Builder_CallStatementGrammar_hpp
#define mirv_Core_Builder_CallStatementGrammar_hpp

#include <mirv/Core/Builder/CallExpressionGrammar.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/pop_front.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to add a statement-level call to
    /// a function.
    struct AddCallAsStatement : boost::proto::callable {
      typedef ptr<Symbol<Function> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             boost::shared_ptr<Statement<Call> > call) {
	result_type function = symtab->getFunction();
	function->statementPushBack(call);
	return function;
      }
    };

    struct CallStatementBuilder : boost::proto::when<
      CallRule,
      CallBuilder(boost::proto::_)
      > {};
  }
}

#endif
