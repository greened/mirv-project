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
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a int32 *
// STDOUT:    {
// STDOUT:       allocate int32
// STDOUT:          vref a
// STDOUT:          cref int64 1
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/FunctionGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::IRBuilder;
using mirv::Type;
using mirv::FunctionType;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::func;
using Builder::int_;
using Builder::void_;

int main(void)
{
  auto module = IRBuilder::GetOrCreateModule("module");

  IRBuilder::getFunctionType(FunctionType::NotVararg,
                             IRBuilder::getVoidType());

  IRBuilder::getIntegerType(32);

  auto code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(
      module, ptr<Function>(),
      func["testfunc"].type[void_()] [
        var["a"].type[int_(32)]
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
