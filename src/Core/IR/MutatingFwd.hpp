#ifndef mirv_core_ir_mutating_fwd_hh
#define mirv_core_ir_mutating_fwd_hh

#include <mirv/Core/IR/StatementFwd.hpp>

namespace mirv {
  class DualExpression;
  class Phi;
  class Store;
  class Call;
  class Allocate;

  namespace detail {
    class DualExpressionInterface;

    /// Define the visitation base type for block statements.
    template<>
    struct VisitorBaseTypeOfStatement<DualExpression> {
      typedef Statement<Controlled> VisitorBaseType;
    };
    /// Define the base type of block statements.
    template<>
    struct BaseTypeOfStatement<DualExpression> {
      typedef DualExpressionInterface BaseType;
    };
 
    class PhiInterface;

    /// Define the visitation base type for block statements.
    template<>
    struct VisitorBaseTypeOfStatement<Phi> {
      typedef Statement<Controlled> VisitorBaseType;
    };
    /// Define the base type of block statements.
    template<>
    struct BaseTypeOfStatement<Phi> {
      typedef StatementBaseGenerator<PhiInterface, Phi, Mutating>::type BaseType;
    };

    class StoreInterface;

    /// Define the visitation base type for block statements.
    template<>
    struct VisitorBaseTypeOfStatement<Store> {
      typedef Statement<DualExpression> VisitorBaseType;
    };
    /// Define the base type of block statements.
    template<>
    struct BaseTypeOfStatement<Store> {
      typedef StatementBaseGenerator<StoreInterface, Store, Mutating>::type BaseType;
    };

    class CallInterface;

    /// Define the visitation base type for block statements.
    template<>
    struct VisitorBaseTypeOfStatement<Call> {
      typedef Statement<Controlled> VisitorBaseType;
    };
    /// Define the base type of block statements.
    template<>
    struct BaseTypeOfStatement<Call> {
      typedef StatementBaseGenerator<CallInterface, Call, Mutating>::type BaseType;
    };

    class AllocateInterface;

    /// Define the visitation base type for block statements.
    template<>
    struct VisitorBaseTypeOfStatement<Allocate> {
      typedef Statement<DualExpression> VisitorBaseType;
    };
    /// Define the base type of block statements.
    template<>
    struct BaseTypeOfStatement<Allocate> {
      typedef StatementBaseGenerator<AllocateInterface, Allocate, Mutating>::type BaseType;
    };
  }
}

#endif
