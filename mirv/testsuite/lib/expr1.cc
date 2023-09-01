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
