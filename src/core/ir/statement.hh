#ifndef MIRVStatement_hh
#define MIRVStatement_hh

#include <mirv/ir/node.hh>
#include <mirv/ir/expression.hh>

namespace MIRV {
   template<
      typename Tag,
      typename Base = typename BaseType<Tag>::type>
   class Statement: public Base {
   };

   typedef Statement<Base> BaseStatement;

   typedef Inner<BaseStatement> InnerStatement;
   typedef Leaf<BaseStatement> LeafStatement;

   /// Statement semantics are somehow affected by expressions
   template<typename Stmt>
   class Controlled {
   public:
      class interface;
      typedef interface base_type;
      typedef Stmt interface_base_type;

      class interface
            : public interface_base_type {
      protected:
         typedef ptr<BaseExpression>::type expression_ptr;
         typedef ptr<BaseExpression>::const_type const_expressuib_ptr;
         typedef std::list<expression_ptr> expression_list;

      private:
         expression_list expressions;

      protected:
         typedef expression_list::iterator expression_iterator;
         typedef expression_list::const_iterator const_expression_iterator;

         expression_iterator expression_begin(void) {
            return(expressions.begin());
         };
         const_expression_iterator expression_begin(void) const {
            return(expressions.begin());
         };

         expression_iterator expression_end(void) {
            return(expressions.end());
         };
         const_expression_iterator expression_end(void) const {
            return(expressions.end());
         };

         void expression_push_back(expression_ptr c) {
            expressions.push_back(c);
         };

         expression_ptr expression_front(void) {
            check_invariant(!empty(),
                            "Attempt to get operand from empty node");
            return(expressions.front());
         };

         const_expression_ptr expression_front(void) const {
            check_invariant(!empty(),
                            "Attempt to get operand from empty node");
            return(expressions.front());
         };

         expression_ptr expression_back(void) {
            check_invariant(!empty(),
                            "Attempt to get operand from empty node");
            return(expressions.back());
         };

         const_expression_ptr expression_back(void) const {
            check_invariant(!expression_empty(),
                            "Attempt to get operand from empty node");
            return(expressions.back());
         };

         typedef expression_list::size_type size_type
         size_type expression_size(void) const { return(expressions.size()); };

         bool expression_empty(void) const { return(expressions.empty()); };
      };
   };

   template<typename Stmt>
   class SingleExpression {
   public:
      class interface;
      typedef interface base_type;
      typedef Statement<Controlled<Stmt> > interface_base_type;

      class interface
            : public interface_base_type {
         // Protected because subclasses might want to rename
      protected:
         typedef interface_base_type::expression_ptr expression_ptr;
         typedef interface_base_type::const_expression_ptr
         const_expression_ptr;

         void set_expression(expression_ptr e) {
            if (expression_empty()) {
               expression_push_back(e);
            }
            else {
               *--expression_end() = e;
            }
         };

         expression_ptr get_expression(void) {
            check_invariant(!expression_empty() && *expression_begin(),
                            "Attempt to get non-existent expression");
            return(*expression_begin());
         };

         const_expression_ptr get_expression(void) const {
            check_invariant(!expression_empty() && *expression_begin(),
                            "Attempt to get non-existent expression");
            return(*expression_begin());
         };
      };
   };

   // Statement property semantics

   /// Not all child statements may be executed
   class Conditional {};

   /// Child statements may be executed multiple times
   class Iterative {};

   /// Modifies program state
   class Mutating {};

   // A metafunction class to generate statement hierarchies.  This
   // makes sure that each property statement specifies Base as its
   // base type so that visit(Expreesion<>::base_type) works
   // correctly.
   class StatementGenerator {
   public:
      template<typename Property, typename Base>
      struct apply {
         typedef Statement<Property, Base> type;
      };
   };

   template<typename Sequence, typename Root>
   class StatementBaseGenerator {
   public:
      typedef typename Properties<StatementGenerator, Root, Sequence>::type
      type;
   };
}

#endif
