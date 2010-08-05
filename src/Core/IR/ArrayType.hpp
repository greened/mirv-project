#ifndef mirv_Core_IR_ArrayType_hpp
#define mirv_Core_IR_ArrayType_hpp

#include <mirv/Core/IR/Type.hpp>

#include <numeric>
#include <vector>

namespace mirv {
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
	Interface(const std::string &name) : InterfaceBaseType(name) {}
	typedef Symbol<Type<TypeBase> > ChildType;
	typedef ptr<ChildType>::type ChildPtr;
         typedef ptr<ChildType>::const_type ConstChildPtr;

         typedef DimensionVector::iterator DimensionIterator;
         typedef DimensionVector::const_iterator ConstDimensionIterator;

         typedef DimensionVector::reverse_iterator ReverseDimensionIterator;
         typedef DimensionVector::const_reverse_iterator
         ConstReverseDimensionIterator;

         void setElementType(ChildPtr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         }

         ChildPtr getElementType(void) {
            return(front());
         }
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
   };
}

#endif
