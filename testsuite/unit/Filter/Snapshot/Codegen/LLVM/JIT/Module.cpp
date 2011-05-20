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
  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};
  Builder::VariableTerminal d = {{"d"}};

  Builder::FunctionTerminal printf_ = {{"printf"}};

  ptr<Symbol<Module> >::type code =
    mirv::safe_cast<Symbol<Module> >(
      Builder::translateWithGrammar<Builder::ModuleBuilder>(
        module["testmodule"] [
          func["printf"].type[int_(32)(*int_(8), vararg)],

          var[a].type[int_(32)],
          var[b].type[int_(32)],

          func["testfunc"].type[void_()] [
            var[c].type[int_(32)],
            var[d].type[int_(32)],
            a = 0,
            b = 1,
            c = 20,
            d = 3,

            do_[
              a = a + b,
              if_(b > c) [
                a = a + d
              ].else_[
                a = a + c
              ]
            ].while_(a < c)
            //printf_("a = %d, b = %d, c = %d, d= %d\n", a, b, c, d)
          ]
        ]
      ));

  mirv::JITAndRun(code, "testfunc");

  return(0);
}