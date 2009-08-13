#ifndef MIRV_Filter_ExpressionFlow_hh
#define MIRV_Filter_ExpressionFlow_hh

namespace MIRV {
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow>
   class ExpressionFlow
         : public ExpressionVisitor {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeAction bfr;
      AfterAction aft;
      Dataflow data;
      Confluence conf;

   protected:
      EnterAction &enter(void) {
         return(ent);
      };

      EnterAction &leave(void) {
         return(lve);
      };

      EnterAction &before(void) {
         return(bfr);
      };

      EnterAction &after(void) {
         return(aft);
      };

      Dataflow &dataflow(void) {
         return(data);
      }
      Confluence &confluence(void) {
         return(conf);
      }

   public:
      ExpressionFlow(EnterAction &e,
                     LeaveAction &l,
                     BeforeAction &b,
                     AfterAction &a,
                     Dataflow &d,
                     Confluence &c)
            : ent(e), lve(l), bfr(b), aft(a),
                 data(d), conf(c) {}
   };

   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow,
      typename Confluence = Dataflow::Confluence>
   class ForwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      Dataflow,
      Confluence> {
   public:
      ForwardExpressionFlow(EnterAction &e,
                            LeaveAction &l,
                            BeforeAction &b,
                            AfterAction &a,
                            Dataflow &d,
                            Confluence &c)
            : ExpressionFlow(e, l, b, a, expr, d, c) {}

      void visit(BaseExpression &expr) {
         enter()(expr);

         for(BaseExpression::iterator i = expr.begin(),
                iend = expr.end();
             i != iend;
             ;) {
            before()(expr, **i);
            (*i)->accept(*this);
            after()(expr, **i);
            BaseExpression::iterator prev = i;
            if (++i != iend) {
               between()(expr, **prev, **i);
            }
         }

         leave()(expr);
      }
   };

   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow,
      typename Confluence = Dataflow::Confluence>
   class BackwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      Dataflow,
      Confluence> {
   public:
      BackwardExpressionFlow(EnterAction &e,
                             LeaveAction &l,
                             BeforeAction &b,
                             AfterAction &a,
                             Dataflow &d,
                             Confluence &c)
            : ExpressionFlow(e, l, b, a, expr, d, c) {}

      void visit(BaseExpression &expr) {
         enter()(expr);

         for(BaseExpression:reverse_:iterator i = expr.rbegin(),
                iend = expr.rend();
             i != iend;
             ;) {
            before()(expr, **i);
            (*i)->accept(*this);
            after()(expr, **i);
            BaseExpression::iterator prev = i;
            if (++i != iend) {
               between()(expr, **prev, **i);
            }
         }

         leave()(expr);
      }
   };
}

#endif
