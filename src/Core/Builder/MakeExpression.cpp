#ifndef mirv_Core_Builder_MakeExpression_hpp
#define mirv_Core_Builder_MakeExpression_hpp

#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Bitwise.hpp>
#include <mirv/Core/IR/Logical.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>

#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/Builder/Translate.hpp>

namespace mirv {
  namespace Builder {
    namespace {
      ptr<Expression<Base> >::type
      makeIntegralConstant(std::uint64_t constant,
                           ptr<Symbol<Module> >::type module)
      {
        checkInvariant(module, "No module available!");
        Symbol<Module>::TypeIterator type =
          module->typeFind("int64");
        checkInvariant(type != module->typeEnd(), "No integer type available!");
        ptr<Symbol<Constant<Base> > >::type symbol =
          mirv::make<Symbol<Constant<std::uint64_t> > >(*type, constant);
        return mirv::make<Expression<Reference<Constant<Base> > > >(symbol);
      }
    }

    ptr<Expression<Base> >::type
    makeExpression(std::int8_t constant, ptr<Symbol<Module> >::type module) 
    {
      return makeIntegralConstant(constant, module);
    }

    ptr<Expression<Base> >::type
    makeExpression(std::uint8_t constant, ptr<Symbol<Module> >::type module) 
    {
      return makeIntegralConstant(constant, module);
    }

    ptr<Expression<Base> >::type
    makeExpression(std::int16_t constant, ptr<Symbol<Module> >::type module) 
    {
      return makeIntegralConstant(constant, module);
    }

    ptr<Expression<Base> >::type
    makeExpression(std::uint16_t constant, ptr<Symbol<Module> >::type module) 
    {
      return makeIntegralConstant(constant, module);
    }

    ptr<Expression<Base> >::type
    makeExpression(std::int32_t constant, ptr<Symbol<Module> >::type module) 
    {
      return makeIntegralConstant(constant, module);
    }

    ptr<Expression<Base> >::type
    makeExpression(std::uint32_t constant, ptr<Symbol<Module> >::type module) 
    {
      return makeIntegralConstant(constant, module);
    }

    ptr<Expression<Base> >::type
    makeExpression(std::int64_t constant, ptr<Symbol<Module> >::type module) 
    {
      return makeIntegralConstant(constant, module);
    }

    ptr<Expression<Base> >::type
    makeExpression(std::uint64_t constant, ptr<Symbol<Module> >::type module) 
    {
      return makeIntegralConstant(constant, module);
    }

    ptr<Expression<Base> >::type
    makeExpression(float constant, ptr<Symbol<Module> >::type module) 
    {
      return safe_cast<Expression<Base> >(
        translateWithGrammar<ConstructExpressionGrammar>(
          module, boost::proto::lit(constant)));
    }

    ptr<Expression<Base> >::type
    makeExpression(double constant, ptr<Symbol<Module> >::type module) 
    {
      return safe_cast<Expression<Base> >(
        translateWithGrammar<ConstructExpressionGrammar>(
          module, boost::proto::lit(constant)));
    }
  }
}

#endif
