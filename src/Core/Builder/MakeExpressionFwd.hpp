#ifndef mirv_Core_Builder_MakeExpressionFwd_hpp
#define mirv_Core_Builder_MakeExpressionFwd_hpp

#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>

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
    makeExpression(const Expr &expr, ptr<Symbol<Module> >::type module);

    template<typename Expr>
    ptr<Expression<Base> >::type
    makeExpression(const Expr &expr);
  }
}

#endif
