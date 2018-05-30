#ifndef mirv_Core_Builder_StatementTransforms_hpp
#define mirv_Core_Builder_StatementTransforms_hpp

#include <mirv/Core/Memory/Heap.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  class Control;
  class Block;
  class IfElse;
  class IfThen;
  class DoWhile;
  class ValueProducer;

  namespace Builder {
    class SymbolTable;

    template<typename Stmt>
    struct StatementTransform : boost::proto::callable {
      typedef ptr<Stmt> result_type;
    };

    struct BlockTransform : public StatementTransform<Block> {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<Control> Stmt1,
                             ptr<Control> Stmt2);
    };

    struct IfTransform : public StatementTransform<IfThen> {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<ValueProducer> condition,
                             ptr<Control> body);
    };

    struct IfElseTransform : public StatementTransform<IfElse> {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<ValueProducer> condition,
                             ptr<Control> thenBody,
                             ptr<Control> elseBody);
    };

    struct WhileTransform : public StatementTransform<IfThen> {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<ValueProducer> condition,
                             ptr<Control> body);
    };

    struct DoWhileTransform : public StatementTransform<DoWhile> {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<ValueProducer> condition,
                             ptr<Control> body);
    };

    struct AssignTransform : public StatementTransform<Control> {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<ValueProducer> lhs,
                             ptr<ValueProducer> rhs);
    };
  }
}

#endif
