#ifndef mirv_Core_IR_Visitable_hpp
#define mirv_Core_IR_Visitable_hpp

#include <mirv/Core/Filter/NodeVisitor.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>

namespace mirv {
  namespace detail {
    template<typename Op>
    struct VisitorBase {
      typedef typename Op::VisitorBaseType VisitorBaseType;
    };
    template<typename Op>
    struct BaseTypeOf {
      typedef typename Op::BaseType BaseType;
    };

    template<typename Tag>
    struct VisitorBase<Expression<Tag> > {
      typedef typename Tag::VisitorBaseType VisitorBaseType;
    };
    template<typename Tag>
    struct BaseTypeOf<Expression<Tag> > {
      typedef typename Tag::BaseType BaseType;
    };

    template<typename Tag>
    struct VisitorBase<Statement<Tag> > {
      typedef typename Tag::VisitorBaseType VisitorBaseType;
    };
    template<typename Tag>
    struct BaseTypeOf<Statement<Tag> > {
      typedef typename Tag::BaseType BaseType;
    };

    template<typename Tag>
    struct VisitorBase<Symbol<Tag> > {
      typedef typename Tag::VisitorBaseType VisitorBaseType;
    };
    template<typename Tag>
    struct BaseTypeOf<Symbol<Tag> > {
      typedef typename Tag::BaseType BaseType;
    };
  }

  /// Enable a class hierarchy to be visited.  This exists as a
  /// separate capability class so that it may be specialized for
  /// different kinds of nodes.
  template<
    typename NodeType,
    typename Visitor,
    typename Base = typename detail::BaseTypeOf<NodeType>::BaseType
    >
  class Visitable : public Base {
  private:
    typedef Base BaseType;

  public:
    Visitable(void) : BaseType() {}
    template<typename ...Arg>
    Visitable(const Arg &...arg) : BaseType(arg...) {};

    virtual void accept(Visitor &V) {
      detail::AcceptImpl<NodeType,
        boost::is_base_of<
          boost::enable_shared_from_this<NodeType>,
          NodeType
          >::value
      > impl;
      impl(safe_cast<NodeType>(this), V);
    }
  };
}

#endif
