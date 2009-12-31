#ifndef mirv_core_builder_grammar_hh
#define mirv_core_builder_grammar_hh

#include <mirv/core/builder/expression.hh>
#include <mirv/core/builder/statement.hh>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    struct ConstructExpressionGrammar;

    struct ConstructExpressionGrammarCases {
      // The primary template matches nothing:
      template<typename Tag>
      struct case_ : boost::proto::not_<boost::proto::_> {};
    };

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::terminal>
      : boost::proto::or_<
      boost::proto::when<Variable,
			 ConstructUnary<Expression<mirv::Reference<mirv::Variable> > >(boost::proto::_value)> > {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::negate>
      :  boost::proto::when<Negate,
			    ConstructUnary<Expression<mirv::Negate> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::complement>
      : boost::proto::when<Complement,
			   ConstructUnary<Expression<mirv::BitwiseComplement> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::dereference>
      : boost::proto::when<Dereference,
			    ConstructUnary<Expression<mirv::Dereference> >(ConstructExpressionGrammar(boost::proto::_child))> {};

#if 0
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::address_of>
      : boost::proto::when<AddressOf,
			   ConstructUnary<Expression<mirv::AddressOf> >(ConstructExpressionGrammar(boost::proto::_child))> {};
#endif

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_not>
      : boost::proto::when<Not,
			    ConstructUnary<Expression<mirv::LogicalNot> >(ConstructExpressionGrammar(boost::proto::_child))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::plus>
      : boost::proto::when<
      Add,
      ConstructBinary<Expression<mirv::Add> >(ConstructExpressionGrammar(boost::proto::_left),
					      ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::minus>
      : boost::proto::when<
      Minus,
      ConstructBinary<Expression<mirv::Subtract> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::multiplies>
      : boost::proto::when<
      Multiplies,
      ConstructBinary<Expression<mirv::Multiply> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::divides>
      : boost::proto::when<
      Divides,
      ConstructBinary<Expression<mirv::Divide> >(ConstructExpressionGrammar(boost::proto::_left),
						 ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_left>
      : boost::proto::when<
      ShiftLeft,
      ConstructBinary<Expression<mirv::ShiftLeft> >(ConstructExpressionGrammar(boost::proto::_left),
						    ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_right>
      : boost::proto::when<
      ShiftRight,
      ConstructBinary<Expression<mirv::ArithmeticShiftRight> >(ConstructExpressionGrammar(boost::proto::_left),
							       ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::modulus>
      : boost::proto::when<
      Modulus,
      ConstructBinary<Expression<mirv::Modulus> >(ConstructExpressionGrammar(boost::proto::_left),
						  ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater>
      : boost::proto::when<
      Greater,
      ConstructBinary<Expression<mirv::GreaterThan> >(ConstructExpressionGrammar(boost::proto::_left),
						      ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less>
      : boost::proto::when<
      Less,
      ConstructBinary<Expression<mirv::LessThan> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater_equal>
      : boost::proto::when<
      GreaterEqual,
      ConstructBinary<Expression<mirv::GreaterThanOrEqual> >(ConstructExpressionGrammar(boost::proto::_left),
							     ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less_equal>
      : boost::proto::when<
      LessEqual,
      ConstructBinary<Expression<mirv::LessThanOrEqual> >(ConstructExpressionGrammar(boost::proto::_left),
							  ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::equal_to>
      : boost::proto::when<
      Equal,
      ConstructBinary<Expression<mirv::Equal> >(ConstructExpressionGrammar(boost::proto::_left),
						ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::not_equal_to>
      : boost::proto::when<
      NotEqual,
      ConstructBinary<Expression<mirv::NotEqual> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_or>
      : boost::proto::when<
      Or,
      ConstructBinary<Expression<mirv::LogicalOr> >(ConstructExpressionGrammar(boost::proto::_left),
						    ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_and>
      : boost::proto::when<
      And,
      ConstructBinary<Expression<mirv::LogicalAnd> >(ConstructExpressionGrammar(boost::proto::_left),
						     ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_or>
      : boost::proto::when<
      BitwiseOr,
      ConstructBinary<Expression<mirv::BitwiseOr> >(ConstructExpressionGrammar(boost::proto::_left),
						    ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_and>
      : boost::proto::when<
      BitwiseAnd,
      ConstructBinary<Expression<mirv::BitwiseAnd> >(ConstructExpressionGrammar(boost::proto::_left),
						     ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_xor>
      : boost::proto::when<
      BitwiseXor,
      ConstructBinary<Expression<mirv::BitwiseXor> >(ConstructExpressionGrammar(boost::proto::_left),
						     ConstructExpressionGrammar(boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::subscript>
      : boost::proto::when<
      Subscript,
      ConstructBinary<Expression<mirv::ArrayRef> >(ConstructExpressionGrammar(boost::proto::_left),
						   ConstructExpressionGrammar(boost::proto::_right))> {};

    //         ConstructNary<Call, mirv::Call>

    struct ConstructExpressionGrammar
      : boost::proto::switch_<ConstructExpressionGrammarCases> {};

    // Define the statement grammar.
    struct ConstructStatementGrammar;

    struct ConstructStatementGrammarCases {
      // The primary template matches nothing:
      template<typename Tag>
      struct case_ : boost::proto::not_<boost::proto::_> {};
    };

    template<>
    struct ConstructStatementGrammarCases::case_<boost::proto::tag::assign>
      : boost::proto::when<
      Assign,
      ConstructBinary<Statement<mirv::Assignment> >(ConstructExpressionGrammar(boost::proto::_left),
						    ConstructExpressionGrammar(boost::proto::_right))> {};

    struct ConstructStatementGrammar
      : boost::proto::switch_<ConstructStatementGrammarCases> {};

    // General IR grammar.
    struct ConstructGrammar
      : boost::proto::or_<
      ConstructExpressionGrammar,
      ConstructStatementGrammar> {};
  }
}

#endif
