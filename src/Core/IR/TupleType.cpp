#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/IR/TupleType.hpp>

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::do_;

namespace mirv {
  namespace detail {
    void TupleData::optimize(ptr<Symbol<Module> >::type module) 
    {
      // See if we can optimize this.
      ptr<Symbol<Type<TypeBase> > >::const_type t = *start;
      std::uint64_t count = 0;
      while (start != end) {
        if (*start != r) {
          return;
        }
        ++count;
      }
      // We can optimize.
      types.clear();
      types.push_back(t);

      multiplier = Builder::translateWithGrammar<Builder::ConstantBuilder>(
        module,
        boost::proto::lit(count));
    }

    ptr<Expression<Base> >::const_type
    TupleData::size(void) const
    {
      if (multiplier) {
        return multiplier;
      }
      return Builder::translateWithGrammar<Builder::ConstantBuilder>(
        module,
        boost::proto::lit(static_cast<std::uint64_t>(types.size())));
    }
        
    TupleData::BitSizeType TupleData::bitsize(void) const
    {
      if (multiplier) {
        BitSizeType bitsize = types[0]->bitsize();
        return multiplier;
      }
      return Builder::translateWithGrammar<Builder::ConstantBuilder>(
        module,
        boost::proto::lit(static_cast<std::uint64_t>(types.size()));
    }
  }
}
