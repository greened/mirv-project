#ifndef mirv_core_ir_statement_fwd_hh
#define mirv_core_ir_statement_fwd_hh

#include <mirv/Core/IR/Inherit.hpp>
#include <mirv/Core/IR/NodeFwd.hpp>
#include <mirv/Core/Filter/ConstStatementVisitorFwd.hpp>
#include <mirv/Core/Filter/StatementVisitorFwd.hpp>

namespace mirv {
  template<typename Tag> class Statement;

  class InnerStatementBase;
  class InnerStatement;
  class LeafStatement;

  class Conditional;
  class Iterative;
  class Mutating;

  class Controlled;
  class SingleExpression;

  namespace detail {
    template<typename Tag>
    struct VisitorBaseTypeOfStatement;
    
    template<typename Tag>
    struct BaseTypeOfStatement;

    template<typename Tag>
    struct VisitorBaseTypeOf<Statement<Tag> > {
      typedef typename VisitorBaseTypeOfStatement<Tag>::VisitorBaseType VisitorBaseType;
    };

    template<typename Tag>
    struct BaseTypeOf<Statement<Tag> > {
      typedef typename BaseTypeOfStatement<Tag>::BaseType BaseType;
    };

    /// Define the visitation base type for base statements.
    template<>
    struct VisitorBaseTypeOfStatement<Base> {
      typedef Node<Base> VisitorBaseType;
    };
    /// Define the base type of base statements.
    template<>
    struct BaseTypeOfStatement<Base> {
      typedef Node<Base> BaseType;
    };

    class InnerStatementTraits;

    /// Define the visitation base type for base inner statements.
    template<>
    struct VisitorBaseTypeOfStatement<Inner<InnerStatementTraits> > {
      typedef Statement<Base> VisitorBaseType;
    };
    /// Define the base type of base inner statements.
    template<>
    struct BaseTypeOfStatement<Inner<InnerStatementTraits> > {
      typedef InnerInterface<InnerStatementTraits> BaseType;
    };

    /// Define the visitation base type for base inner statements.
    template<>
    struct VisitorBaseTypeOf<InnerStatementBase> {
      typedef Statement<Base> VisitorBaseType;
    };
    /// Define the base type of base inner statements.
    template<>
    struct BaseTypeOf<InnerStatementBase> {
      typedef Statement<Inner<detail::InnerStatementTraits> > BaseType;
    };

    /// Define the visitation base type for inner statements.
    template<>
    struct VisitorBaseTypeOf<InnerStatement> {
      typedef Statement<Base> VisitorBaseType;
    };
    /// Define the base type of inner statements.
    template<>
    struct BaseTypeOf<InnerStatement> {
      typedef InnerImpl<Statement<Base>, Virtual<InnerStatementBase> > BaseType;
    };

    /// Define the visitation base type for leaf statements.
    template<>
    struct VisitorBaseTypeOf<LeafStatement> {
      typedef Statement<Base> VisitorBaseType;
    };
    /// Define the base type of leaf statements.
    template<>
    struct BaseTypeOf<LeafStatement> {
      typedef LeafImpl<Virtual<Statement<Base> > > BaseType;
    };

    /// Define the visitation base type for conditional statements.
    template<>
    struct VisitorBaseTypeOfStatement<Conditional> {
      typedef Statement<Base> VisitorBaseType;
    };
    /// Define the base type of conditional statements.
    template<>
    struct BaseTypeOfStatement<Conditional> {
      typedef Virtual<Statement<Base> > BaseType;
    };

    /// Define the visitation base type for iterative statements.
    template<>
    struct VisitorBaseTypeOfStatement<Iterative> {
      typedef Statement<Base> VisitorBaseType;
    };
    /// Define the base type of iterative statements.
    template<>
    struct BaseTypeOfStatement<Iterative> {
      typedef Virtual<Statement<Base> > BaseType;
    };

    /// Define the visitation base type for mutating statements.
    template<>
    struct VisitorBaseTypeOfStatement<Mutating> {
      typedef Statement<Base> VisitorBaseType;
    };
    /// Define the base type of mutating statements.
    template<>
    struct BaseTypeOfStatement<Mutating> {
      typedef Virtual<Statement<Base> > BaseType;
    };

    class ControlledInterface;

    /// Define the visitation base type for controlled statements.
    template<>
    struct VisitorBaseTypeOfStatement<Controlled> {
      typedef Statement<Base> VisitorBaseType;
    };
    /// Define the base type of controlled statements.
    template<>
    struct BaseTypeOfStatement<Controlled> {
      typedef ControlledInterface BaseType;
    };

    class SingleExpressionInterface;

    /// Define the visitation base type for single expression statements.
    template<>
    struct VisitorBaseTypeOfStatement<SingleExpression> {
      typedef Statement<Controlled> VisitorBaseType;
    };
    /// Define the base type of single expression statements.
    template<>
    struct BaseTypeOfStatement<SingleExpression> {
      typedef SingleExpressionInterface BaseType;
    };
  }

  /// This is a metafunction to generate a scattered base class
  /// hierarchy of property statements.  The Sequence is a sorted
  /// list of property tags and Root is the base type of the whole
  /// hierarchy.
  template<typename Root, typename Tag, typename ...Property>
  class StatementBaseGenerator {
    template<typename ...BaseProperty>
    class StatementBase 
        : public Root,
          public Statement<BaseProperty>... {
    public:
      StatementBase(void) : Root() {}
      template<typename ...Args>
        StatementBase(const Args &...args) : Root(args...) {}
      virtual void accept(StatementVisitor &) {
        error("StatementBase::accept called!");
      }
      virtual void accept(ConstStatementVisitor &) const {
        error("StatementBase::accept called!");
      }
    };

  public:
    typedef StatementBase<Property...> type;
  };
}

#endif
