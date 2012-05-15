#ifndef mirv_Core_Builder_VoidTypeGrammar_hpp
#define mirv_Core_Builder_VoidTypeGrammar_hpp

#include <mirv/Core/Builder/VoidTypeRules.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/transform.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to construct a void type.
    struct ConstructVoidType : boost::proto::callable {
      typedef ptr<Symbol<Type<TypeBase> > > result_type;
      result_type operator()(void) {
	return result_type();
      }
    };

    /// This is the grammar for void types.
    struct VoidBuilder : boost::proto::when<
      VoidTerminal,
      ConstructVoidType()
      > {};

     namespace {
       /// A void_ "operator."  This is a protoized object that
       /// implements the function operator to construct function types
       /// with no return value.
       const VoidTerminal void_ = {{}};
     }
  }
}

#endif
