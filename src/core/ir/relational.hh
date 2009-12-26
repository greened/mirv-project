#ifndef mirv_core_ir_relational_hh
#define mirv_core_ir_relational_hh

#include <mirv/core/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  class LessThan {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  class LessThanOrEqual {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  class Equal {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  class NotEqual {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  class GreaterThanOrEqual {
  private:
    typedef boost::mpl::vector<Relational, Transitive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

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
