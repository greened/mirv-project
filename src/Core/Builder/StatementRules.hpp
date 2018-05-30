#ifndef mirv_Core_Builder_Statement_hpp
#define mirv_Core_Builder_Statement_hpp

#include <mirv/Core/Builder/AssignRules.hpp>
#include <mirv/Core/Builder/SequenceRules.hpp>
#include <mirv/Core/Builder/IfRules.hpp>
#include <mirv/Core/Builder/IfElseRules.hpp>
#include <mirv/Core/Builder/WhileRules.hpp>
#include <mirv/Core/Builder/DoWhileRules.hpp>
#include <mirv/Core/Builder/CallStatementRules.hpp>

namespace mirv {
   namespace Builder {
    struct StatementRule
      : boost::proto::or_<
      AssignRule,
      SequenceBuilder,
      IfBuilder,
      IfElseBuilder,
      WhileBuilder,
      DoWhileBuilder,
      SubroutineCallStatementBuilder
      > {};
   }
}

#endif
