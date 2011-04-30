// Test building of variables.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a int32 *
// STDOUT:    vdecl b int32 *
// STDOUT:    vdecl c int32 *
// STDOUT:    {
// STDOUT:       allocate int32
// STDOUT:          vref a
// STDOUT:          cref int64 1
// STDOUT:       allocate int32
// STDOUT:          vref b
// STDOUT:          cref int64 1
// STDOUT:       allocate int32
// STDOUT:          vref c
// STDOUT:          cref int64 1
// STDOUT:       doWhile
// STDOUT:          {
// STDOUT:             assign
// STDOUT:                tref
// STDOUT:                   vref a
// STDOUT:                   cref int64 0
// STDOUT:                +
// STDOUT:                   tref
// STDOUT:                      vref a
// STDOUT:                      cref int64 0
// STDOUT:                   tref
// STDOUT:                      vref b
// STDOUT:                      cref int64 0
// STDOUT:             ifElse
// STDOUT:                >
// STDOUT:                   tref
// STDOUT:                      vref b
// STDOUT:                      cref int64 0
// STDOUT:                   tref
// STDOUT:                      vref c
// STDOUT:                      cref int64 0
// STDOUT:                {
// STDOUT:                   assign
// STDOUT:                      tref
// STDOUT:                         vref a
// STDOUT:                         cref int64 0
// STDOUT:                      +
// STDOUT:                         tref
// STDOUT:                            vref a
// STDOUT:                            cref int64 0
// STDOUT:                         tref
// STDOUT:                            vref b
// STDOUT:                            cref int64 0
// STDOUT:                }
// STDOUT:                {
// STDOUT:                   assign
// STDOUT:                      tref
// STDOUT:                         vref a
// STDOUT:                         cref int64 0
// STDOUT:                      +
// STDOUT:                         tref
// STDOUT:                            vref a
// STDOUT:                            cref int64 0
// STDOUT:                         tref
// STDOUT:                            vref c
// STDOUT:                            cref int64 0
// STDOUT:                }
// STDOUT:          }
// STDOUT:          <
// STDOUT:             tref
// STDOUT:                vref a
// STDOUT:                cref int64 0
// STDOUT:             tref
// STDOUT:                vref c
// STDOUT:                cref int64 0
// STDOUT:       assign
// STDOUT:          tref
// STDOUT:             vref c
// STDOUT:             cref int64 0
// STDOUT:          tref
// STDOUT:             vref a
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
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/Builder/AssignGrammar.hpp>
#include <mirv/Core/Builder/DoWhileGrammar.hpp>
#include <mirv/Core/Builder/IfElseGrammar.hpp>
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

using Builder::var;
using Builder::func;
using Builder::void_;
using Builder::int_;
using Builder::do_;
using Builder::if_;

int main(void)
{
  ptr<Symbol<Module> >::type module = make<Symbol<Module> >("testmodule");

  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};

  ptr<Node<Base> >::type code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(module,
      func["testfunc"].type[void_()] [
	var["a"].type[int_(32)],
	var["b"].type[int_(32)],
	var["c"].type[int_(32)],

        do_[
          a = a + b,
          if_(b > c) [
            a = a + b
          ].else_[
            a = a + c
          ]
        ].while_(a < c),
        c = a
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
