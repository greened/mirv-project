#ifndef mirv_core_ir_type_hh
#define mirv_core_ir_type_hh

#include <mirv/core/ir/symbol.hh>

#include <vector>
#include <numeric>

namespace mirv {
   struct SymbolVisitor;

  /// This is the type implementation for all type symbols.  It is a
  /// templated Symbol tag.  Each type is an instance of this template
  /// (Symbol<Type<Integral> >, Symbol<Type<Function> >, etc.).  It
  /// keeps all of the property and visitor logic in one place, hiding
  /// the gory details from the type tags and specific type
  /// interfaces.
   template<typename Tag>
   class Type {
   public:
     typedef typename Tag::base_type base_type;
     typedef typename Tag::visitor_base_type visitor_base_type;

     Type() {}
     Type(const std::string &name) : base_type(name) {}
   };

  /// A type tag for the base type of all types.
   class TypeBase {
   private:
     typedef Symbol<Named> interface_base_type;

   public:

      class interface : public interface_base_type {
      public:
	interface(const std::string &name) : interface_base_type(name) {}
	typedef int BitSizeType;
	virtual BitSizeType bitsize(void) const = 0;
      };

      typedef interface base_type;
     typedef Symbol<Base> visitor_base_type;
   };

  //  template<>
  //class Symbol<Type<TypeBase> > : public Type<TypeBase>::base_type {};

  /// A type with no children.
  class LeafType : public LeafImpl<Symbol<Type<TypeBase> > > {
  public:
    typedef LeafImpl<Symbol<Type<TypeBase> > > BaseType;
    typedef Symbol<Type<TypeBase> > visitor_base_type;
    LeafType(const std::string &name) : BaseType(name) {}
  };

  /// This is the implementation of inner types.  It is inherited from
  /// once in the hierarchy for any inner types.  This holds the child
  /// pointers and other data necessary for inner types.
  class InnerType : public InnerImpl<Symbol<Type<TypeBase> >, VisitedInherit1<SymbolVisitor>::apply<Symbol<Type<TypeBase> > >::type> {
  public:
    InnerType(const std::string &name) : BaseType(name) {}
    typedef InnerImpl<Symbol<Type<TypeBase> >, VisitedInherit1<SymbolVisitor>::apply<Symbol<Type<TypeBase> > >::type> BaseType;
    typedef Symbol<Type<TypeBase> > visitor_base_type;
  };

  /// A type with no children that has a specific bit size, for
  /// example integer and floating point types.
   class Simple {
   private:
     typedef LeafType interface_base_type;

     class interface : public interface_base_type {
     private:
       BitSizeType bsize;

     public:
       interface(const std::string &name, BitSizeType s)
	 : interface_base_type(name), bsize(s) {};

       BitSizeType bitsize(void) const {
	 return bsize;
       }
     };
   public:
     typedef interface base_type;
     typedef LeafType visitor_base_type;
   };

  /// Integer types.
   struct Integral {
   private:
    typedef Symbol<Type<Simple> > interface_base_type;

     class interface : public interface_base_type {
     public:
       interface(const std::string &name, BitSizeType size) :
	 interface_base_type(name, size) {}
     };

   public:
     typedef interface base_type;
     typedef Symbol<Type<Simple> > visitor_base_type;
  };

  /// Floating point types.
   struct Floating {
    typedef Symbol<Type<Simple> > interface_base_type;

     class interface : public interface_base_type {
     public:
       interface(const std::string &name, BitSizeType size) :
	 interface_base_type(name, size) {}
     };
   public:
     typedef interface base_type;
     typedef Symbol<Type<Simple> > visitor_base_type;
  };

  /// A type that is built upon other types.  For example structures
  /// and pointers.
   struct Derived {
   public:
      typedef InnerType base_type;
     typedef InnerType visitor_base_type;
   };

  /// An array type.  Array types are true multidimensional concepts.
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
	interface(const std::string &name) : interface_base_type(name) {}
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

	/// Get the start of the dimension sequence.
         dimensionIterator dimensionBegin(void) {
            return(dimensions.begin());
         }
	/// Get the start of the dimension sequence.
         constDimensionIterator dimensionBegin(void) const {
            return(dimensions.begin());
         }

	/// Get the end of the dimension sequence.
         dimensionIterator dimensionEnd(void) {
            return(dimensions.end());
         }
	/// Get the end of the dimension sequence.
         constDimensionIterator dimensionEnd(void) const {
            return(dimensions.end());
         }

	/// Add a dimension.
         void dimensionPushBack(dimension_type d) {
            dimensions.push_back(d);
         }

         BitSizeType bitsize(void) const {
            return(std::accumulate(dimensionBegin(), dimensionEnd(),
                                   getElementType()->bitsize(),
                                   std::multiplies<size_type>()));
	 }
	virtual void accept(mirv::SymbolVisitor &) {
	  error("Array::accept called");
	}
      };
     typedef interface base_type;
     typedef Symbol<Type<Derived> > visitor_base_type;
   };

  /// A pointer type.
   struct Pointer {
   private:
     typedef Symbol<Type<Derived> > interface_base_type;

   public:
      class interface
            : public interface_base_type {
      public:
	interface(const std::string &name) : interface_base_type(name) {}
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
	virtual void accept(mirv::SymbolVisitor &) {
	  error("Pointer::accept called");
	}
      };
     typedef interface base_type;
     typedef Symbol<Type<Derived> > visitor_base_type;
   };

  /// A function type.  Function types have a return type and a list
  /// of parameter types.  A function that does not return anything
  /// will have a pointer to zero as its return type.  A funtion that
  /// does not take any arguments will have an empty argument
  /// sequence.
   struct FunctionType {
   private:
     typedef Symbol<Type<Derived> > interface_base_type;

   public:
      class interface
            : public interface_base_type {
      public:
	typedef Symbol<Type<TypeBase> > child_type;
	typedef ptr<child_type>::type child_ptr;
	typedef ptr<child_type>::const_type const_child_ptr;

	interface(const std::string &name,
		  child_ptr ReturnType = child_ptr())
	  : interface_base_type(name) {
	  setReturnType(ReturnType);
	}

	BitSizeType bitsize(void) const {
	  return 0;
	}

	/// By conventon, the return type is the first element of the
	/// underlying child list.  A void function will have a 0
	/// pointer as the first child.
	void setReturnType(child_ptr c) {
	  if (empty()) {
	    push_back(c);
	  }
	  else {
	    *begin() = c;
	  }
	}

	child_ptr getReturnType(void) {
	  return(front());
	}

	const_child_ptr getReturnType(void) const {
	  return(front());
	}

	/// Add a parameter type.
	void parametersPushBack(child_ptr Parameter) {
	  push_back(Parameter);
	}

	/// Return whether this function type does not have any
	/// parameters.
	bool parameters_empty(void) const {
	  return empty();
	}

	/// Get the start of the parameter type sequence.
	iterator parameters_begin(void) {
	  return ++begin();
	}
	/// Get the start of the parameter type sequence.
	const_iterator parameters_begin(void) const {
	  return ++begin();
	}
	/// Get the end of the parameter type sequence.
	iterator parameters_end(void) {
	  return end();
	}
	/// Get the end of the parameter type sequence.
	const_iterator parameters_end(void) const {
	  return end();
	}
	virtual void accept(mirv::SymbolVisitor &) {
	  error("FunctionType::accept called");
	}
      };
     typedef interface base_type;
     typedef Symbol<Type<Derived> > visitor_base_type;
   };
}

#endif
