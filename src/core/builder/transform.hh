#ifndef mirv_core_builder_transform_hh
#define mirv_core_builder_transform_hh

#include <mirv/core/mem/heap.hh>
#include <mirv/core/builder/make.hh>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
      // Transform a one-operand node
     template<typename NodeType,
	      typename Child = typename NodeType::child_ptr,
	      typename Dummy = boost::proto::callable>
      struct ConstructUnary : boost::proto::callable {
	typedef typename ptr<NodeType>::type result_type;

	result_type operator()(Child child) {
	  return make<NodeType>(child);
	}
      };

      // Transform a two-operand node
      template<typename NodeType,
	       typename Child1 = typename NodeType::child_ptr,
	       typename Child2 = typename NodeType::child_ptr,
	       typename Dummy = boost::proto::callable>
      struct ConstructBinary : boost::proto::callable {
	typedef typename ptr<NodeType>::type result_type;

	result_type operator()(Child1 left, Child2 right) {
	  return make<NodeType>(left, right);
         }
      };

#if 0
      // Transform a two-operand temporary node
      template<typename NodeType, typename Dummy = boost::proto::callable>
      struct ConstructBinaryTemporary
            : public boost::proto::callable {
         template <typename Sig>
         struct result;

         template<typename This, typename Expr, typename State, typename Visitor>
         struct result<This(Expr, State, Visitor)> {
            typedef typename boost::proto::left<
               typename Grammar::apply<Expr, State, Visitor>::type>::type
            left_type;

            typedef typename boost::proto::right<
               typename Grammar::apply<Expr, State, Visitor>::type>::type
            right_type;
            typedef struct temp {
               left_type left;
               right_type right;

               temp(const left_type &lft,
                    const right_type &rgt)
                     : left(lft), right(rgt) {};
            } type;
         };

         template<typename Expr, typename State, typename Visitor>
         static typename result<ConsructBinaryTemporary<NodeType, Dummy>, Expr, State, Visitor>::type
         operator()(Expr const &expr, State const &state, Visitor &visitor) {
            return(typename result<ConstructBinaryTemporary<NodeType, Dummy>, Expr, State, Visitor>::type(
                      boost::proto::left(Grammar::call(expr,state,visitor)),
                      boost::proto::right(Grammar::call(expr,state,visitor))));
         }
      };
#endif

      // Transform a three-operand node
      template<typename NodeType,
	       typename Child1 = typename NodeType::child_ptr,
	       typename Child2 = typename NodeType::child_ptr,
	       typename Child3 = typename NodeType::child_ptr,
	       typename Dummy = boost::proto::callable>
      struct ConstructTernary : boost::proto::callable {
	typedef typename ptr<NodeType>::type result_type;

	result_type operator()(Child1 child1, Child2 child2, Child3 child3) {
	  return make<NodeType>(child1, child2, child3);
         }
      };

#if 0
      // Transform a three-operand node, where the left operand
      // returns a temporary structure holding the first and second
      // operands
      template<typename NodeType, typename Dummy = boost::proto::callable>
      struct ConstructTernaryLeftNested
            : public boost::proto::callable {
         template <typename Sig>
         struct result;

         template<typename This, typename Expr, typename State, typename Visitor>
         struct result<This(Expr, State, Visitor)> {
            typedef typename boost::proto::left<
               typename Grammar::apply<Expr, State, Visitor>::type>::type
            left_type;

            typedef Ptr<NodeType>::type type;
         };

         template<typename Expr, typename State, typename Visitor>
         static typename apply<Expr, State, Visitor>::type
         call(Expr const &expr, State const &state, Visitor &visitor) {
            left_type temp(boost::proto::left(Grammar::call(expr,state,visitor)));
            return(typename apply<Expr, State, Visitor>::type(
                      new NodeType(
                         temp.left,
                         temp.right,
                         boost::proto::right(
                            Grammar::call(expr,state,visitor)))));
         }
      };
#endif

#if 0
      // Transform for an n-ary expression
      template<typename NodeType, typename Dummy = boost::proto::callable>
      struct ConstructNary
            : boost::proto::callable {
         template<typename Sig>
         struct result;

         template<typename This, typename Expr, typename State, typename Visitor>
         struct result<This(Expr, State, Visitor)> {
            typedef Ptr<NodeType>::type type;
         };

         template<typename Expr, typename State, typename Visitor>
         struct add_arg {

         private:
            typedef  typename result<ConstructNary<NodeType, Dummy>, Expr, State, Visitor>::type call_type;

            typedef typename Grammar::apply<Expr, State, Visitor>::type
            transformed_type;

         public:
            add_arg(call_type &c,
                    transformed_type &e)
                  : call(c), transformed_expr(e) {};
            template<typename T>
            void operator()(T) {
               call->addOperand(
                  boost::proto::arg<T::value>(transformed_expr));
            }

         private:
            call_type &call;
            transformed_type &transformed_expr;
         };

         template<typename Expr, typename State, typename Visitor>
         static typename result<ConstructNary<NodeType, Dummy>, Expr, State, Visitor>::type
         operator()(Expr const &expr, State const &state, Visitor &visitor) {
            typedef typename result<ConstructNary<NodeType, Dummy>, Expr, State, Visitor>::type ptr_type;

            ptr_type result(new NodeType(
                               // Function
                               boost::proto::_arg0));

            // Call FCall::addOperand for each operand
            typedef boost::mpl::range_c<
            int,
               1,
               boost::proto::arity_of<Expr>::value> args;
            boost::mpl::for_each<args>(
               add_arg<Expr, State, Visitor>(result, transformed_expr));
            return(result);
         }
      };
#endif
   }
}

#endif
