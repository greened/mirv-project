#ifndef mirv_Core_Builder_VoidTypeGrammar_hpp
#define mirv_Core_Builder_VoidTypeGrammar_hpp

#include <mirv/Core/Builder/VoidTypeRules.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/transform.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to construct a void type.
    struct ConstructVoidType : boost::proto::callable {
      typedef ptr<const Type> result_type;
      result_type operator()(void) {
	return IRBuilder::getVoidType();
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
