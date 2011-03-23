#ifndef mirv_Core_Builder_TransformFwd_hpp
#define mirv_Core_Builder_TransformFwd_hpp

namespace mirv {
  namespace Builder {
    class SymbolTable;
    struct SetModule;
    struct SetFunction;
    template<typename SymbolType, typename Dummy>
    struct LookupSymbol;
    template<typename SymbolType, typename Dummy>
    struct LookupAndAddSymbol;
    struct ClearPendingStatements;
    struct ClearPendingStatementsDoWhile;
    struct ClearPendingStatementsWhileRule;
    template<typename NodeType, typename Child, typename Dummy>
    struct ConstructUnary;
    template<typename NodeType, typename Child1, typename Child2, typename Dummy>
    struct ConstructBinary;
    /// Transform a three-operand node to a three-child IR node.
    template<typename NodeType,
      typename Child1,
      typename Child2,
      typename Child3,
      typename Dummy>
    struct ConstructTernary;
    template<typename ExpressionType>
    class TranslateToExpression;
    template<typename NodeType, typename Dummy>
    struct ConstructNary;
  }
}

#endif
