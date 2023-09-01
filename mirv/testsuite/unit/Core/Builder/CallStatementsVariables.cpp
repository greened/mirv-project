// Copyright 2023 David A. Greene

// This file is part of the MIRV project.

// MIRV is free software: you can redistribute it and/or modify it under the
// terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.

// MIRV is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.

// You should have received a copy of the GNU General Public License along with
// Foobar. If not, see <https://www.gnu.org/licenses/>.

// Test building of function calls combined with another statement and
// variable declarations.
// STDOUT: fdef testfunc {
// STDOUT:    vdecl c int32 *
// STDOUT:    {
// STDOUT:       allocate int32
// STDOUT:          vref c
// STDOUT:          cref int64 1
// STDOUT:       store
// STDOUT:          cref int32 * & b
// STDOUT:          load
// STDOUT:             cref int32 * & a
// STDOUT:       call
// STDOUT:          fref foo
// STDOUT:          load
// STDOUT:             cref int32 * & b
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::IRBuilder;
using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::FunctionType;
using mirv::GlobalVariable;
using mirv::Type;
using mirv::ptr;
using mirv::PrintFilter;

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
  ptr<Module> module = IRBuilder::GetOrCreateModule("testmodule");

  ptr<const Type> inttype = IRBuilder::getIntegerType(32);

  ptr<const Type> functype = IRBuilder::getFunctionType(FunctionType::NotVararg,
                                                        IRBuilder::getVoidType(),
                                                        inttype);

  IRBuilder::GetOrCreateGlobalVariable("a", inttype);
  IRBuilder::GetOrCreateGlobalVariable("b", inttype);

  IRBuilder::GetOrCreateFunction("foo", functype);

  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  auto code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(module,
      func["testfunc"].type[void_()] [
        var[c].type[int_(32)],
        b = a,
        foo(b)
      ]
    );

  mirv::PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
