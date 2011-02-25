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
  namespace detail {
    // TODO: Make this a proper std function object.
    class TypeName {
    public:
      typedef std::string result_type;

      std::string operator()(ptr<Symbol<Type<TypeBase> > >::const_type type) const {
        return type->name();
      }
    };

    std::string stringize(ptr<Expression<Base> >::type expr);

    template<typename InputIterator>
    std::string stringize(InputIterator start, InputIterator end) {
      // If all the types are the same, optimize it.
      bool same = true;
      int count = 0;
      ptr<Symbol<Type<TypeBase> > >::const_type type = *start;
      for (InputIterator i = start; i != end; ++i) {
        ++count;
        if (type != *i) {
          same = false;
          break;
        }
      }

      if (same) {
        return "(" + type->name() + " x " + "i64 "
          + boost::lexical_cast<std::string>(count) + ")";
      }

      std::stringstream result;

      result << "(";

      // Print the most significant dimension first.
      std::copy(boost::make_transform_iterator(end, TypeName()),
                boost::make_transform_iterator(start, TypeName()),
                std::ostream_iterator<std::string>(result, ","));

      // Knock off the last delimiter.
      // TODO: There must be a better way.
      std::string resultString = result.str();
      return resultString.substr(0, resultString.length() - 1) + ")";
    }
  }
  
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
          InterfaceBaseType("(" + ElementType->name() + " x "
                            + detail::stringize(count)
                            + ")"),
            multiplier(count) {
        push_back(ElementType);
      }

      template<typename InputIterator>
      Interface(InputIterator start, InputIterator end) 
          : InterfaceBaseType(detail::stringize(start, end)) {
        construct(start, end);
      }

      template<typename Sequence>
      Interface(const std::string &name, const Sequence &members)
          : InterfaceBaseType(name) {
        // Add the member types.
        typedef std::vector<ConstChildPtr> ChildList;
        ChildList temp;
        boost::fusion::for_each(members,
                                boost::bind(
                                  static_cast<void (ChildList::*)(
                                    const ChildPtr &)>(
                                      &std::vector<ConstChildPtr>::push_back),
                                  temp,
                                  _1));
        construct(temp.begin(), temp.end());
      }

      /// Construct a tuple type with a single member.
      Interface(const std::string &name,
                ptr<Symbol<Type<TypeBase> > >::const_type member)
          : InterfaceBaseType(name) {
        push_back(member);
      }

      /// Construct a tuple type with no members.
      Interface(const std::string &name) : InterfaceBaseType(name) {}

      BitSizeType bitsize(void) const;

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

    static std::string
    getName(ptr<Symbol<Type<TypeBase> > >::const_type elementType,
            ptr<Expression<Base> >::type count) {
      return "(" + elementType->name() + " x "
        + detail::stringize(count) + ")";
    }

    template<typename InputIterator>
    static std::string
    getName(InputIterator start, InputIterator end) {
      return detail::stringize(start, end);
    }

    static std::string getName(std::string &name) {
      return name;
    }

    template<typename List>
    static std::string getName(std::string &name, List) {
      return getName(name);
    }
  };
}

#endif
