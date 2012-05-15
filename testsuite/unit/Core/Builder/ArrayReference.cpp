// Test building of array types.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a (cref int64 5 x (cref int64 4 x (cref int64 3 x int32))) *
// STDOUT:    vdecl b int32 *
// STDOUT:    {
// STDOUT:       allocate (cref int64 5 x (cref int64 4 x (cref int64 3 x int32)))
// STDOUT:          vref a
// STDOUT:          cref int64 1
// STDOUT:       allocate int32
// STDOUT:          vref b
// STDOUT:          cref int64 1
// STDOUT:       store
// STDOUT:          vref b
// STDOUT:          cref int32 1
// STDOUT:       store
// STDOUT:          vref b
// STDOUT:          +
// STDOUT:             load
// STDOUT:                vref b
// STDOUT:             load
// STDOUT:                &
// STDOUT:                   vref a
// STDOUT:                   cref int64 0
// STDOUT:                   cref int32 2
// STDOUT:                   cref int32 0
// STDOUT:                   cref int32 1
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
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/FunctionGrammar.hpp>
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

using Builder::func;
using Builder::var;
using Builder::void_;
using Builder::int_;

int main(void)
{
  ptr<Symbol<Module> > module = make<Symbol<Module> >("testmodule");

  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};

  ptr<Node<Base> > code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(
      module, ptr<Symbol<Function> >(),
      func["testfunc"].type[void_()] [
        var["a"].type[int_(32)[boost::proto::lit(5),4,3]],
        var["b"].type[int_(32)],

        b = 1,
        b = b + a[boost::proto::lit(2),0,1]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
