#ifndef mirv_Core_Builder_ModuleTransform_hpp
#define mirv_Core_Builder_ModuleTransform_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/Fusion.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <boost/proto/proto.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <iterator>
#include <algorithm>

namespace mirv {
  namespace Builder {
    /// Transform a one-operand node into a single-child IR node.
    struct ConstructModule : boost::proto::callable {
      typedef ptr<Module> result_type;

      result_type operator()(ptr<const SymbolTable> symtab, std::string Name) {
        return IRBuilder::GetOrCreateModule(std::move(Name));
      }
    };
  }
}

#endif
