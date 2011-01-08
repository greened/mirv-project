#ifndef mirv_Core_IR_ArrayType_hpp
#define mirv_Core_IR_ArrayType_hpp

#include <mirv/Core/IR/Type.hpp>

#include <boost/iterator/reverse_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/lexical_cast.hpp>

#include <numeric>
#include <vector>

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
  }
  
  /// An array type.  Array types are true multidimensional concepts.
   struct Array {
   private:
     typedef Symbol<Type<Derived> > InterfaceBaseType;

     class Interface : public InterfaceBaseType,
                       public boost::enable_shared_from_this<Symbol<Type<Array> > > {
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
             dimensions(dimensionStart, dimensionEnd) {}
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

	virtual void accept(mirv::SymbolVisitor &) {
	  error("Array::accept called");
	}
	virtual void accept(mirv::ConstSymbolVisitor &) const {
	  error("Array::accept called");
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
