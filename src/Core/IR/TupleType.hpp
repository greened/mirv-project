#ifndef mirv_Core_IR_TupleType_hpp
#define mirv_Core_IR_TupleType_hpp

#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/iterator/reverse_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/lexical_cast.hpp>

#include <numeric>
#include <vector>
#include <iterator>
#include <algorithm>

namespace mirv {
  namespace detail {
    // TODO: Make this a proper std function object.
    class ToString {
    public:
      typedef std::string result_type;

      std::string operator()(int value) const {
        return boost::lexical_cast<std::string>(value);
      }
    };

    template<typename InputIterator>
    std::string stringizeDimensions(InputIterator start, InputIterator end) {
      std::stringstream result;
      // Print the most significant dimension first.
      std::copy(boost::make_transform_iterator(
                  boost::make_reverse_iterator(end), ToString()),
                boost::make_transform_iterator(
                  boost::make_reverse_iterator(start), ToString()),
                std::ostream_iterator<std::string>(result, ","));
      // Knock off the last delimiter.
      // TODO: There must be a better way.
      std::string resultString = result.str();
      return resultString.substr(0, resultString.length() - 1);
    }

    /// Tuple implementation.  This implementation optimizes for the
    /// common case of uniform element type structures such as arrays.
    class TupleData {
    private:
      typedef std::vector<ptr<Symbol<Type<TypeBase> > >::const_type > TypeList;
      TypeList types;
      ptr<Expression<Base> >::type multiplier;

    public:
      TupleData(ptr<Symbol<Type<TypeBase> > >::const_type type,
                ptr<Expression<Base> >::const_type count) 
          : types(1, type), multiplier(count) {}
      template<typename InputIterator>
      TupleData(InputIterator start, InputIterator end) 
          : types(start, end) {}
      
      void optimize(ptr<Symbol<Module> >::type module);

      ptr<Expression<Base> >::const_type size(void) const;

      typedef int BitSizeType;
      BitSizeType bitsize(void) const;
    }
  }
  
  /// A tuple type.  Multidimensional tuples are represented by tuples
  /// of tuples.
  struct Tuple {
  private:
    typedef Symbol<Type<Derived> > InterfaceBaseType;

    class Interface : public InterfaceBaseType,
                      public boost::enable_shared_from_this<Symbol<Type<Tuple> > > {
    public:
      typedef int DimensionType;

    private:
      typedef std::vector<int> DimensionVector;

      DimensionVector dimensions;

    public:
      typedef Symbol<Type<TypeBase> > ChildType;
      typedef ptr<ChildType>::const_type ConstChildPtr;

      template<typename InputIterator>
      Interface(ConstChildPtr ElementType,
                InputIterator dimensionStart,
                InputIterator dimensionEnd) :
          InterfaceBaseType(ElementType->name() + "["
                            + detail::stringizeDimensions(dimensionStart,
                                                          dimensionEnd)
                            + "]"),
            dimensions(dimensionStart, dimensionEnd) {
        push_back(ElementType);
      }

      typedef DimensionVector::iterator DimensionIterator;
      typedef DimensionVector::const_iterator ConstDimensionIterator;

      typedef DimensionVector::reverse_iterator ReverseDimensionIterator;
      typedef DimensionVector::const_reverse_iterator
      ConstReverseDimensionIterator;

      ConstChildPtr getElementType(void) const {
        return(front());
      }

      /// Get the start of the dimension sequence.
      DimensionIterator dimensionBegin(void) {
        return(dimensions.begin());
      }
      /// Get the start of the dimension sequence.
      ConstDimensionIterator dimensionBegin(void) const {
        return(dimensions.begin());
      }

      /// Get the start of the reversed dimension sequence.
      ReverseDimensionIterator dimensionRBegin(void) {
        return(dimensions.rbegin());
      }
      /// Get the start of the reversed dimension sequence.
      ConstReverseDimensionIterator dimensionRBegin(void) const {
        return(dimensions.rbegin());
      }

      /// Get the end of the dimension sequence.
      DimensionIterator dimensionEnd(void) {
        return(dimensions.end());
      }
      /// Get the end of the dimension sequence.
      ConstDimensionIterator dimensionEnd(void) const {
        return(dimensions.end());
      }

      /// Get the end of the reversed dimension sequence.
      ReverseDimensionIterator dimensionREnd(void) {
        return(dimensions.rend());
      }
      /// Get the end of the reversed dimension sequence.
      ConstReverseDimensionIterator dimensionREnd(void) const {
        return(dimensions.rend());
      }

      /// Add a dimension.
      void dimensionPushBack(DimensionType d) {
        dimensions.push_back(d);
      }

      typedef DimensionVector::size_type size_type;
      size_type dimensionSize(void) const {
        return dimensions.size();
      }

      BitSizeType bitsize(void) const {
        return(std::accumulate(dimensionBegin(), dimensionEnd(),
                               getElementType()->bitsize(),
                               std::multiplies<size_type>()));
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

    template<typename InputIterator>
    static std::string
    getName(ptr<Symbol<Type<TypeBase> > >::const_type elementType,
            InputIterator start,
            InputIterator end) {
      return elementType->name()
        + "["
        + detail::stringizeDimensions(start, end)
        + "]";
    }
  };
}

#endif
