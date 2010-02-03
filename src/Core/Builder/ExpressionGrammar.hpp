#ifndef mirv_Core_Builder_ExpressionGrammar_hpp
#define mirv_Core_Builder_ExpressionGrammar_hpp

#include <mirv/Core/Builder/Expression.hpp>

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
      boost::proto::when<VariableTerminal,
			 ConstructUnary<Expression<Reference<Variable> > >(LookupSymbol<Symbol<Variable> >(boost::proto::_data, boost::proto::_value))> > {};

    /// This is the grammar for negate expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::negate>
      :  boost::proto::when<NegateRule,
			    ConstructUnary<Expression<Negate> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    /// This is the grammar for complement expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::complement>
      : boost::proto::when<ComplementRule,
			   ConstructUnary<Expression<BitwiseComplement> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    /// This is the grammar for deref expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::dereference>
      : boost::proto::when<DereferenceRule,
			   ConstructUnary<Expression<Dereference> >(ConstructExpressionGrammar(boost::proto::_child))> {};

#if 0
    /// This is the grammar for addrof expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::address_of>
      : boost::proto::when<AddressOfRule,
			   ConstructUnary<Expression<AddressOf> >(ConstructExpressionGrammar(boost::proto::_child))> {};
#endif

    /// This is the grammar for logical not expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_not>
      : boost::proto::when<NotRule,
			   ConstructUnary<Expression<LogicalNot> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    /// This is the grammar for add expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::plus>
      : boost::proto::when<
      AddRule,
      ConstructBinary<Expression<Add> >(ConstructExpressionGrammar(boost::proto::_left),
					      ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for subtract expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::minus>
    : boost::proto::when<
    MinusRule,
    ConstructBinary<Expression<Subtract> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for multiply expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::multiplies>
  : boost::proto::when<
  MultipliesRule,
  ConstructBinary<Expression<Multiply> >(ConstructExpressionGrammar(boost::proto::_left),
					       ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for divide expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::divides>
      : boost::proto::when<
      DividesRule,
      ConstructBinary<Expression<Divide> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for left shift expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_left>
    : boost::proto::when<
    ShiftLeftRule,
    ConstructBinary<Expression<ShiftLeft> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for right shift expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_right>
  : boost::proto::when<
  ShiftRightRule,
  ConstructBinary<Expression<ArithmeticShiftRight> >(ConstructExpressionGrammar(boost::proto::_left),
							   ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for modulo expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::modulus>
      : boost::proto::when<
      ModulusRule,
      ConstructBinary<Expression<Modulus> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for greater than expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater>
    : boost::proto::when<
    GreaterRule,
    ConstructBinary<Expression<GreaterThan> >(ConstructExpressionGrammar(boost::proto::_left),
						    ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for less than expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less>
  : boost::proto::when<
  LessRule,
  ConstructBinary<Expression<LessThan> >(ConstructExpressionGrammar(boost::proto::_left),
					       ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for greater than or equal expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater_equal>
      : boost::proto::when<
      GreaterEqualRule,
      ConstructBinary<Expression<GreaterThanOrEqual> >(ConstructExpressionGrammar(boost::proto::_left),
							     ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for less than or equal expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less_equal>
    : boost::proto::when<
    LessEqualRule,
    ConstructBinary<Expression<LessThanOrEqual> >(ConstructExpressionGrammar(boost::proto::_left),
							ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for equal expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::equal_to>
  : boost::proto::when<
  EqualRule,
  ConstructBinary<Expression<Equal> >(ConstructExpressionGrammar(boost::proto::_left),
					    ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for not equal expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::not_equal_to>
      : boost::proto::when<
      NotEqualRule,
      ConstructBinary<Expression<NotEqual> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for logical or expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_or>
    : boost::proto::when<
    OrRule,
    ConstructBinary<Expression<LogicalOr> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for logical and expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_and>
  : boost::proto::when<
  AndRule,
  ConstructBinary<Expression<LogicalAnd> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for bitwise or expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_or>
      : boost::proto::when<
      BitwiseOrRule,
      ConstructBinary<Expression<BitwiseOr> >(ConstructExpressionGrammar(boost::proto::_left),
						    ConstructExpressionGrammar(boost::proto::_right))> {};

  /// This is the grammar for bitwise and expressions.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_and>
    : boost::proto::when<
    BitwiseAndRule,
    ConstructBinary<Expression<BitwiseAnd> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

/// This is the grammar for bitwise xor expressions.
template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_xor>
  : boost::proto::when<
  BitwiseXorRule,
  ConstructBinary<Expression<BitwiseXor> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

    /// This is the grammar for array reference expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::subscript>
      : boost::proto::when<
      SubscriptRule,
      ConstructBinary<Expression<ArrayRef> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

  //         ConstructNary<CallRule, Call>

  /// This aggregates all of the expression grammars, making it the
  /// grammar for all expressions.
  struct ConstructExpressionGrammar
    : boost::proto::switch_<ConstructExpressionGrammarCases> {};
  }
}

#endif
