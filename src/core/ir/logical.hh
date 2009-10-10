#ifndef MIRVLogical_hh
#define MIRVLogical_hh

#include <mirv/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace MIRV {

  class LogicalAnd {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class LogicalOr {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class LogicalNot {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
  };

};

#endif
