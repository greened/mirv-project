// Test printing of satements.
//
// STDOUT: ifThen
// STDOUT:    <
// STDOUT:       load
// STDOUT:          vref a
// STDOUT:       load
// STDOUT:          vref c
// STDOUT:    {
// STDOUT:       doWhile
// STDOUT:          {
// STDOUT:             store
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   load
// STDOUT:                      vref a
// STDOUT:                   load
// STDOUT:                      cref int32 * & b
// STDOUT:             ifElse
// STDOUT:                >
// STDOUT:                   load
// STDOUT:                      cref int32 * & b
// STDOUT:                   load
// STDOUT:                      vref c
// STDOUT:                {
// STDOUT:                   store
// STDOUT:                      vref a
// STDOUT:                      +
// STDOUT:                         load
// STDOUT:                            vref a
// STDOUT:                         load
// STDOUT:                            cref int32 * & b
// STDOUT:                }
// STDOUT:                {
// STDOUT:                   store
// STDOUT:                      vref a
// STDOUT:                      +
// STDOUT:                         load
// STDOUT:                            vref a
// STDOUT:                         load
// STDOUT:                            vref c
// STDOUT:                }
// STDOUT:          }
// STDOUT:          <
// STDOUT:             load
// STDOUT:                vref a
// STDOUT:             load
// STDOUT:                vref c
// STDOUT:    }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/IfGrammar.hpp>
#include <mirv/Core/Builder/WhileGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Allocate;
using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::GlobalVariable;
using mirv::IRBuilder;
using mirv::Type;
using mirv::FunctionType;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::if_;
using Builder::do_;
using Builder::while_;

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

  Builder::VariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};

  auto stmt =
    Builder::translateWithGrammar<Builder::ConstructStatementGrammar>(
      module,
      function,
      while_(a < c) [
	a = a + b,
        if_(b > c) [
          a = a + b
        ].else_[
          a = a + c
        ]
      ]
    );

  PrintFilter print(std::cout);

  print.run(stmt);

  return(0);
}
