// Test printing of satements.
//
// STDOUT: doWhile
// STDOUT:    {
// STDOUT:       store
// STDOUT:          vref a
// STDOUT:          +
// STDOUT:             load
// STDOUT:                vref a
// STDOUT:             load
// STDOUT:                cref int32 * & b
// STDOUT:       ifElse
// STDOUT:          >
// STDOUT:             load
// STDOUT:                cref int32 * & b
// STDOUT:             load
// STDOUT:                vref c
// STDOUT:          {
// STDOUT:             store
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   load
// STDOUT:                      vref a
// STDOUT:                   load
// STDOUT:                      cref int32 * & b
// STDOUT:          }
// STDOUT:          {
// STDOUT:             store
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   load
// STDOUT:                      vref a
// STDOUT:                   load
// STDOUT:                      vref c
// STDOUT:          }
// STDOUT:    }
// STDOUT:    <
// STDOUT:       load
// STDOUT:          vref a
// STDOUT:       load
// STDOUT:          vref c

#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/DoWhileGrammar.hpp>
#include <mirv/Core/Builder/IfElseGrammar.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Allocate;
using mirv::Block;
using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::FunctionType;
using mirv::GlobalVariable;
using mirv::Sequence;
using mirv::Type;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::IRBuilder;

namespace Builder = mirv::Builder;

using Builder::if_;
using Builder::do_;

int main(void)
{
  ptr<Module> module = IRBuilder::GetOrCreateModule("testmodule");

  ptr<const Type> voidtype = IRBuilder::getVoidType();
  ptr<const Type> functype = IRBuilder::getFunctionType(FunctionType::NotVararg,
                                                        voidtype);

  ptr<const Type> inttype = IRBuilder::getIntegerType(32);

  ptr<const Type> ptrtype = IRBuilder::getPointerType(inttype);

  ptr<Function> function = IRBuilder::GetOrCreateFunction("testfunc", functype);

  function->setBlock(IRBuilder::get<Block>("testblock"));

  ptr<Allocate> asym = IRBuilder::getOrCreateAllocate("a", ptrtype);
  auto aseq = IRBuilder::get<Sequence>(asym);
  function->getBlock()->push_back(aseq);

  IRBuilder::GetOrCreateGlobalVariable("b", inttype);

  ptr<Allocate> csym = IRBuilder::getOrCreateAllocate("c", ptrtype);
  auto cseq = IRBuilder::get<Sequence>(csym);
  function->getBlock()->push_back(cseq);

  Builder::VariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};

  auto stmt =
    Builder::translateWithGrammar<Builder::ConstructStatementGrammar>(
      module, function,
      do_[
	a = a + b,
        if_(b > c) [
          a = a + b
        ].else_[
          a = a + c
        ]
      ].while_(a < c)
    );

  mirv::PrintFilter print(std::cout);

  print.run(stmt);

  return(0);
}
