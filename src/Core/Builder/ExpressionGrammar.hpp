#ifndef mirv_Core_Builder_ExpressionGrammar_hpp
#define mirv_Core_Builder_ExpressionGrammar_hpp

#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/ExpressionTransforms.hpp>

#include <mirv/Core/Builder/CallExpressionGrammar.hpp>
#include <mirv/Core/Builder/ConstantGrammar.hpp>
#include <mirv/Core/Builder/ExpressionRules.hpp>
#include <mirv/Core/Builder/ExpressionTerminals.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>

#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Bitwise.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Logical.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/AddressConstant.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>

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
      /// This is a generic grammar to construct unary expressions.
      template<typename Rule, typename Tag>
      struct UnaryBuilder : boost::proto::when<
        Rule,
        ConstructUnary<
          Expression<Tag>
          >(boost::proto::_data,
            ConstructExpressionGrammar(boost::proto::_child))
        > {};

      template<typename Rule, typename Tag>
      struct BinaryBuilder :  boost::proto::when<
        Rule,
        ConstructBinary<
          Expression<Tag>
          >(boost::proto::_data,
            ConstructExpressionGrammar(boost::proto::_left),
            ConstructExpressionGrammar(boost::proto::_right))
        > {};
    }

    /// Given a variable reference, construct a reference to its
    /// value.  Variables are always put on the stack and accessed
    /// via load/store.
    struct VariableRefBuilder : boost::proto::when<
      VariableTerminal,
      VariableRefTransform(boost::proto::_data, boost::proto::_value)
       > {};

    /// Given a global variable, construct a reference to its value.
    /// Globals are always accessed via load/store.
    struct GlobalVariableRefBuilder : boost::proto::when<
      GlobalVariableTerminal,
      GlobalVariableRefTransform(boost::proto::_data, boost::proto::_value)
      > {};

  /// This is the grammar to build a reference to a function.
  struct FunctionRefBuilder : boost::proto::when<
    FunctionTerminal,
    FunctionRefTransform(boost::proto::_data, boost::proto::_value)
    > {};

    /// This is the grammar to construct references to constants.
    struct ConstantRefBuilder : boost::proto::when<
      ConstantBuilder,
      ConstantRefTransform(boost::proto::_data, ConstantBuilder(boost::proto::_))
      > {};

    /// This is the grammar for all terminal expressions.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::terminal>
        : boost::proto::or_<
      GlobalVariableRefBuilder,
      VariableRefBuilder,
      FunctionRefBuilder,
      ConstantRefBuilder,
      StringRefBuilder
      > {};

    /// This is the grammar for negate expressions.
    struct NegateBuilder : detail::UnaryBuilder<NegateRule, Negate> {};

    /// Group negate rules.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::negate>
        : NegateBuilder {};

    /// This is the grammar for complement expressions.
    struct ComplementBuilder
        : detail::UnaryBuilder<ComplementRule, BitwiseComplement> {};

    /// Group bitwise complement rules together.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::complement>
        : ComplementRule {};

    /// This is the grammar for deref expressions.
    //    struct DerefBuilder 
    //        : detail::UnaryBuilder<DereferenceRule, Dereference> {};

    //    template<>
    //    struct ConstructExpressionGrammarCases::case_<
    //      boost::proto::tag::dereference
    //      > : DerefBuilder {};

    /// This is the grammar for logical not expressions.
    struct NotBuilder 
        : detail::UnaryBuilder<NotRule, LogicalNot> {};

    /// Group logical not expression grammars.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_not>
        : NotBuilder {};

    /// This is the grammar for add expressions.
    struct AddBuilder 
        : detail::BinaryBuilder<AddRule, Add> {};

    /// Group addition expression grammars.
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

    /// Group division expression grammars.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::divides>
        : DividesBuilder {};

  /// This is the grammar for left shift expressions.
    struct ShiftLeftBuilder 
        : detail::BinaryBuilder<ShiftLeftRule, ShiftLeft> {};

    /// Group left shift rules.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_left>
        : ShiftLeftBuilder {};

    /// This is the grammar for right shift expressions.
    struct ShiftRightBuilder 
        : detail::BinaryBuilder<ShiftRightRule, ArithmeticShiftRight> {};

    /// Group right shift expression grammars.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::shift_right>
        : ShiftRightBuilder {};

    /// This is the grammar for modulo expressions.
    struct ModulusBuilder 
        : detail::BinaryBuilder<ModulusRule, Modulus> {};

    /// Group modulus rules.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::modulus>
        : ModulusBuilder {};

  /// This is the grammar for greater than expressions.
    struct GreaterBuilder 
        : detail::BinaryBuilder<GreaterRule, GreaterThan> {};

    /// Group > expression grammars
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater>
        : GreaterBuilder {};

    /// This is the grammar for less than expressions.
    struct LessBuilder 
        : detail::BinaryBuilder<LessRule, LessThan> {};

    /// Group < expression grammars.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::less>
        : LessBuilder {};

    /// This is the grammar for greater than or equal expressions.
    struct GreaterEqualBuilder 
        : detail::BinaryBuilder<GreaterEqualRule, GreaterThanOrEqual> {};

    /// Group all greater than or equal rules.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::greater_equal>
        : GreaterEqualBuilder {};

  /// This is the grammar for less than or equal expressions.
    struct LessEqualBuilder 
        : detail::BinaryBuilder<LessEqualRule, LessThanOrEqual> {};

    /// Group <= rules together.
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

    /// Group != expression grammars.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::not_equal_to>
        : NotEqualBuilder {};

    /// This is the grammar for logical or expressions.
    struct OrBuilder 
        : detail::BinaryBuilder<OrRule, LogicalOr> {};

    /// Group logical or rules.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_or>
      : OrBuilder {};

    /// This is the grammar for logical and expressions.
    struct AndBuilder 
        : detail::BinaryBuilder<AndRule, LogicalAnd> {};

    /// Group logical and rules.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::logical_and>
        : AndBuilder {};

    /// This is the grammar for bitwise or expressions.
    struct BitwiseOrBuilder 
        : detail::BinaryBuilder<BitwiseOrRule, BitwiseOr> {};

    /// Group bitwise or expression grammars.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_or>
        : BitwiseOrBuilder {};

  /// This is the grammar for bitwise and expressions.
    struct BitwiseAndBuilder 
        : detail::BinaryBuilder<BitwiseAndRule, BitwiseAnd> {};

    /// Group all bitwise and rules together.
  template<>
  struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_and>
      : BitwiseAndBuilder {};

