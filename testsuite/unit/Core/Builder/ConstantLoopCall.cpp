// Test building of constants.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl foo void (int32)
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl i int32
// STDOUT:       {
// STDOUT:          doWhile
// STDOUT:             {
// STDOUT:                call
// STDOUT:                   fref foo
// STDOUT:                   vref i
// STDOUT:                assign
// STDOUT:                   vref i
// STDOUT:                   +
// STDOUT:                      vref i
// STDOUT:                      cref int32 1
// STDOUT:             }
// STDOUT:             <
// STDOUT:                vref i
// STDOUT:                cref int32 10
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/StructType.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::Variable;
using mirv::Type;
using mirv::TypeBase;
using mirv::Integral;
using mirv::FunctionType;
using mirv::Node;
using mirv::Base;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::make;

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::do_;

int main(void)
{
  Builder::VariableTerminal i = {{"i"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  ptr<Node<Base> >::type code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        func["foo"].type[void_(int_(32))],
        func["testfunc"].type[void_()] [
          var["i"].type[int_(32)],

          do_[
            foo(i),
            i = i + 1
          ].while_(i < 10)
        ]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}