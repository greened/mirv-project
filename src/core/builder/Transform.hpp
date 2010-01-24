#ifndef mirv_core_builder_Transform_hpp
#define mirv_core_builder_Transform_hpp

#include <mirv/core/mem/Heap.hpp>
#include <mirv/core/builder/Make.hpp>

#include <boost/proto/proto.hpp>

#include <iterator>
#include <algorithm>

namespace mirv {
   namespace Builder {
      /// Transform a one-operand node into a single-child IR node.
     template<typename NodeType,
	      typename Child = typename NodeType::ChildPtr,
	      typename Dummy = boost::proto::callable>
      struct ConstructUnary : boost::proto::callable {
	typedef typename ptr<NodeType>::type result_type;

	result_type operator()(Child child) {
	  return make<NodeType>(child);
	}
      };

     /// Transform a two-operand node to a two-child IR node.
      template<typename NodeType,
	       typename Child1 = typename NodeType::ChildPtr,
	       typename Child2 = typename NodeType::ChildPtr,
	       typename Dummy = boost::proto::callable>
      struct ConstructBinary : boost::proto::callable {
	typedef typename ptr<NodeType>::type result_type;

	result_type operator()(Child1 left, Child2 right) {
	  return make<NodeType>(left, right);
         }
      };

     /// This is a specialization for block statements to add the
     /// child to a block if it already exists.
     template<>
     struct ConstructBinary<Statement<Block>,
			    Statement<Block>::ChildPtr,
			    Statement<Block>::ChildPtr,
			    boost::proto::callable> : boost::proto::callable {
       typedef ptr<Statement<Block> >::type result_type;

       result_type operator()(Statement<Block>::ChildPtr left,
			      Statement<Block>::ChildPtr right) {
	 if (ptr<Statement<Block> >::type lb =
	     dyn_cast<Statement<Block> >(left)) {
	   if (ptr<Statement<Block> >::type rb =
	       dyn_cast<Statement<Block> >(right)) {
	     std::copy(rb->begin(), rb->end(), std::back_inserter(*lb));
	   }
	   lb->push_back(right);
	   return lb;
	 }
	 else if (ptr<Statement<Block> >::type rb =
		  dyn_cast<Statement<Block> >(right)) {
	   rb->push_front(left);
	   return rb;
	 }
	 return make<Statement<Block> >(left, right);
       }
     };

     /// Transform a three-operand node to a three-child IR node.
     template<typename NodeType,
	      typename Child1 = typename NodeType::ChildPtr,
	      typename Child2 = typename NodeType::ChildPtr,
	      typename Child3 = typename NodeType::ChildPtr,
	      typename Dummy = boost::proto::callable>
     struct ConstructTernary : boost::proto::callable {
       typedef typename ptr<NodeType>::type result_type;

	result_type operator()(Child1 child1, Child2 child2, Child3 child3) {
	  return make<NodeType>(child1, child2, child3);
         }
      };

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
