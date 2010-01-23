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
     typedef typename Tag::BaseType BaseType;
     typedef typename Tag::VisitorBaseType VisitorBaseType;

     Type() {}
     Type(const std::string &name) : BaseType(name) {}
   };

  /// A type tag for the base type of all types.
   class TypeBase {
   private:
     typedef Symbol<Named> InterfaceBaseType;

      class Interface : public InterfaceBaseType {
      public:
	Interface(const std::string &name) : InterfaceBaseType(name) {}
	typedef int BitSizeType;
	virtual BitSizeType bitsize(void) const = 0;
      };

   public:
     typedef Interface BaseType;
     typedef Symbol<Base> VisitorBaseType;
   };

  //  template<>
  //class Symbol<Type<TypeBase> > : public Type<TypeBase>::BaseType {};

  /// A type with no children.
  class LeafType : public LeafImpl<Symbol<Type<TypeBase> > > {
  public:
    typedef LeafImpl<Symbol<Type<TypeBase> > > BaseType;
    typedef Symbol<Type<TypeBase> > VisitorBaseType;
    LeafType(const std::string &name) : BaseType(name) {}
  };

  /// This is the implementation of inner types.  It is inherited from
  /// once in the hierarchy for any inner types.  This holds the child
  /// pointers and other data necessary for inner types.
  class InnerType : public InnerImpl<Symbol<Type<TypeBase> >, VisitedInherit1<SymbolVisitor>::apply<Symbol<Type<TypeBase> > >::type> {
  public:
    InnerType(const std::string &name) : BaseType(name) {}
    typedef InnerImpl<Symbol<Type<TypeBase> >, VisitedInherit1<SymbolVisitor>::apply<Symbol<Type<TypeBase> > >::type> BaseType;
    typedef Symbol<Type<TypeBase> > VisitorBaseType;
  };

  /// A type with no children that has a specific bit size, for
  /// example integer and floating point types.
   class Simple {
   private:
     typedef LeafType InterfaceBaseType;

     class Interface : public InterfaceBaseType {
     private:
       BitSizeType bsize;

     public:
       Interface(const std::string &name, BitSizeType s)
	 : InterfaceBaseType(name), bsize(s) {};

       BitSizeType bitsize(void) const {
	 return bsize;
       }
     };

   public:
     typedef Interface BaseType;
     typedef LeafType VisitorBaseType;
   };

  /// Integer types.
   struct Integral {
   private:
    typedef Symbol<Type<Simple> > InterfaceBaseType;

     class Interface : public InterfaceBaseType {
     public:
       Interface(const std::string &name, BitSizeType size) :
	 InterfaceBaseType(name, size) {}
     };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Simple> > VisitorBaseType;
  };

  /// Floating point types.
   struct Floating {
    typedef Symbol<Type<Simple> > InterfaceBaseType;

     class Interface : public InterfaceBaseType {
     public:
       Interface(const std::string &name, BitSizeType size) :
	 InterfaceBaseType(name, size) {}
     };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Simple> > VisitorBaseType;
  };

  /// A type that is built upon other types.  For example structures
  /// and pointers.
   struct Derived {
   public:
      typedef InnerType BaseType;
     typedef InnerType VisitorBaseType;
   };

  /// An array type.  Array types are true multidimensional concepts.
   struct Array {
   private:
     typedef Symbol<Type<Derived> > InterfaceBaseType;

      class Interface : public InterfaceBaseType {
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

	/// Get the end of the dimension sequence.
         DimensionIterator dimensionEnd(void) {
            return(dimensions.end());
         }
	/// Get the end of the dimension sequence.
         ConstDimensionIterator dimensionEnd(void) const {
            return(dimensions.end());
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
	virtual void accept(mirv::SymbolVisitor &) {
	  error("Array::accept called");
	}
      };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Derived> > VisitorBaseType;
   };

  /// A pointer type.
   struct Pointer {
   private:
     typedef Symbol<Type<Derived> > InterfaceBaseType;

      class Interface : public InterfaceBaseType {
      public:
	Interface(const std::string &name) : InterfaceBaseType(name) {}
	typedef Symbol<Type<TypeBase> > ChildType;
	typedef ptr<ChildType>::type ChildPtr;
         typedef ptr<ChildType>::const_type ConstChildPtr;

         void setBaseType(ChildPtr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         }

         ChildPtr getBaseType(void) {
            return(front());
         }

         ConstChildPtr getBaseType(void) const {
            return(front());
         }
	virtual void accept(mirv::SymbolVisitor &) {
	  error("Pointer::accept called");
	}
      };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Derived> > VisitorBaseType;
   };

  /// A function type.  Function types have a return type and a list
  /// of parameter types.  A function that does not return anything
  /// will have a pointer to zero as its return type.  A funtion that
  /// does not take any arguments will have an empty argument
  /// sequence.
   struct FunctionType {
   private:
     typedef Symbol<Type<Derived> > InterfaceBaseType;

      class Interface : public InterfaceBaseType {
      public:
	typedef Symbol<Type<TypeBase> > ChildType;
	typedef ptr<ChildType>::type ChildPtr;
	typedef ptr<ChildType>::const_type ConstChildPtr;

	Interface(const std::string &name,
		  ChildPtr ReturnType = ChildPtr())
	  : InterfaceBaseType(name) {
	  setReturnType(ReturnType);
	}

	BitSizeType bitsize(void) const {
	  return 0;
	}

	/// By conventon, the return type is the first element of the
	/// underlying child list.  A void function will have a 0
	/// pointer as the first child.
	void setReturnType(ChildPtr c) {
	  if (empty()) {
	    push_back(c);
	  }
	  else {
	    *begin() = c;
	  }
	}

	ChildPtr getReturnType(void) {
	  return(front());
	}

	ConstChildPtr getReturnType(void) const {
	  return(front());
	}

	/// Add a parameter type.
	void parameterPushBack(ChildPtr Parameter) {
	  push_back(Parameter);
	}

	/// Return whether this function type does not have any
	/// parameters.
	bool parameterEmpty(void) const {
	  return empty();
	}

	/// Get the start of the parameter type sequence.
	iterator parameterBegin(void) {
	  return ++begin();
	}
	/// Get the start of the parameter type sequence.
	const_iterator parameterBegin(void) const {
	  return ++begin();
	}
	/// Get the end of the parameter type sequence.
	iterator parameterEnd(void) {
	  return end();
	}
	/// Get the end of the parameter type sequence.
	const_iterator parameterEnd(void) const {
	  return end();
	}
	virtual void accept(mirv::SymbolVisitor &) {
	  error("FunctionType::accept called");
	}
      };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Derived> > VisitorBaseType;
   };
}

#endif
