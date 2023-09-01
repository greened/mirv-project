// Test printing of satements.
//
// STDOUT: {
// STDOUT:    allocate int32
// STDOUT:       vref a
// STDOUT:       cref int32 1
// STDOUT:    allocate int32
// STDOUT:       vref b
// STDOUT:       cref int32 1
// STDOUT:    allocate int32
// STDOUT:       vref c
// STDOUT:       cref int32 1
// STDOUT:    doWhile
// STDOUT:       {
// STDOUT:          store
// STDOUT:             vref a
// STDOUT:             +
// STDOUT:                load
// STDOUT:                   vref a
// STDOUT:                load
// STDOUT:                   vref b
// STDOUT:          ifElse
// STDOUT:             >
// STDOUT:                load
// STDOUT:                   vref b
// STDOUT:                load
// STDOUT:                   vref c
// STDOUT:             {
// STDOUT:                store
// STDOUT:                   vref a
// STDOUT:                   +
// STDOUT:                      load
// STDOUT:                         vref a
// STDOUT:                      load
// STDOUT:                         vref b
// STDOUT:             }
// STDOUT:             {
// STDOUT:                store
// STDOUT:                   vref a
// STDOUT:                   +
// STDOUT:                      load
// STDOUT:                         vref a
// STDOUT:                      load
// STDOUT:                         vref c
// STDOUT:             }
// STDOUT:       }
// STDOUT:       <
// STDOUT:          load
// STDOUT:             vref a
// STDOUT:          load
// STDOUT:             vref c
// STDOUT: }

#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Allocate;
using mirv::DoWhile;
using mirv::LessThan;
using mirv::GreaterThan;
using mirv::Block;
using mirv::Store;
using mirv::IfElse;
using mirv::Sequence;
using mirv::Add;
using mirv::Load;
using mirv::IRBuilder;
using mirv::PrintFilter;

int main(void)
{
  auto int32type = IRBuilder::getIntegerType(32);
  auto ptrtype = IRBuilder::getPointerType(int32type);

  auto a = IRBuilder::get<Allocate>("a", ptrtype);
  auto b = IRBuilder::get<Allocate>("b", ptrtype);
  auto c = IRBuilder::get<Allocate>("c", ptrtype);

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
              IRBuilder::get<Load>(c)),
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
                    IRBuilder::get<Load>(c))))))));

  PrintFilter print(std::cout);

  print.run(dowhile);

  return(0);
}
