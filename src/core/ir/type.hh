#ifndef MIRVType_hh
#define MIRVType_hh

namespace MIRV {
   template<
      typename Tag,
      typename Base = typename BaseType<Tag>::type>
   class Type
         : public Base {
   public:
   };

   class TypeBase {
   private:
      typedef BaseSymbol interface_base_type;

   public:
      typedef int size_type;

      class interface
            : public interface_base_type {
      public:
         virtual size_type size(void) const = 0;
      };

      typedef interface base_type;
   };

   typedef Type<TypeBase> BaseType;
   typedef Leaf<BaseType> LeafType;
   typedef Inner<BaseType> InnerType;

   class Simple {
   private:
      typedef LeafType interface_base_type;

   public:
      class interface
            : public interface_base_type {
      private:
         int sz;

      public:
         interface(int s)
               : sz(s) {};

         size_type size(void) const {
            return(sz);
         }
      };

      typedef interface base_type;
   };

   struct Integral {
   public:
      typedef Type<Simple> base_type;
   };

   struct Floating {
   public:
      typedef Type<Simple> base_type;
   };

   struct Derived {
   public:
      typedef InnerType base_type;
   };

   struct Array {
   private:
      typedef Type<Derived> interface_base_type;

   public:
      class interface
            : public interface_base_type {
      public:
         typedef int dimension_type;

      private:
         typedef std::vector<int> dimension_vector;

         dimension_vector dimensions;

      public:
         typedef BaseType child_type;
         typedef ptr<child_type>::type child_ptr
         typedef ptr<child_type>::const_type const_child_ptr;

         typedef dimension_vector::iterator iterator;

         void set_element_type(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         };

         child_ptr get_element_type(void) {
            return(front());
         };

         const_child_ptr get_element_type(void) const {
            return(front());
         };

         iterator begin(void) {
            return(dimensions.begin());
         };
         const_iterator begin(void) const {
            return(dimensions.begin());
         };

         iterator end(void) {
            return(dimensions.end());
         };
         const_iterator end(void) const {
            return(dimensions.end());
         };

         void push_back(dimension_type d) {
            dimensions.push_back(d);
         };

         size_type size(void) const {
            return(std::accumulate(begin(), end(),
                                   get_element_type()->size(),
                                   std::multiplies<size_type>()));
      };
   };

   struct Pointer {
   private:
      typedef Type<Derived> interface_base_type;

   public:
      class interface
            : public interface_base_type {
      public:
         typedef BaseType child_type;
         typedef ptr<child_type>::type child_ptr
         typedef ptr<child_type>::const_type const_child_ptr;

         void set_base_type(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         };

         child_ptr get_base_type(void) {
            return(front());
         };

         const_child_ptr get_base_type(void) const {
            return(front());
         };
      };
   };
}

#endif
