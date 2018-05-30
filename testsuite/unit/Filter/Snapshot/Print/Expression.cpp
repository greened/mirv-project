// Test printing of expressions.
//
//
// STDOUT: +
// STDOUT:    vref a
// STDOUT:    /
// STDOUT:       *
// STDOUT:          -
// STDOUT:             vref b
// STDOUT:             vref c
// STDOUT:          vref d
// STDOUT:       neg
// STDOUT:          vref e

#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Add;
using mirv::Divide;
using mirv::Multiply;
using mirv::Subtract;
using mirv::Negate;
using mirv::PrintFilter;
using mirv::IRBuilder;

int main(void)
{
  auto type = IRBuilder::getIntegerType(32);

  auto a = IRBuilder::GetOrCreateGlobalVariable("a", type);
  auto b = IRBuilder::GetOrCreateGlobalVariable("b", type);
  auto c = IRBuilder::GetOrCreateGlobalVariable("c", type);
  auto d = IRBuilder::GetOrCreateGlobalVariable("d", type);
  auto e = IRBuilder::GetOrCreateGlobalVariable("e", type);

   // a + (b - c) * d / -e)
   auto expr =
     IRBuilder::get<Add>(a,
       IRBuilder::get<Divide>(
         IRBuilder::get<Multiply>(
           IRBuilder::get<Subtract>(b, c),
           d),
         IRBuilder::get<Negate>(e)));

   PrintFilter print(std::cout);

   print.run(expr);

   return(0);
}
