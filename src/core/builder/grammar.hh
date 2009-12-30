#ifndef mirv_core_builder_grammar_hh
#define mirv_core_builder_grammar_hh

#include <mirv/core/builder/expression.hh>
//#include <mirv/core/builder/statement.hh>

namespace mirv {
  namespace Builder {
    struct ConstructExpressionGrammar;

    struct ConstructExpressionGrammarCases {
      // The primary template matches nothing:
      template<typename Tag>
      struct case_ : proto::not_<_> {};
    };

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::terminal>
      : boost::proto::or<> {};

    //ConstructUnary<Symbol, mirv::Ref>,

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::negate>
      :  boost::proto::when<Negate,
			    ConstructUnary<mirv::Negate>(ConstructExpressionGrammar(boost::proto::_child))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::complement>
      : boost::proto::when<Complement,
			   ConstructUnary<mirv::Complement>(ConstructExpressionGrammar(boost::proto::_child))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::dereference>
      : boost::proto::while<Dereference,
			    ConstructUnary<mirv::Dereference>(ConstructExpressionGrammar(boost::proto::_child))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::address_of>
      : boost::proto::when<AddressOf,
			   ConstructUnary<mirv::AddressOf>(ConstructExpressionGrammar(boost::proto::_child))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::not>
      : boost::proto::when<Not,
			    ConstructUnary<mirv::Not>(ConstructExpressionGrammar(boost::proto::_child))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::plus>
      : boost::proto::when<Add,
			   ConstructBinary<mirv::Add>(ConstructExpressionGrammmar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::minus>
      : boost::proto::when<Minus,
			   ConstructBinary<mirv::Subtract>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::multiplies>
      : boost::proto::when<Multiplies,
			   ConstructBinary<mirv::Multiply>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::divides>
      : boost::proto::when<Divides, ConstructBinary<mirv::Divide>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::shift_left>
      : boost::proto::when<ShiftLeft, ConstructBinary<mirv::ShiftLeft>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::shift_right>
      : boost::proto::when<ShiftRight, ConstructBinary<mirv::ShiftRight>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::modulus>
      : boost::proto::when<Modulus, ConstructBinary<mirv::Modulus>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::greater>
      : boost::proto::when<Greater, ConstructBinary<mirv::GreaterThan>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::less>
      : boost::proto::when<Less, ConstructBinary<mirv::LessThan>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::greater_equal>
      : boost::proto::when<GreaterEqual, ConstructBinary<mirv::GreaterThanOrEqual>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::less_equal>
      : boost::proto::when<LessEqual, ConstructBinary<mirv::LessThanOrEqual>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::equal>
      : boost::proto::when<Equal, ConstructBinary<mirv::Equal>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::not_equal>
      : boost::proto::when<NotEqual, ConstructBinary<mirv::NotEqual>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::or>
      : boost::proto::when<Or, ConstructBinary<mirv::LogicalOr>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::and>
      : boost::proto::when<And, ConstructBinary<mirv::LogicalAnd>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::bitwise_or>
      : boost::proto::when<BitwiseOr, ConstructBinary<mirv::BitwiseOr>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::bitwise_and>
      : boost::proto::when<BitwiseAnd, ConstructBinary<mirv::BitwiseAnd>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::bitwise_xor>
      : boost::proto::when<BitwiseXor, ConstructBinary<mirv::BitwiseXor>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<proto::tag::subscript>
      : boost::proto::when<Subscript, ConstructBinary<mirv::ArrayRef>(ConstructExpressionGrammar(boost::proto::_left, boost::proto::_right))> {};

    //         ConstructNary<Call, mirv::Call>

    struct ConstructExpressionGrammar : boost::proto::switch_<ConstructExpressionGrammarCases> {};

    struct StatementGrammar;

    struct ConstructExpressionGrammarCases {
      // The primary template matches nothing:
      template<typename Tag>
      struct case_ : proto::not_<_> {};
    };

    //ConstructBinary<IfGrammar, MIRV::IfStatement>,
//                  ConstructTernaryLeftNested<IfElseGrammar,
//                                             MIRV::IfElseStatement>,
    //ConstructBinary<WhileGrammar, MIRV::WhileStatement>,
    //ConstructBinary<DoWhileGrammar, MIRV::DoWhileStatement>

    struct Grammar
      : boost::proto::or<
      ConstructExpressionGramar,
      StatementGrammar> {};
  }
}

#endif
