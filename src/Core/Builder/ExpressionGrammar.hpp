#ifndef mirv_Core_Builder_ExpressionGrammar_hpp
#define mirv_Core_Builder_ExpressionGrammar_hpp

#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>

#include <mirv/Core/Builder/ExpressionRules.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// Define the rules for the expression builder grammar.
    struct ConstructExpressionGrammarCases {
      /// This is the default case.  It matches nothing so that
      /// illegal constructs will flag an error.
      template<typename Tag>
      struct case_ : boost::proto::not_<boost::proto::_> {};
    };

    namespace detail {
      template<typename Rule, typename Tag>
      struct UnaryBuilder : boost::proto::when<
        Rule,
        ConstructUnary<
          Expression<Tag>
          >(ConstructExpressionGrammar(boost::proto::_child))
        > {};

      template<typename Rule, typename Tag>
      struct BinaryBuilder :  boost::proto::when<
        Rule,
        ConstructBinary<
          Expression<Tag>
          >(ConstructExpressionGrammar(boost::proto::_left),
            ConstructExpressionGrammar(boost::proto::_right))
        > {};
    }

    struct VariableRefBuilder : boost::proto::when<
      VariableTerminal,
      ConstructUnary<
        Expression<Reference<Variable> >
        >(LookupSymbol<Symbol<Variable> >(boost::proto::_data,
                                          boost::proto::_value))
      > {};

    struct FunctionRefBuilder : boost::proto::when<
      FunctionTerminal,
      ConstructUnary<
        Expression<Reference<Function> >
        >(LookupSymbol<Symbol<Function> >(boost::proto::_data,
                                          boost::proto::_value))
      > {};
    
    /// This is the grammar for all terminal expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::terminal>
        : boost::proto::or_<
      VariableRefBuilder,
      FunctionRefBuilder
      > {};

    /// This is the grammar for negate expressions.
    struct NegateBuilder : detail::UnaryBuilder<NegateRule, Negate> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::negate>
        : NegateBuilder {};

    /// This is the grammar for complement expressions.
    struct ComplementBuilder
        : detail::UnaryBuilder<ComplementRule, BitwiseComplement> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::complement>
        : ComplementRule {};

    /// This is the grammar for deref expressions.
    struct DerefBuilder 
        : detail::UnaryBuilder<DereferenceRule, Dereference> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<
      boost::proto::tag::dereference
      > : DerefBuilder {};

#if 0
    /// This is the grammar for addrof expressions.
    struct AddressOfBuilder 
        : detail::UnaryBuilder<AddressOfRule, AddressOf> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::address_of>
        : AddressOfBuilder {};
#endif

    /// This is the grammar for logical not expressions.
    struct NotBuilder 
        : detail::UnaryBuilder<NotRule, LogicalNot> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_not>
        : NotBuilder {};

    /// This is the grammar for add expressions.
    struct AddBuilder 
        : detail::BinaryBuilder<AddRule, Add> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::plus>
        : AddBuilder {};

  /// This is the grammar for subtract expressions.
    struct MinusBuilder 
        : detail::BinaryBuilder<MinusRule, Subtract> {};

  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::minus>
      : MinusBuilder {};

/// This is the grammar for multiply expressions.
    struct MultipliesBuilder 
        : detail::BinaryBuilder<MultipliesRule, Multiply> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::multiplies>
        : MultipliesBuilder {};

    /// This is the grammar for divide expressions.
    struct DividesBuilder 
        : detail::BinaryBuilder<DividesRule, Divide> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::divides>
        : DividesBuilder {};

  /// This is the grammar for left shift expressions.
    struct ShiftLeftBuilder 
        : detail::BinaryBuilder<ShiftLeftRule, ShiftLeft> {};

    template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_left>
        : ShiftLeftBuilder {};

/// This is the grammar for right shift expressions.
    struct ShiftRightBuilder 
        : detail::BinaryBuilder<ShiftRightRule, ArithmeticShiftRight> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_right>
    : ShiftRightBuilder {};

    /// This is the grammar for modulo expressions.
    struct ModulusBuilder 
        : detail::BinaryBuilder<ModulusRule, Modulus> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::modulus>
        : ModulusBuilder {};

  /// This is the grammar for greater than expressions.
    struct GreaterBuilder 
        : detail::BinaryBuilder<GreaterRule, GreaterThan> {};

  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater>
      : GreaterBuilder {};

/// This is the grammar for less than expressions.
    struct LessBuilder 
        : detail::BinaryBuilder<LessRule, LessThan> {};

    template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less>
        : LessBuilder {};

    /// This is the grammar for greater than or equal expressions.
    struct GreaterEqualBuilder 
        : detail::BinaryBuilder<GreaterEqualRule, GreaterThanOrEqual> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater_equal>
        : GreaterEqualBuilder {};

  /// This is the grammar for less than or equal expressions.
    struct LessEqualBuilder 
        : detail::BinaryBuilder<LessEqualRule, LessThanOrEqual> {};

  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less_equal>
      : LessEqualBuilder {};

/// This is the grammar for equal expressions.
    struct EqualBuilder 
        : detail::BinaryBuilder<EqualRule, Equal> {};

template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::equal_to>
    : EqualBuilder {};

    /// This is the grammar for not equal expressions.
    struct NotEqualBuilder 
        : detail::BinaryBuilder<NotEqualRule, NotEqual> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::not_equal_to>
        : NotEqualBuilder {};

    /// This is the grammar for logical or expressions.
    struct OrBuilder 
        : detail::BinaryBuilder<OrRule, LogicalOr> {};

  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_or>
      : OrBuilder {};

/// This is the grammar for logical and expressions.
    struct AndBuilder 
        : detail::BinaryBuilder<AndRule, LogicalAnd> {};

    template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_and>
        : AndBuilder {};

    /// This is the grammar for bitwise or expressions.
    struct BitwiseOrBuilder 
        : detail::BinaryBuilder<BitwiseOrRule, BitwiseOr> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_or>
        : BitwiseOrBuilder {};

  /// This is the grammar for bitwise and expressions.
    struct BitwiseAndBuilder 
        : detail::BinaryBuilder<BitwiseAndRule, BitwiseAnd> {};

  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_and>
      : BitwiseAndBuilder {};

/// This is the grammar for bitwise xor expressions.
    struct BitwiseXorBuilder 
        : detail::BinaryBuilder<BitwiseXorRule, BitwiseXor> {};

template<>
struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_xor>
    : BitwiseXorBuilder {};

    /// This is the grammar for array reference expressions.
    struct SubscriptBuilder 
        : detail::BinaryBuilder<SubscriptRule, ArrayRef> {};

    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::subscript>
        : SubscriptBuilder {};

  //         ConstructNary<CallRule, Call>

  /// This aggregates all of the expression grammars, making it the
  /// grammar for all expressions.
  struct ConstructExpressionGrammar
    : boost::proto::switch_<ConstructExpressionGrammarCases> {};
  }
}

#endif
