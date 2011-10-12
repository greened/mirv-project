#ifndef mirv_Core_Builder_AssignGrammar_hpp
#define mirv_Core_Builder_AssignGrammar_hpp

#include <mirv/Core/Builder/AssignRules.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      /// Given a value, construct a statement to store to it.
      struct ConstructStore : boost::proto::callable {
        typedef ptr<Statement<Base> >::type result_type;
        
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Expression<Base> >::type address,
                               ptr<Expression<Base> >::type value);
      };
    }

    /// This is the grammar for assignment statements.
    struct AssignBuilder : boost::proto::when<
      AssignRule,
      ClearPendingStatements(
        boost::proto::_data,
        detail::ConstructStore(boost::proto::_data,
                               ConstructExpressionGrammar(boost::proto::_left),
                               ConstructExpressionGrammar(boost::proto::_right)))
      > {};
  }
}

#endif
