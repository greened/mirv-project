#ifndef MIRVLogical_hh
#define MIRVLogical_hh

#include <mirv/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace MIRV {

  class LogicalAnd {
  private:
    typedef boost::mpl::vector<LogicalSemantic, CommutativeSemantic,
			       AssociativeSemantic, TransitiveSemantic,
			       ReflexiveSemantic, LogAndSemantic> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class LogicalOr {
  private:
    typedef boost::mpl::vector<LogicalSemantic, CommutativeSemantic,
			       AssociativeSemantic, TransitiveSemantic,
			       ReflexiveSemantic, LogOrSemantic> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class Not {
  private:
    typedef boost::mpl::vector<LogicalSemantic, CommutativeSemantic,
			       AssociativeSemantic, TransitiveSemantic,
			       ReflexiveSemantic, LogNotSemantic> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
  };

};

#endif
