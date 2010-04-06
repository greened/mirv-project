// Test building of variables.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a int32
// STDOUT:    vdecl b int32
// STDOUT:    vdecl c int32
// STDOUT:    {
// STDOUT:       doWhile
// STDOUT:          {
// STDOUT:             assign
// STDOUT:                vref a
// STDOUT:                +
// STDOUT:                   vref a
// STDOUT:                   vref b
// STDOUT:             ifElse
// STDOUT:                >
// STDOUT:                   vref b
// STDOUT:                   vref c
// STDOUT:                {
// STDOUT:                   assign
// STDOUT:                      vref a
// STDOUT:                      +
// STDOUT:                         vref a
// STDOUT:                         vref b
// STDOUT:                }
// STDOUT:                {
// STDOUT:                   assign
// STDOUT:                      vref a
// STDOUT:                      +
// STDOUT:                         vref a
// STDOUT:                         vref c
// STDOUT:                }
// STDOUT:          }
// STDOUT:          <
// STDOUT:             vref a
// STDOUT:             vref c
// STDOUT:       assign
// STDOUT:          vref c
// STDOUT:          vref a
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

using Builder::var;
using Builder::function;
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
    Builder::translate(module,
      function["testfunc"].type[void_()] [
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
