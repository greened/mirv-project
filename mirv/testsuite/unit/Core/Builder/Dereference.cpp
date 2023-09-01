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

// Test building of array types.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl a (int32, (int32, float32, (int32, float32)), int16) * *
// STDOUT:       vdecl b int32 *
// STDOUT:       {
// STDOUT:          allocate (int32, (int32, float32, (int32, float32)), int16) *
// STDOUT:             vref a
// STDOUT:             cref int64 1
// STDOUT:          allocate int32
// STDOUT:             vref b
// STDOUT:             cref int64 1
// STDOUT:          store
// STDOUT:             vref b
// STDOUT:             cref int32 1
// STDOUT:          store
// STDOUT:             vref b
// STDOUT:             +
// STDOUT:                load
// STDOUT:                   vref b
// STDOUT:                load
// STDOUT:                   &
// STDOUT:                      &
// STDOUT:                         &
// STDOUT:                            &
// STDOUT:                               load
// STDOUT:                                  vref a
// STDOUT:                               cref int32 0
// STDOUT:                            cref int64 0
// STDOUT:                            cref int32 1
// STDOUT:                         cref int64 0
// STDOUT:                         cref int32 2
// STDOUT:                      cref int64 0
// STDOUT:                      cref int32 0
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
using mirv::Type;
using mirv::FunctionType;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::module;
using Builder::func;
using Builder::var;
using Builder::void_;
using Builder::int_;
using Builder::float_;
using Builder::struct_;

int main(void)
{
  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};

  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        struct_["A"] [
          int_(32),
          struct_["B"] [
            int_(32),
            float_(32),
            struct_["C"] [
              int_(32),
              float_(32)
            ]
          ],
          int_(16)
        ],
        func["testfunc"].type[void_()] [
          var["a"].type[*struct_["A"]],
          var["b"].type[int_(32)],

          b = 1,
          b = b + a[0][1][2][0]
        ]
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
