// Test building of variables.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a int32 *
// STDOUT:    {
// STDOUT:       allocate int32
// STDOUT:          vref a
// STDOUT:          cref int64 1
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
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/Builder/FunctionGrammar.hpp>
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

int main(void)
{
  ptr<Symbol<Module> > module = make<Symbol<Module> >("testmodule");

  ptr<Symbol<Type<TypeBase> > > functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >());
  module->typePushBack(functype);

  ptr<Symbol<Type<TypeBase> > > inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  ptr<Node<Base> > code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(
      module, ptr<Symbol<Function> >(),
      func["testfunc"].type["void ()"] [
	var["a"].type["int32"]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
