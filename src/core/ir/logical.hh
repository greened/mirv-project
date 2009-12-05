#ifndef mirv_core_ir_logical_hh
#define mirv_core_ir_logical_hh

#include <mirv/core/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  class LogicalAnd {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  class LogicalOr {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  class LogicalNot {
  private:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Unary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Unary> >::type base_type;
  };

};

#endif
