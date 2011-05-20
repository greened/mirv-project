// Test building of variables.
//
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
#include <mirv/Core/Builder/ConstantGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Codegen/LLVM/LLVM.hpp>
#include <mirv/Filter/Snapshot/Codegen/LLVM/JIT/JIT.hpp>

#include <iostream>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::Variable;
using mirv::GlobalVariable;
using mirv::Type;
using mirv::TypeBase;
using mirv::Integral;
using mirv::FunctionType;
using mirv::Node;
using mirv::Base;
using mirv::ptr;

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::do_;
using Builder::if_;
using Builder::vararg;

int main(void)
{
  Builder::FunctionTerminal printf_ = {{"printf"}};

  ptr<Symbol<Module> >::type code =
    mirv::safe_cast<Symbol<Module> >(
      Builder::translateWithGrammar<Builder::ModuleBuilder>(
        module["testmodule"] [
          func["printf"].type[int_(32)(*int_(8), vararg)],

          func["testfunc"].type[void_()] [
            printf_("Hello, World!\n")
          ]
        ]
      ));

  print(std::cout, code);

  mirv::LLVMCodegenFilter codegen;
  codegen(code);
  codegen.getModule()->dump();

  mirv::JITAndRun(code, "testfunc");

  return(0);
}
