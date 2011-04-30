// Test building of function calls combined with another statement.
// STDOUT: fdef testfunc {
// STDOUT:    {
// STDOUT:       assign
// STDOUT:          tref
// STDOUT:             cref int32 * & b
// STDOUT:             cref int64 0
// STDOUT:          tref
// STDOUT:             cref int32 * & a
// STDOUT:             cref int64 0
// STDOUT:       call
// STDOUT:          fref foo
// STDOUT:          tref
// STDOUT:             cref int32 * & b
// STDOUT:             cref int64 0
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
using mirv::GlobalVariable;
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
using Builder::if_;

int main(void)
{
  ptr<Symbol<Module> >::type module = make<Symbol<Module> >("testmodule");

  
  ptr<Symbol<Type<TypeBase> > >::type inttype =
    make<Symbol<Type<Integral> > >(32);
  module->typePushBack(inttype);

  std::vector<ptr<Symbol<Type<TypeBase> > >::type> argTypes;
  argTypes.push_back(inttype);

  ptr<Symbol<Type<TypeBase> > >::type functype =
    make<Symbol<Type<FunctionType> > >(ptr<Symbol<Type<TypeBase> > >::type(),
                                       argTypes.begin(),
                                       argTypes.end());
  module->typePushBack(functype);

  ptr<Symbol<GlobalVariable> >::type asym =
    make<Symbol<GlobalVariable> >("a", inttype);
  module->globalVariablePushBack(asym);

  ptr<Symbol<GlobalVariable> >::type bsym =
    make<Symbol<GlobalVariable> >("b", inttype);
  module->globalVariablePushBack(bsym);

  ptr<Symbol<Function> >::type fsym = make<Symbol<Function> >("foo", functype);
  module->functionPushBack(fsym);

  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  ptr<Node<Base> >::type code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(module,
      func["testfunc"].type[void_()] [
        b = a,
        foo(b)
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
