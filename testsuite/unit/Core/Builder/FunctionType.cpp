// Test building of function types.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl testfunc
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl a int32
// STDOUT:       {
// STDOUT:
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Builder/Builder.hpp>
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

using Builder::function;
using Builder::module;
using Builder::var;
using Builder::void_;
using Builder::int_;

int main(void)
{
  ptr<Node<Base> >::type code =
    Builder::translate(
      module["testmodule"] [
        function["testfunc"].type[void_(int_(32), int_(16))] [
          var["a"].type[int_(32)]
        ]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
