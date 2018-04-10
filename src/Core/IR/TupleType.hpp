#ifndef mirv_Core_IR_TupleType_hpp
#define mirv_Core_IR_TupleType_hpp

#include <mirv/Core/IR/TupleTypeFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/lexical_cast.hpp>

#include <numeric>
#include <vector>
#include <iterator>
#include <algorithm>

namespace mirv {
  struct Tuple;

  namespace detail {
    class TupleInterface : public Symbol<Type<Derived> >,
                           public boost::enable_shared_from_this<Symbol<Type<Tuple> > > {
    private:
      typedef Symbol<Type<Derived> > BaseType;

      ptr<Expression<Base> > multiplier;

      // If all the members are the same, remove all but one and add
      // a multiplier.
      void construct_optimized(ptr<const Symbol<Type<TypeBase> > > t,
                               std::uint64_t count);

      template<typename InputIterator>
      void construct(InputIterator start, InputIterator end) {
        // See if we can optimize this.
        if (start == end) {
          return;
        }

        ptr<const Symbol<Type<TypeBase> > > t = *start;
        std::uint64_t count = 0;
        for (auto i = start; i != end; ++i) {
          if (*i != t) {
            while (start != end) {
              push_back(*start++);
            }
            return;
          }
          ++count;
        }
        if (count == 1) {
          push_back(*start);
          return;
        }

        // We can optimize.
        construct_optimized(t, count);
      }

    public:
      typedef Symbol<Type<TypeBase> > ChildType;
      typedef ptr<const ChildType> ConstChildPtr;

      TupleInterface(ConstChildPtr ElementType,
                     ptr<Expression<Base> > count) :
          BaseType(), multiplier(count) {
        push_back(ElementType);
      }

      template<typename InputIterator>
      TupleInterface(InputIterator start, InputIterator end) 
          : BaseType() {
        construct(start, end);
      }

      /// Construct a tuple type with a single member.
      TupleInterface(ptr<const Symbol<Type<TypeBase> > > member)
          : BaseType() {
        push_back(member);
      }

      /// Construct a tuple type with no members.
      TupleInterface() : BaseType() {}

      ptr<const Symbol<Type<TypeBase> > >
      elementType(ptr<const Expression<Base> >) const;

      BitSizeType bitsize(void) const;

      bool isUniform(void) const {
        return multiplier != nullptr;
      }

      ptr<const Expression<Base> >
      count(void) const {
        return multiplier;
      }

      ptr<Node<Base>> getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<const Node<Base>> getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }
    };
  }

  /// A tuple type.  Multidimensional tuples are represented by tuples
  /// of tuples.
  struct Tuple {};
}

#endif
