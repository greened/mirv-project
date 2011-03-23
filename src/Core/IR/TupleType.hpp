#ifndef mirv_Core_IR_TupleType_hpp
#define mirv_Core_IR_TupleType_hpp

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
  /// A tuple type.  Multidimensional tuples are represented by tuples
  /// of tuples.
  struct Tuple {
  private:
    typedef Symbol<Type<Derived> > InterfaceBaseType;

    class Interface : public InterfaceBaseType,
                      public boost::enable_shared_from_this<Symbol<Type<Tuple> > > {
    private:
      ptr<Expression<Base> >::type multiplier;

      // If all the members are the same, remove all but one and add
      // a multiplier.
      void construct_optimized(ptr<Symbol<Type<TypeBase> > >::const_type t,
                               std::uint64_t count);

      template<typename InputIterator>
      void construct(InputIterator start, InputIterator end) {
        // See if we can optimize this.
        if (start == end) {
          return;
        }

        ptr<Symbol<Type<TypeBase> > >::const_type t = *start;
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
        // We can optimize.
        construct_optimized(t, count);
      }

    public:
      typedef Symbol<Type<TypeBase> > ChildType;
      typedef ptr<ChildType>::const_type ConstChildPtr;

      Interface(ConstChildPtr ElementType,
                ptr<Expression<Base> >::type count) :
          InterfaceBaseType(), multiplier(count) {
        push_back(ElementType);
      }

      template<typename InputIterator>
      Interface(InputIterator start, InputIterator end) 
          : InterfaceBaseType() {
        construct(start, end);
      }

      template<typename Sequence>
      Interface(const Sequence &members) : InterfaceBaseType() {
        // Add the member types.
        typedef std::vector<ConstChildPtr> ChildList;
        ChildList temp;
        boost::fusion::for_each(members,
                                boost::bind(
                                  static_cast<void (ChildList::*)(const ConstChildPtr &)>(
                                      &ChildList::push_back),
                                  &temp,
                                  _1));
        construct(temp.begin(), temp.end());
      }

      /// Construct a tuple type with a single member.
      Interface(ptr<Symbol<Type<TypeBase> > >::const_type member)
          : InterfaceBaseType() {
        push_back(member);
      }

      /// Construct a tuple type with no members.
      Interface() : InterfaceBaseType() {}

      ptr<Symbol<Type<TypeBase> > >::const_type
      elementType(ptr<Expression<Base> >::const_type) const;

      BitSizeType bitsize(void) const;

      bool isUniform(void) const {
        return multiplier;
      }

      ptr<Expression<Base> >::const_type
      count(void) const {
        return multiplier;
      }

      ptr<Node<Base>>::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<Node<Base>>::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }
    };

  public:
    typedef Interface BaseType;
    typedef Symbol<Type<Derived> > VisitorBaseType;
  };
}

#endif
