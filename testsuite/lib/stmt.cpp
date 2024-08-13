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
