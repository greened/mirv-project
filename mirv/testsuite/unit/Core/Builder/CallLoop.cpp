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

// Test building of function calls in statement context.
//
// STDOUT: doWhile
// STDOUT:    {
// STDOUT:       call
// STDOUT:          fref foo
// STDOUT:    }
// STDOUT:    <
// STDOUT:       load
// STDOUT:          cref int32 * & a
// STDOUT:       load
// STDOUT:          cref int32 * & b

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
using mirv::GlobalVariable;
using mirv::IRBuilder;
using mirv::Type;
using mirv::FunctionType;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::do_;

int main(void)
{
  auto module = IRBuilder::GetOrCreateModule("testmodule");

  auto functype = IRBuilder::getFunctionType(FunctionType::NotVararg,
                                             IRBuilder::getVoidType());

  auto inttype = IRBuilder::getIntegerType(32);

  auto function = IRBuilder::GetOrCreateFunction("testfunc", functype);

  IRBuilder::GetOrCreateFunction("foo", functype);

  IRBuilder::GetOrCreateGlobalVariable("a", inttype);
  IRBuilder::GetOrCreateGlobalVariable("b", inttype);

  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  auto code =
    Builder::translateWithGrammar<Builder::ConstructStatementGrammar>(
      module,
      function,
      do_[
        foo()
      ].while_(a < b)
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
