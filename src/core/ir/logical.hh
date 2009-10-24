#ifndef mirv_core_ir_logical_hh
#define mirv_core_ir_logical_hh

#include <mirv/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  class LogicalAnd {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef BinaryExpression visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class LogicalOr {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef BinaryExpression visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class LogicalNot {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef UnaryExpression visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
  };

};

#endif
