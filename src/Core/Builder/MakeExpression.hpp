#ifndef mirv_Core_Builder_MakeExpression_hpp
#define mirv_Core_Builder_MakeExpression_hpp

#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <cstdint>

namespace mirv {
  namespace Builder {
    ptr<Expression<Base> >::type
    makeExpression(std::int8_t constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(std::uint8_t constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(std::int16_t constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(std::uint16_t constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(std::int32_t constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(std::uint32_t constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(std::int64_t constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(std::uint64_t constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(float constant, ptr<Symbol<Module> >::type module);

    ptr<Expression<Base> >::type
    makeExpression(double constant, ptr<Symbol<Module> >::type module);

    template<typename Expr>
    ptr<Expression<Base> >::type
    makeExpression(const Expr &expr, ptr<Symbol<Module> >::type module) 
    {
      return safe_cast<Expression<Base> >(
        translateWithGrammar<ConstructExpressionGrammar>(module, expr));
    }

    template<typename Expr>
    ptr<Expression<Base> >::type
    makeExpression(const Expr &expr) 
    {
      return safe_cast<Expression<Base> >(
        translateWithGrammar<ConstructExpressionGrammar>(expr));
    }
  }
}

#endif
