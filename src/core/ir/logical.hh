#ifndef mirv_core_ir_logical_hh
#define mirv_core_ir_logical_hh

#include <mirv/core/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for the logical and expression.
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

  /// Specify the interface for the logical or expression.
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

  /// Specify the interface for the logical not expression.
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
