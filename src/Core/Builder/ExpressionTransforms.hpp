#ifndef mirv_Core_Builder_ExpressionTransforms_hpp
#define mirv_Core_Builder_ExpressionTransforms_hpp

#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/ReferenceFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/SymbolTableFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    struct ExpressionTransform : boost::proto::callable {
      typedef ptr<Expression<Base> > result_type;
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
      result_type operator()(ptr<const SymbolTable> symtab,
			     const std::string &name);
    };

    struct ConstantRefTransform : ExpressionTransform {
      typedef ptr<Expression<Reference<Constant<Base> > > > result_type;
      result_type operator()(ptr<const SymbolTable> symtab,
			     ptr<Symbol<Constant<Base> > > constant);
    };

    struct ArrayRefSequenceTransform : ExpressionTransform {
      result_type operator()(ptr<const SymbolTable> symtab,
			     ptr<Expression<Base> > address);
    };

    struct ArrayRefIndexTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Expression<Base> > base,
                             ptr<Expression<Base> > index);
    };

    struct ArrayAddressSequenceTransform : ExpressionTransform {
      result_type operator()(ptr<const SymbolTable> symtab,
			     ptr<Expression<Base> > address);
    };

    struct ArrayAddressIndexTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Expression<Base> > base,
                             ptr<Expression<Base> > index);
    };
  }
}

#endif
