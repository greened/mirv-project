#ifndef mirv_Core_Builder_MakeExpression_hpp
#define mirv_Core_Builder_MakeExpression_hpp

#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>

#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/Builder/Translate.hpp>

namespace mirv {
  namespace Builder {
    namespace {
      ptr<ValueProducer>
      makeIntegralConstant(std::uint64_t constant,
                           ptr<Module> module) {
        checkInvariant(module, "No module available!");
        auto IntType = IRBuilder::getIntegerType(64);
        ptr<Constant> symbol =
          IRBuilder::getIntegerConstant(IntType, constant);
        return symbol;
      }
    }

    ptr<ValueProducer>
    makeExpression(std::int8_t constant, ptr<Module> module) {
      return makeIntegralConstant(constant, module);
    }

    ptr<ValueProducer>
    makeExpression(std::uint8_t constant, ptr<Module> module) {
      return makeIntegralConstant(constant, module);
    }

    ptr<ValueProducer>
    makeExpression(std::int16_t constant, ptr<Module> module) {
      return makeIntegralConstant(constant, module);
    }

    ptr<ValueProducer>
    makeExpression(std::uint16_t constant, ptr<Module> module) {
      return makeIntegralConstant(constant, module);
    }

    ptr<ValueProducer>
    makeExpression(std::int32_t constant, ptr<Module> module) {
      return makeIntegralConstant(constant, module);
    }

    ptr<ValueProducer>
    makeExpression(std::uint32_t constant, ptr<Module> module) {
      return makeIntegralConstant(constant, module);
    }

    ptr<ValueProducer>
    makeExpression(std::int64_t constant, ptr<Module> module) {
      return makeIntegralConstant(constant, module);
    }

    ptr<ValueProducer>
    makeExpression(std::uint64_t constant, ptr<Module> module) {
      return makeIntegralConstant(constant, module);
    }

    ptr<ValueProducer>
    makeExpression(float constant, ptr<Module> module) {
      return safe_cast<ValueProducer>(
        translateWithGrammar<ConstructExpressionGrammar>(
          module, boost::proto::lit(constant)));
    }

    ptr<ValueProducer>
    makeExpression(double constant, ptr<Module> module) {
      return safe_cast<ValueProducer>(
        translateWithGrammar<ConstructExpressionGrammar>(
          module, boost::proto::lit(constant)));
    }
  }
}

#endif
