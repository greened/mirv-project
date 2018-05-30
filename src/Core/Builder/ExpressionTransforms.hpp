#ifndef mirv_Core_Builder_ExpressionTransforms_hpp
#define mirv_Core_Builder_ExpressionTransforms_hpp

#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/SymbolTableFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    struct ExpressionTransform : boost::proto::callable {
      typedef ptr<ValueProducer> result_type;
    };

    struct VariableRefTransform : ExpressionTransform {
      result_type operator()(ptr<const SymbolTable> symtab,
			     const std::string &name);
    };

    struct GlobalVariableRefTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable> symtab,
			     const std::string &name);
    };

    struct FunctionRefTransform : ExpressionTransform {
      typedef ptr<Function> result_type;
      result_type operator()(ptr<const SymbolTable> symtab,
         		     const std::string &name);
    };

    struct ConstantRefTransform : ExpressionTransform {
      typedef ptr<Constant> result_type;
      result_type operator()(ptr<const SymbolTable> symtab,
			     ptr<Constant> constant);
    };

    struct ArrayRefSequenceTransform : ExpressionTransform {
      result_type operator()(ptr<const SymbolTable> symtab,
			     ptr<ValueProducer> address);
    };

    struct ArrayRefIndexTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<ValueProducer> base,
                             ptr<ValueProducer> index);
    };

    struct ArrayAddressSequenceTransform : ExpressionTransform {
      result_type operator()(ptr<const SymbolTable> symtab,
			     ptr<ValueProducer> address);
    };

    struct ArrayAddressIndexTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<ValueProducer> base,
                             ptr<ValueProducer> index);
    };
  }
}

#endif
