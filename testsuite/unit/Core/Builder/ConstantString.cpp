// Test building of string constants.
//
// STDOUT: mdef testmodule {
// STDOUT:    gvdecl __str1__ (cref int64 7 x int8) cref (cref int64 7 x int8) "i = %d\n"
// STDOUT:    fdecl printf int32 (int8 *, ...)
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl i int32 *
// STDOUT:       vdecl __ct0__ int32
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref i
// STDOUT:             cref int64 1
// STDOUT:          store
// STDOUT:             vref i
// STDOUT:             cref int32 0
// STDOUT:          call
// STDOUT:             fref printf
// STDOUT:             vref __ct0__
// STDOUT:             &
// STDOUT:                cref (cref int64 7 x int8) * & __str1__
// STDOUT:                cref int32 0
// STDOUT:                cref int32 0
// STDOUT:             load
// STDOUT:                vref i
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ConstantGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
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
using Builder::vararg;

int main(void)
{
  Builder::VariableTerminal i = {{"i"}};
  Builder::FunctionTerminal printf_ = {{"printf"}};

  ptr<Node<Base> > code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        func["printf"].type[int_(32)(*int_(8), vararg)],
        func["testfunc"].type[void_()] [
          var[i].type[int_(32)],
          i = 0,
          printf_("i = %d\n", i)
        ]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
