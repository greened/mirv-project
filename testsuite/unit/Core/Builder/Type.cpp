// Test building of types.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a int32
// STDOUT:    {
// STDOUT: 
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
using Builder::var;
using Builder::void_;
using Builder::int_;

int main(void)
{
  ptr<Symbol<Module> >::type module = make<Symbol<Module> >("testmodule");

  ptr<Node<Base> >::type code =
    Builder::translate(module, ptr<Symbol<Function> >::type(),
      function["testfunc"].type[void_()] [
        var["a"].type[int_(32)]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
