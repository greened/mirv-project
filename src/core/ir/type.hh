#ifndef mirv_core_ir_type_hh
#define mirv_core_ir_type_hh

#include <mirv/core/ir/symbol.hh>

#include <vector>
#include <numeric>

namespace mirv {
   struct SymbolVisitor;

   template<typename Tag>
   class Type {
   public:
     typedef typename Tag::base_type base_type;
     typedef typename Tag::visitor_base_type visitor_base_type;
   };

   class TypeBase {
   private:
     typedef Symbol<Base> interface_base_type;

   public:

      class interface : public interface_base_type {
      public:
	typedef int BitSizeType;
	virtual BitSizeType bitsize(void) const = 0;
      };

      typedef interface base_type;
     typedef Symbol<Base> visitor_base_type;
   };

  //  template<>
  //class Symbol<Type<TypeBase> > : public Type<TypeBase>::base_type {};

  class LeafType : public LeafImpl<Symbol<Type<TypeBase> > > {
  public:
    typedef Symbol<Type<TypeBase> > visitor_base_type;
  };
  class InnerType : public InnerImpl<Symbol<Type<TypeBase> >, VisitedInherit1<SymbolVisitor>::apply<Symbol<Type<TypeBase> > >::type> {
  public:
    typedef Symbol<Type<TypeBase> > visitor_base_type;
  };

   class Simple {
   public:
      typedef LeafType base_type;
     typedef LeafType visitor_base_type;
   };

  struct IntegralBase {
  public:
    typedef Symbol<Type<Simple> > base_type;
    typedef Symbol<Type<Simple> > visitor_base_type;
  };

  template<int Size>
   struct Integral {
    typedef Symbol<Type<IntegralBase> > interface_base_type;
   public:

      class interface : public interface_base_type {
      public:
	BitSizeType bitsize(void) const { return Size; }
      };

      typedef interface base_type;
    typedef Symbol<Type<IntegralBase> > visitor_base_type;
  };

   struct FloatingBase {
   public:
     typedef Symbol<Type<Simple> > base_type;
     typedef Symbol<Type<Simple> > visitor_base_type;
   };

  template<int Size>
   struct Floating {
    typedef Symbol<Type<FloatingBase> > interface_base_type;
   public:

    class interface : public interface_base_type {
    public:
      BitSizeType bitsize(void) const { return Size; }
    };

    typedef interface base_type;
    typedef Symbol<Type<FloatingBase> > visitor_base_type;
  };

   struct Derived {
   public:
      typedef InnerType base_type;
     typedef InnerType visitor_base_type;
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

         BitSizeType bitsize(void) const {
            return(std::accumulate(dimensionBegin(), dimensionEnd(),
                                   getElementType()->bitsize(),
                                   std::multiplies<size_type>()));
	 }
      };
     typedef interface base_type;
     typedef Symbol<Type<Derived> > visitor_base_type;
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
     typedef interface base_type;
     typedef Symbol<Type<Derived> > visitor_base_type;
   };
}

#endif
