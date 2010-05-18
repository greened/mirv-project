// Test building of variables.
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

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/ArrayType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/StructType.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
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

using Builder::var;
using Builder::function;
using Builder::module;
using Builder::void_;
using Builder::int_;

int main(void)
{
  ptr<Node<Base> >::type code =
    Builder::translate(
      module["testmodule"] [
        function["testfunc"].type[void_()] [
  	  var["a"].type[int_(32)]
        ]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
