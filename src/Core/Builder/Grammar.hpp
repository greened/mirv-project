#ifndef mirv_Core_Builder_Grammar_hpp
#define mirv_Core_Builder_Grammar_hpp

#include <mirv/Core/Builder/Expression.hpp>
#include <mirv/Core/Builder/Statement.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    struct ConstructExpressionGrammar;

    /// Define the rules for the expression builder grammar.
    struct ConstructExpressionGrammarCases {
      /// This is the default case.  It matches nothing so that
      /// illegal constructs will flag an error.
      template<typename Tag>
      struct case_ : boost::proto::not_<boost::proto::_> {};
    };

    /// This is the grammar for all terminal expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::terminal>
      : boost::proto::or_<
      boost::proto::when<Variable,
			 ConstructUnary<Expression<mirv::Reference<mirv::Variable> > >(boost::proto::_value)> > {};

    /// This is the grammar for negate expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::negate>
      :  boost::proto::when<Negate,
			    ConstructUnary<Expression<mirv::Negate> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    /// This is the grammar for complement expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::complement>
      : boost::proto::when<Complement,
			   ConstructUnary<Expression<mirv::BitwiseComplement> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    /// This is the grammar for deref expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::dereference>
      : boost::proto::when<Dereference,
			   ConstructUnary<Expression<mirv::Dereference> >(ConstructExpressionGrammar(boost::proto::_child))> {};

#if 0
    /// This is the grammar for addrof expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::address_of>
      : boost::proto::when<AddressOf,
			   ConstructUnary<Expression<mirv::AddressOf> >(ConstructExpressionGrammar(boost::proto::_child))> {};
#endif

    /// This is the grammar for logical not expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_not>
      : boost::proto::when<Not,
			   ConstructUnary<Expression<mirv::LogicalNot> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    /// This is the grammar for add expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::plus>
      : boost::proto::when<
      Add,
      ConstructBinary<Expression<mirv::Add> >(ConstructExpressionGrammar(boost::proto::_left),
					      ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for subtract expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::minus>
    : boost::proto::when<
    Minus,
    ConstructBinary<Expression<mirv::Subtract> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for multiply expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::multiplies>
  : boost::proto::when<
  Multiplies,
  ConstructBinary<Expression<mirv::Multiply> >(ConstructExpressionGrammar(boost::proto::_left),
					       ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for divide expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::divides>
      : boost::proto::when<
      Divides,
      ConstructBinary<Expression<mirv::Divide> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for left shift expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_left>
    : boost::proto::when<
    ShiftLeft,
    ConstructBinary<Expression<mirv::ShiftLeft> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for right shift expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_right>
  : boost::proto::when<
  ShiftRight,
  ConstructBinary<Expression<mirv::ArithmeticShiftRight> >(ConstructExpressionGrammar(boost::proto::_left),
							   ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for modulo expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::modulus>
      : boost::proto::when<
      Modulus,
      ConstructBinary<Expression<mirv::Modulus> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for greater than expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater>
    : boost::proto::when<
    Greater,
    ConstructBinary<Expression<mirv::GreaterThan> >(ConstructExpressionGrammar(boost::proto::_left),
						    ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for less than expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less>
  : boost::proto::when<
  Less,
  ConstructBinary<Expression<mirv::LessThan> >(ConstructExpressionGrammar(boost::proto::_left),
					       ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for greater than or equal expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater_equal>
      : boost::proto::when<
      GreaterEqual,
      ConstructBinary<Expression<mirv::GreaterThanOrEqual> >(ConstructExpressionGrammar(boost::proto::_left),
							     ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for less than or equal expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less_equal>
    : boost::proto::when<
    LessEqual,
    ConstructBinary<Expression<mirv::LessThanOrEqual> >(ConstructExpressionGrammar(boost::proto::_left),
							ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for equal expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::equal_to>
  : boost::proto::when<
  Equal,
  ConstructBinary<Expression<mirv::Equal> >(ConstructExpressionGrammar(boost::proto::_left),
					    ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for not equal expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::not_equal_to>
      : boost::proto::when<
      NotEqual,
      ConstructBinary<Expression<mirv::NotEqual> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for logical or expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_or>
    : boost::proto::when<
    Or,
    ConstructBinary<Expression<mirv::LogicalOr> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for logical and expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_and>
  : boost::proto::when<
  And,
  ConstructBinary<Expression<mirv::LogicalAnd> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for bitwise or expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_or>
      : boost::proto::when<
      BitwiseOr,
      ConstructBinary<Expression<mirv::BitwiseOr> >(ConstructExpressionGrammar(boost::proto::_left),
						    ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for bitwise and expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_and>
    : boost::proto::when<
    BitwiseAnd,
    ConstructBinary<Expression<mirv::BitwiseAnd> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for bitwise xor expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_xor>
  : boost::proto::when<
  BitwiseXor,
  ConstructBinary<Expression<mirv::BitwiseXor> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for array reference expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::subscript>
      : boost::proto::when<
      Subscript,
      ConstructBinary<Expression<mirv::ArrayRef> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

  //         ConstructNary<Call, mirv::Call>

  /// This aggregates all of the expression grammars, making it the
  /// grammar for all expressions.
  struct ConstructExpressionGrammar
    : boost::proto::switch_<ConstructExpressionGrammarCases> {};

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
    Assign,
    ConstructBinary<Statement<mirv::Assignment> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for block statements.  The comma plays the
/// role of the semicolon in C-like languages.
template<>
struct ConstructStatementGrammarCases::case_<boost::proto::tag::comma>
  : boost::proto::when<
  Comma,
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
	  Statement<mirv::IfThen>,
	  ptr<Expression<Base> >::type,
	  ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
				       ConstructStatementGrammar(boost::proto::_right))
	>,
	  boost::proto::when<
	    IfElseRule,
	    ConstructTernary<
	      Statement<mirv::IfElse>,
	      ptr<Expression<Base> >::type,
	      ptr<Statement<Base> >::type,
	      ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left(boost::proto::_left)))),
					   ConstructStatementGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left))),
					   ConstructStatementGrammar(boost::proto::_right))
	    >,
      boost::proto::when<
	WhileRule,
	ConstructBinary<
	  Statement<mirv::IfThen>,
	  ptr<Expression<Base> >::type,
	  ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
				       ConstructBinary<
				       Statement<mirv::DoWhile>,
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
	  Statement<mirv::DoWhile>,
	  ptr<Expression<Base> >::type,
	  ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right),
				       ConstructStatementGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left))))
	>
      > {};

    /// This aggregates all of the statement rules.  It serves as the
    /// grammar for all statements.
    struct ConstructStatementGrammar
      : boost::proto::switch_<ConstructStatementGrammarCases> {};

    /// This aggregates all builder grammar rules and actions.  It
    /// serves as the grammar for building the whole IR.
    struct ConstructGrammar
      : boost::proto::or_<
      ConstructExpressionGrammar,
      ConstructStatementGrammar> {};
  }
}

#endif
