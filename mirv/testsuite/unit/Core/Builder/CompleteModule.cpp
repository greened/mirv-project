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

// Test building of variables.
//
// STDOUT: mdef testmodule {
// STDOUT:    gvdecl a int32
// STDOUT:    gvdecl b int32
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl c int32 *
// STDOUT:       vdecl d int32 *
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref c
// STDOUT:             cref int64 1
// STDOUT:          allocate int32
// STDOUT:             vref d
// STDOUT:             cref int64 1
// STDOUT:          doWhile
// STDOUT:             {
// STDOUT:                store
// STDOUT:                   cref int32 * & a
// STDOUT:                   +
// STDOUT:                      load
// STDOUT:                         cref int32 * & a
// STDOUT:                      load
// STDOUT:                         cref int32 * & b
// STDOUT:                ifElse
// STDOUT:                   >
// STDOUT:                      load
// STDOUT:                         cref int32 * & b
// STDOUT:                      load
// STDOUT:                         vref c
// STDOUT:                   {
// STDOUT:                      store
// STDOUT:                         cref int32 * & a
// STDOUT:                         +
// STDOUT:                            load
// STDOUT:                               cref int32 * & a
// STDOUT:                            load
// STDOUT:                               vref d
// STDOUT:                   }
// STDOUT:                   {
// STDOUT:                      store
// STDOUT:                         cref int32 * & a
// STDOUT:                         +
// STDOUT:                            load
// STDOUT:                               cref int32 * & a
// STDOUT:                            load
// STDOUT:                               vref c
// STDOUT:                   }
// STDOUT:             }
// STDOUT:             <
// STDOUT:                load
// STDOUT:                   cref int32 * & a
// STDOUT:                load
// STDOUT:                   vref c
// STDOUT:          store
// STDOUT:             vref c
// STDOUT:             load
// STDOUT:                cref int32 * & a
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/Builder/Builder.hpp>
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
using Builder::if_;

int main(void)
{
  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};
  Builder::VariableTerminal d = {{"d"}};

  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
	var[a].type[int_(32)],
	var[b].type[int_(32)],

        func["testfunc"].type[void_()] [
          var[c].type[int_(32)],
          var[d].type[int_(32)],

          do_[
            a = a + b,
            if_(b > c) [
              a = a + d
            ].else_[
              a = a + c
            ]
          ].while_(a < c),
          c = a
        ]
      ]
    );

  mirv::PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
