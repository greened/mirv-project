#ifndef mirv_core_ir_type_hh
#define mirv_core_ir_type_hh

#include <vector>
#include <numeric>

namespace mirv {
   template<typename Tag>
   class Type {
   public:
     typedef typename Tag::base_type base_type;
   };

   class TypeBase {
   private:
     typedef Symbol<Base> interface_base_type;

   public:

      class interface : public interface_base_type {
      public:
	typedef int size_type;
	virtual size_type bitsize(void) const = 0;
      };

      typedef interface base_type;
   };

  template<>
  class Symbol<Type<TypeBase> > : public Type<TypeBase>::base_type {};

  typedef LeafImpl<Symbol<Type<TypeBase> > > LeafType;
  typedef InnerImpl<Symbol<Type<TypeBase> >, Symbol<Type<TypeBase> > > InnerType;

   class Simple {
   public:
      typedef LeafType base_type;
   };

  template<int Size>
   struct Integral {
    typedef Symbol<Type<Simple> > interface_base_type;
   public:

      class interface : public interface_base_type {
      public:
	size_type bitsize(void) const { return Size; }
      };

      typedef interface base_type;
   };

   struct Floating {
   public:
     typedef Symbol<Type<Simple> > base_type;
   };

   struct Derived {
   public:
      typedef InnerType base_type;
   };

   struct Array {
   private:
     typedef Symbol<Type<Derived> > interface_base_type;

   public:
      class interface
            : public interface_base_type {
      public:
         typedef int dimension_type;

      private:
         typedef std::vector<int> dimension_vector;

         dimension_vector dimensions;

      public:
	typedef Symbol<Type<TypeBase> > child_type;
	typedef ptr<child_type>::type child_ptr;
         typedef ptr<child_type>::const_type const_child_ptr;

         typedef dimension_vector::iterator dimensionIterator;
         typedef dimension_vector::const_iterator constDimensionIterator;

         void setElementType(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         }

         child_ptr getElementType(void) {
            return(front());
         }

         const_child_ptr getElementType(void) const {
            return(front());
         }

         dimensionIterator dimensionBegin(void) {
            return(dimensions.begin());
         }
         constDimensionIterator dimensionBegin(void) const {
            return(dimensions.begin());
         }

         dimensionIterator dimensionEnd(void) {
            return(dimensions.end());
         }
         constDimensionIterator dimensionEnd(void) const {
            return(dimensions.end());
         }

         void dimensionPushBack(dimension_type d) {
            dimensions.push_back(d);
         }

         int bitsize(void) const {
            return(std::accumulate(dimensionBegin(), dimensionEnd(),
                                   getElementType()->bitsize(),
                                   std::multiplies<size_type>()));
	 }
      };
   };

   struct Pointer {
   private:
     typedef Symbol<Type<Derived> > interface_base_type;

   public:
      class interface
            : public interface_base_type {
      public:
	typedef Symbol<Type<TypeBase> > child_type;
	typedef ptr<child_type>::type child_ptr;
         typedef ptr<child_type>::const_type const_child_ptr;

         void setBaseType(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         }

         child_ptr getBaseType(void) {
            return(front());
         }

         const_child_ptr getBaseType(void) const {
            return(front());
         }
      };
   };
}

#endif
