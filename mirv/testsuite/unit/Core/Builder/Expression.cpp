// Test building of expressions.
//
//
// STDOUT: +
// STDOUT:    load
// STDOUT:       vref a
// STDOUT:    /
// STDOUT:       *
// STDOUT:          -
// STDOUT:             load
// STDOUT:                cref int32 * & b
// STDOUT:             load
// STDOUT:                vref c
// STDOUT:          load
// STDOUT:             vref d
// STDOUT:       neg
// STDOUT:          load
// STDOUT:             vref e

#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Allocate;
using mirv::Module;
using mirv::Function;
using mirv::Symbol;
using mirv::FunctionType;
using mirv::GlobalVariable;
using mirv::IRBuilder;
using mirv::Type;
using mirv::Add;
using mirv::Subtract;
using mirv::Multiply;
using mirv::Divide;
using mirv::Negate;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

int main(void)
{
  auto module = IRBuilder::GetOrCreateModule("testmodule");

  auto functype = IRBuilder::getFunctionType(FunctionType::NotVararg,
                                             IRBuilder::getVoidType());
  auto inttype = IRBuilder::getIntegerType(32);

  auto ptrtype = IRBuilder::getPointerType(inttype);

  auto function = IRBuilder::GetOrCreateFunction("testfunc", functype);

  IRBuilder::getOrCreateAllocate("a", ptrtype);
  IRBuilder::GetOrCreateGlobalVariable("b", inttype);
  IRBuilder::getOrCreateAllocate("c", ptrtype);
  IRBuilder::getOrCreateAllocate("d", ptrtype);
  IRBuilder::getOrCreateAllocate("e", ptrtype);

  Builder::VariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};
  Builder::VariableTerminal d = {{"d"}};
  Builder::VariableTerminal e = {{"e"}};

  auto expr =
    Builder::translateWithGrammar<Builder::ConstructExpressionGrammar>(
      module, function, a + (b - c) * d / -e);

  PrintFilter print(std::cout);

  print.run(expr);

  return(0);
}
