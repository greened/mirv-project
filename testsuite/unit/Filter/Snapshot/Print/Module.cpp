// Test printing of modules.
//
// STDOUT: mdef Test {
// STDOUT:    gvdecl c int32
// STDOUT:    fdecl foo void ()
// STDOUT:    fdef foo {
// STDOUT:       vdecl a int32 *
// STDOUT:       vdecl b int32 *
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref a
// STDOUT:             cref int32 1
// STDOUT:          allocate int32
// STDOUT:             vref b
// STDOUT:             cref int32 1
// STDOUT:          doWhile
// STDOUT:             {
// STDOUT:                store
// STDOUT:                   vref a
// STDOUT:                   +
// STDOUT:                      load
// STDOUT:                         vref a
// STDOUT:                      load
// STDOUT:                         vref b
// STDOUT:                ifElse
// STDOUT:                   >
// STDOUT:                      load
// STDOUT:                         vref b
// STDOUT:                      load
// STDOUT:                         &
// STDOUT:                            cref int32 * & c
// STDOUT:                            cref int32 0
// STDOUT:                   {
// STDOUT:                      store
// STDOUT:                         vref a
// STDOUT:                         +
// STDOUT:                            load
// STDOUT:                               vref a
// STDOUT:                            load
// STDOUT:                               vref b
// STDOUT:                   }
// STDOUT:                   {
// STDOUT:                      store
// STDOUT:                         vref a
// STDOUT:                         +
// STDOUT:                            load
// STDOUT:                               vref a
// STDOUT:                            load
// STDOUT:                               &
// STDOUT:                                  cref int32 * & c
// STDOUT:                                  cref int32 0
// STDOUT:                   }
// STDOUT:             }
// STDOUT:             <
// STDOUT:                load
// STDOUT:                   vref a
// STDOUT:                load
// STDOUT:                   &
// STDOUT:                      cref int32 * & c
// STDOUT:                      cref int32 0
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
using mirv::GlobalVariable;
using mirv::IntegerType;
using mirv::PointerType;
using mirv::FunctionType;
using mirv::TupleType;
using mirv::TuplePointer;
using mirv::Constant;
using mirv::Add;
using mirv::LessThan;
using mirv::Load;
using mirv::GreaterThan;
using mirv::Allocate;
using mirv::Block;
using mirv::IfElse;
using mirv::DoWhile;
using mirv::Store;
using mirv::Sequence;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::IRBuilder;

int main(void) {
  auto m = IRBuilder::GetOrCreateModule("Test");

  auto int32type = IRBuilder::getIntegerType(32);
  auto ptrtype = IRBuilder::getPointerType(int32type);
  auto ftype = IRBuilder::getFunctionType(FunctionType::NotVararg,
                                          IRBuilder::getVoidType(),
                                          IRBuilder::getVoidType());

  auto a = IRBuilder::get<Allocate>("a", ptrtype);
  auto b = IRBuilder::get<Allocate>("b", ptrtype);
  auto c = IRBuilder::GetOrCreateGlobalVariable("c", ptrtype);

  auto dowhile =
    IRBuilder::get<DoWhile>(
      IRBuilder::get<LessThan>(a, c),
        IRBuilder::get<Block>(
          IRBuilder::get<Sequence>(
            IRBuilder::get<Store>(
              a,
              IRBuilder::get<Add>(IRBuilder::get<Load>(a),
                                  IRBuilder::get<Load>(b)))),
          IRBuilder::get<IfElse>(
            IRBuilder::get<GreaterThan>(
              IRBuilder::get<Load>(b),
              IRBuilder::get<Load>(
                IRBuilder::get<TuplePointer>(
                  c,
                  IRBuilder::getIntegerConstant(int32type, 0)))),
            IRBuilder::get<Block>(
              IRBuilder::get<Sequence>(
                IRBuilder::get<Store>(
                  a,
                  IRBuilder::get<Add>(
                    IRBuilder::get<Load>(a),
                    IRBuilder::get<Load>(b))))),
            IRBuilder::get<Block>(
              IRBuilder::get<Sequence>(
                IRBuilder::get<Store>(
                  a,
                  IRBuilder::get<Add>(
                    IRBuilder::get<Load>(a),
                    IRBuilder::get<TuplePointer>(
                      c,
                      IRBuilder::getIntegerConstant(int32type, 0)))))))));

  auto block = IRBuilder::get<Block>(dowhile);

  auto f = IRBuilder::GetOrCreateFunction("foo", ftype);

  f->setBlock(block);

  PrintFilter print(std::cout);

  print.run(m);

  return(0);
}
