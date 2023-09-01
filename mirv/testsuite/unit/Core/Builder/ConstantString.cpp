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

// Test building of string constants.
//
// STDOUT: mdef testmodule {
// STDOUT:    gvdecl __str1__ (cref int64 7 x int8) cref (cref int64 7 x int8) "i = %d\n"
// STDOUT:    fdecl printf int32 (int8 *, ...)
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl i int32 *
// STDOUT:       vdecl __ct0__ int32
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref i
// STDOUT:             cref int64 1
// STDOUT:          store
// STDOUT:             vref i
// STDOUT:             cref int32 0
// STDOUT:          call
// STDOUT:             fref printf
// STDOUT:             vref __ct0__
// STDOUT:             &
// STDOUT:                cref (cref int64 7 x int8) * & __str1__
// STDOUT:                cref int32 0
// STDOUT:                cref int32 0
// STDOUT:             load
// STDOUT:                vref i
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ConstantGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
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
using Builder::vararg;

int main(void)
{
  Builder::VariableTerminal i = {{"i"}};
  Builder::FunctionTerminal printf_ = {{"printf"}};

  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        func["printf"].type[int_(32)(*int_(8), vararg)],
        func["testfunc"].type[void_()] [
          var[i].type[int_(32)],
          i = 0,
          printf_("i = %d\n", i)
        ]
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