/// This is the grammar for bitwise xor expressions.
    struct BitwiseXorBuilder 
        : detail::BinaryBuilder<BitwiseXorRule, BitwiseXor> {};

    /// Group bitwise xor rules together.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::bitwise_xor>
        : BitwiseXorBuilder {};

    /// This is the grammar for array reference expressions.
    struct ArrayRefBuilder : boost::proto::or_<
      boost::proto::when<
        MultiSubscriptRule,
        ArrayRefSequenceTransform(
          boost::proto::_data,
          ConstructAddressFromSequence(
            boost::proto::_data,
            ConstructExpressionGrammar(boost::proto::_left),
            boost::proto::_right))
        >,
      boost::proto::when<
        SubscriptRule,
        ArrayRefIndexTransform(
          boost::proto::_data,
          ConstructExpressionGrammar(boost::proto::_left),
          ConstructExpressionGrammar(boost::proto::_right))
        > 
      >{};

    /// Group subscript expression grammars.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::subscript>
        : ArrayRefBuilder {};

    /// This is the grammar for array address expressions.
    struct ArrayAddressBuilder : boost::proto::or_<
      boost::proto::when<
        MultiSubscriptAddressRule,
        ArrayAddressSequenceTransform(
          boost::proto::_data,
          ConstructAddressFromSequence(
            boost::proto::_data,
            ConstructExpressionGrammar(boost::proto::_left(boost::proto::_left)),
            boost::proto::_right(boost::proto::_left)))
        >,
      boost::proto::when<
        SubscriptAddressRule,
        ArrayAddressIndexTransform(
          boost::proto::_data,
          ConstructExpressionGrammar(boost::proto::_left(boost::proto::_left)),
          ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)))
        > 
      > {};

    /// Group address of rules.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::address_of>
        : ArrayAddressBuilder {};

    /// Group function call expression grammars.
    template<>
    struct ConstructExpressionGrammarCases::case_<boost::proto::tag::function>
        : CallExpressionBuilder {};

  //         ConstructNary<CallRule, Call>

  /// This aggregates all of the expression grammars, making it the
  /// grammar for all expressions.
  struct ConstructExpressionGrammar
    : boost::proto::switch_<ConstructExpressionGrammarCases> {};
  }
}

#endif
