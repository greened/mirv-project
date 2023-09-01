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

// Test printing of expressions

#include <ir/symbol.hh>
#include <builder/builder.hh>
#include <filter/print.hh>

using MIRV::Symbol;
using MIRV::Variable;
using MIRV::Type;
using MIRV::Integral;

int main(void)
{
   Symbol<Variable> asym =
      new Symbol<Varialbe>("a", Symbol<Type<Integral<32> > >());
   Symbol<Variable> bsym =
      new Symbol<Varialbe>("b", Symbol<Type<Integral<32> > >());
   Symbol<Variable> csym =
      new Symbol<Varialbe>("c", Symbol<Type<Integral<32> > >());
   Symbol<Variable> dsym =
      new Symbol<Varialbe>("d", Symbol<Type<Integral<32> > >());
   Symbol<Variable> esym =
      new Symbol<Variable>("e", Symbol<Type<Integral<32> > >());

   MIRV::Builder::Symbol a =
      {{ asym }};
   MIRV::Builder::Symbol b =
      {{ bsym }};
   MIRV::Builder::Symbol c =
      {{ csym }};
   MIRV::Builder::Symbol d =
      {{ dsym }};
   MIRV::Builder::Symbol e =
      {{ esym }};

   MIRV::ptr<MIRV::ExpressionBase>::type expr =
      MIRV::build(a + (b - c) * d / -e);

   MIRV::PrintFilter print(std::cout);

   print(*expr);

   return(0);
}
