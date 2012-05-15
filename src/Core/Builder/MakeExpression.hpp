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
    ptr<Expression<Base> >
    makeExpression(std::int8_t constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(std::uint8_t constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(std::int16_t constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(std::uint16_t constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(std::int32_t constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(std::uint32_t constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(std::int64_t constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(std::uint64_t constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(float constant, ptr<Symbol<Module> > module);

    ptr<Expression<Base> >
    makeExpression(double constant, ptr<Symbol<Module> > module);

    template<typename Expr>
    ptr<Expression<Base> >
    makeExpression(const Expr &expr, ptr<Symbol<Module> > module) 
    {
      return safe_cast<Expression<Base> >(
        translateWithGrammar<ConstructExpressionGrammar>(module, expr));
    }

    template<typename Expr>
    ptr<Expression<Base> >
    makeExpression(const Expr &expr) 
    {
      return safe_cast<Expression<Base> >(
        translateWithGrammar<ConstructExpressionGrammar>(expr));
    }
  }
}

#endif
