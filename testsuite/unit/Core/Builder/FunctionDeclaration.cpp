// Test building of function declaration.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl foo void (int32)
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl a int32
// STDOUT:       {
// STDOUT:
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Domain.hpp>
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

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::float_;

int main(void)
{
  ptr<Node<Base> >::type code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        func["foo"].type[void_(int_(32))],
        func["testfunc"].type[void_()] [
  	  var["a"].type[int_(32)]
        ]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
