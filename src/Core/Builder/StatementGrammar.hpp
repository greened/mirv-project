#ifndef mirv_Core_Builder_StatementGrammar_hpp
#define mirv_Core_Builder_StatementGrammar_hpp

#include <mirv/Core/Builder/Statement.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
  // Define the statement grammar.
  struct ConstructStatementGrammar;

  struct ConstructStatementGrammarCases {
    /// This is the default case.  It matches nothing, ensuring that
    /// illegal oncstructs flag an error.
    template<typename Tag>
    struct case_ : boost::proto::not_<boost::proto::_> {};
  };
 
  /// This is the grammar for assignment statements.
  template<>
  struct ConstructStatementGrammarCases::case_<boost::proto::tag::assign>
    : boost::proto::when<
    AssignRule,
    ConstructBinary<Statement<Assignment> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for block statements.  The comma plays the
/// role of the semicolon in C-like languages.
template<>
struct ConstructStatementGrammarCases::case_<boost::proto::tag::comma>
  : boost::proto::when<
  CommaRule,
  ConstructBinary<Statement<Block> >(ConstructStatementGrammar(boost::proto::_left),
				     ConstructStatementGrammar(boost::proto::_right))> {};

    /// This is the grammar for block-type statements, including
    /// if-then, if-then-else, while and do-while.  It covers the
    /// portion of the statement grammar involving the subscript
    /// operator.
    template<>
    struct ConstructStatementGrammarCases::case_<boost::proto::tag::subscript>
      : boost::proto::or_<
      boost::proto::when<
	IfRule,
	ConstructBinary<
	  Statement<IfThen>,
	  ptr<Expression<Base> >::type,
	  ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
				       ConstructStatementGrammar(boost::proto::_right))
	>,
	  boost::proto::when<
	    IfElseRule,
	    ConstructTernary<
	      Statement<IfElse>,
	      ptr<Expression<Base> >::type,
	      ptr<Statement<Base> >::type,
	      ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left(boost::proto::_left)))),
					   ConstructStatementGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left))),
					   ConstructStatementGrammar(boost::proto::_right))
	    >,
      boost::proto::when<
	WhileRule,
	ConstructBinary<
	  Statement<IfThen>,
	  ptr<Expression<Base> >::type,
	  ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
				       ConstructBinary<
				       Statement<DoWhile>,
				       ptr<Expression<Base> >::type,
				       ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
								    ConstructStatementGrammar(boost::proto::_right)))
	>
      > {};

    /// This is the grammar for block-type statements that use
    /// function call operators.  For example, if statements express
    /// the controlling condition through a function call operator.
    /// It covers the portion of the statement grammar using the call
    /// operator.
    template<>
    struct ConstructStatementGrammarCases::case_<boost::proto::tag::function>
      : boost::proto::or_<
      boost::proto::when<
	DoWhileRule,
	ConstructBinary<
	  Statement<DoWhile>,
	  ptr<Expression<Base> >::type,
	  ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right),
				       ConstructStatementGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left))))
	>
      > {};

    /// This aggregates all of the statement rules.  It serves as the
    /// grammar for all statements.
    struct ConstructStatementGrammar
      : boost::proto::switch_<ConstructStatementGrammarCases> {};
  }
}

#endif
