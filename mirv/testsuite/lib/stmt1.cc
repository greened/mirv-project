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

// Test printing of statements

int main(void)
{
   MIRV::Symbol<Variable> a("a", MIRV::Symbol<Type<Integral<32> > >());
   MIRV::Symbol<Variable> b("b", MIRV::Symbol<Type<Integral<32> > >());
   MIRV::Symbol<Variable> c("c", MIRV::Symbol<Type<Integral<32> > >());
   MIRV::Symbol<Variable> d("d", MIRV::Symbol<Type<Integral<32> > >());
   MIRV::Symbol<Variable> e("e", MIRV::Symbol<Type<Integral<32> > >());

   MIRV::ptr<MIRV::BaseStatement>::type stmt =
      MIRV::block [
         a = 32,
         b = -10,
         c = 2,
         d = 47,
         e = -34,

         if (a + b > 10) [
            c = c + 1,
            e = e - d * b
         ]
         .else[
            b = 0
         ],

         while(a > 0) [
            d = d << 1,
            a = a - 1
         ]
      ];

   MIRV::PrintFilter print(std::cout);

   print(*stmt);

   return(0);
}
