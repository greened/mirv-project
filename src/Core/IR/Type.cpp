#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  namespace detail {
    SimpleInterface::BitSizeType
    SimpleInterface::bitsize(void) const
    {
      BitSizeType size =
        Builder::makeExpression(bsize, this->parent<Symbol<Module> >());
      return size;
    }

    void
    DerivedInterface::resolve(ptr<const Symbol<Type<Placeholder> > >
                              placeholder,
                              ptr<const Symbol<Type<TypeBase> > >
                              replacement) 
    {  
      for (auto member = begin(); member != end(); ++member) {
        if (*member == placeholder) {
          *member = replacement;
          continue;
        }
        if (*member == replacement) {
          continue;
        }
        boost::const_pointer_cast<Symbol<Type<TypeBase> > >((*member))->
          resolve(placeholder, replacement);
      }
    }
  }
}
