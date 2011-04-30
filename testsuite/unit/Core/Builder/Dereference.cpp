// Test building of array types.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl a (int32, (int32, float32, (int32, float32)), int16) * *
// STDOUT:       vdecl b int32 *
// STDOUT:       {
// STDOUT:          allocate (int32, (int32, float32, (int32, float32)), int16) *
// STDOUT:             vref a
// STDOUT:             cref int64 1
// STDOUT:          allocate int32
// STDOUT:             vref b
// STDOUT:             cref int64 1
// STDOUT:          assign
// STDOUT:             tref
// STDOUT:                vref b
// STDOUT:                cref int64 0
// STDOUT:             cref int32 1
// STDOUT:          assign
// STDOUT:             tref
// STDOUT:                vref b
// STDOUT:                cref int64 0
// STDOUT:             +
// STDOUT:                tref
// STDOUT:                   vref b
// STDOUT:                   cref int64 0
// STDOUT:                tref
// STDOUT:                   tref
// STDOUT:                      tref
// STDOUT:                         tref
// STDOUT:                            tref
// STDOUT:                               vref a
// STDOUT:                               cref int64 0
// STDOUT:                            cref int32 0
// STDOUT:                         cref int32 1
// STDOUT:                      cref int32 2
// STDOUT:                   cref int32 0
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
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

using Builder::module;
using Builder::func;
using Builder::var;
using Builder::void_;
using Builder::int_;
using Builder::float_;
using Builder::struct_;

int main(void)
{
  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};

  ptr<Node<Base> >::type code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        struct_["A"] [
          int_(32),
          struct_["B"] [
            int_(32),
            float_(32),
            struct_["C"] [
              int_(32),
              float_(32)
            ]
          ],
          int_(16)
        ],
        func["testfunc"].type[void_()] [
          var["a"].type[*struct_["A"]],
          var["b"].type[int_(32)],

          b = 1,
          b = b + a[0][1][2][0]
        ]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
