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
      typedef ptr<Expression<Base> >::type result_type;
    };

    struct VariableRefTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable>::const_type symtab,
			     const std::string &name);
    };

    struct GlobalVariableRefTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable>::type symtab,
			     const std::string &name);
    };

    struct FunctionRefTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable>::const_type symtab,
			     const std::string &name);
    };

    struct ConstantRefTransform : ExpressionTransform {
      typedef ptr<Expression<Reference<Constant<Base> > > >::type result_type;
      result_type operator()(ptr<SymbolTable>::const_type symtab,
			     ptr<Symbol<Constant<Base> > >::type constant);
    };

    struct ArrayRefSequenceTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable>::const_type symtab,
			     ptr<Expression<Base> >::type address);
    };

    struct ArrayRefIndexTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable>::type symtab,
			     ptr<Expression<Base> >::type base,
                             ptr<Expression<Base> >::type index);
    };

    struct ArrayAddressSequenceTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable>::const_type symtab,
			     ptr<Expression<Base> >::type address);
    };

    struct ArrayAddressIndexTransform : ExpressionTransform {
      result_type operator()(ptr<SymbolTable>::type symtab,
			     ptr<Expression<Base> >::type base,
                             ptr<Expression<Base> >::type index);
    };
  }
}

#endif
