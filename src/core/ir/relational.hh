#ifndef mirv_core_ir_relational_hh
#define mirv_core_ir_relational_hh

#include <mirv/core/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for less-than expressions.
  class LessThan {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface for less-than-or-equal expressions.
  class LessThanOrEqual {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface for equal expressions.
  class Equal {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface for not-equal expressions.
  class NotEqual {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface for greater-than-or-equal expressions.
  class GreaterThanOrEqual {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface for greater-than expressions.
  class GreaterThan {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };
}

#endif
