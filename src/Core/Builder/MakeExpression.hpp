#ifndef mirv_Core_Builder_MakeExpression_hpp
#define mirv_Core_Builder_MakeExpression_hpp

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <cstdint>

namespace mirv {
  namespace Builder {
    ptr<ValueProducer>
    makeExpression(std::int8_t constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(std::uint8_t constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(std::int16_t constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(std::uint16_t constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(std::int32_t constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(std::uint32_t constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(std::int64_t constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(std::uint64_t constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(float constant, ptr<Module> module);

    ptr<ValueProducer>
    makeExpression(double constant, ptr<Module> module);

    template<typename Expr>
    ptr<ValueProducer>
    makeExpression(const Expr &expr, ptr<Module> module) {
      return safe_cast<ValueProducer>(
        translateWithGrammar<ConstructExpressionGrammar>(module, expr));
    }

    template<typename Expr>
    ptr<ValueProducer>
    makeExpression(const Expr &expr) {
      return safe_cast<ValueProducer>(
        translateWithGrammar<ConstructExpressionGrammar>(expr));
    }
  }
}

#endif
