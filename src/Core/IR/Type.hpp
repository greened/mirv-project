#ifndef mirv_Core_IR_Type_hpp
#define mirv_Core_IR_Type_hpp

#include <vector>
#include <numeric>
//#include <tr1/functional>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/size.hpp>

#include <mirv/Core/IR/Symbol.hpp>

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

     template<typename Arg>
     static std::string getName(Arg &a) {
       return Tag::getName(a);
     }

     template<typename Arg1, typename Arg2>
     static std::string getName(Arg1 &a1, Arg2 &a2) {
       return Tag::getName(a1, a2);
     }
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

   namespace detail {
     /// A traits class to define various properties of inner types
     /// such as child type, iterator types and other things.
     class InnerTypeTraits {
     public:
       typedef Symbol<Type<TypeBase> > Child;
       typedef Symbol<Type<TypeBase> > BaseType;

     private:
       typedef boost::shared_ptr<Child> ChildPtr;
       typedef std::list<ChildPtr> ChildList;

     public:
       /// Make this compatible with certain standard algorithms.
       typedef ChildPtr value_type;
       typedef const ChildPtr & const_reference;

       typedef ChildList::iterator iterator;
       typedef ChildList::reverse_iterator reverse_iterator;
       typedef ChildList::const_iterator const_iterator;
       typedef ChildList::const_reverse_iterator const_reverse_iterator;

       typedef ChildList::size_type size_type;
     };
   }

  /// This is an inner type abstract interface.  It exists because we
  /// need to be able to inherit virtually from inner types (to allow
  /// property symbol visitors to manipulate children) but we do not
  /// want to force subclasses to explicitly initialize the inner type
  /// object.  Separating the interface from the implementation solves
  /// that problem.
    template<>
    class Symbol<Type<Inner<detail::InnerTypeTraits> > > : public Inner<detail::InnerTypeTraits>::BaseType {
    private:
      typedef Inner<detail::InnerTypeTraits>::BaseType BaseType;

    public:
      Symbol<Type<Inner<detail::InnerTypeTraits> > >(const std::string &name)
      : BaseType(name) {}

      typedef Symbol<Type<TypeBase> > VisitorBaseType;
      virtual void accept(SymbolVisitor &V);
   };

  class InnerTypeBase : public Symbol<Type<Inner<detail::InnerTypeTraits> > > {
  private:
    typedef Symbol<Type<Inner<detail::InnerTypeTraits> > > BaseType;
  public:
    InnerTypeBase(const std::string &name) : BaseType(name) {}
  };

  /// This is the implementation of inner types.  It is inherited from
  /// once in the hierarchy for any inner types.  This holds the child
  /// pointers and other data necessary for inner types.
  class InnerType : public InnerImpl<
    Symbol<Type<TypeBase> >,
    VisitedInherit1<SymbolVisitor>::apply<InnerTypeBase>::type> {
  private:
    typedef InnerImpl<
    Symbol<Type<TypeBase> >,
    VisitedInherit1<SymbolVisitor>::apply<InnerTypeBase>::type> BaseType;

  public:
    InnerType(const std::string &name) : BaseType(name) {}

    virtual void accept(SymbolVisitor &V);
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
       Interface(BitSizeType size) :
           InterfaceBaseType("int" + boost::lexical_cast<std::string>(size),
                             size) {}
     };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Simple> > VisitorBaseType;

     static std::string getName(Interface::BitSizeType size) {
       return "int" + boost::lexical_cast<std::string>(size);
     }
  };

  /// Floating point types.
   struct Floating {
    typedef Symbol<Type<Simple> > InterfaceBaseType;

     class Interface : public InterfaceBaseType {
     public:
       Interface(BitSizeType size) :
           InterfaceBaseType("float" + boost::lexical_cast<std::string>(size),
                             size) {}
     };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Simple> > VisitorBaseType;

     static std::string getName(Interface::BitSizeType size) {
       return "float" + boost::lexical_cast<std::string>(size);
     }
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

      private:
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

        std::string constructName(ChildPtr ReturnType) {
          if (ReturnType) {
            return ReturnType->name() + " ()";
          }
          else {
            return "void ()";
          }
        }

        template<typename InputIterator>
        std::string constructName(ChildPtr ReturnType,
                                  InputIterator start, InputIterator end) {
          std::stringstream name;
          name << ReturnType->name() << "(";
          for (InputIterator a = start; a != end; /* NULL */) {
            name << (*a)->name();
            if ((a = boost::fusion::next(a)) != end) {
              name << ',';
            }
          }
          name << ")";
          return name.str();
        }

      public:
	Interface(ChildPtr returnType)
            : InterfaceBaseType(constructName(returnType)) {
	  setReturnType(returnType);
        }

        template<typename Sequence>
	Interface(ChildPtr returnType, Sequence args)
            : InterfaceBaseType(constructName(returnType,
                                              boost::fusion::begin(args),
                                              boost::fusion::end(args))) {
	  setReturnType(returnType);
          // Add the parameter types.
          boost::fusion::for_each(args,
                                  boost::bind(&Interface::push_back,
                                              this,
                                              _1));
	}

	BitSizeType bitsize(void) const {
	  return 0;
	}

	ChildPtr getReturnType(void) {
	  return(front());
	}

	ConstChildPtr getReturnType(void) const {
	  return(front());
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

     class Output {
       std::ostream &out;

     public:
       Output(std::ostream &o) : out(o) {}
       typedef void result_type;
       template<typename Item>
       result_type operator()(Item i) const {
         out << ", " << i->getName();
       }
     };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Derived> > VisitorBaseType;

     static std::string
     getName(ptr<Symbol<Type<TypeBase> > >::type returnType) {
       if (returnType) {
         return returnType->name() + " ()";
       }
       else {
         return "void ()";
       }
     }

     template<typename FusionSequence>
     static std::string getName(ptr<Symbol<Type<TypeBase> > >::type returnType,
                                FusionSequence argTypes) {
       std::stringstream name;
       if (returnType) {
         name << returnType->name() << " (";
       }
       else {
         name << "void (";
       }

       int size = boost::fusion::size(argTypes);

       if (size > 0) {
         // Print the first argument type.
         name << boost::fusion::front(argTypes)->name();
         if (size > 1) {
           // Print the following argument type preceeded by a comma.
           boost::fusion::for_each(boost::fusion::pop_front(argTypes),
                                   Output(name));
         }
       }
  
       name << ")";
       return name.str();
     }
   };

  /// A struct type.  Struct types have a list of member types.
   struct StructType {
   private:
     typedef Symbol<Type<Derived> > InterfaceBaseType;

     class Interface : public InterfaceBaseType {
      public:
	typedef Symbol<Type<TypeBase> > ChildType;
       typedef boost::shared_ptr<ChildType> ChildPtr;
       typedef boost::shared_ptr<const ChildType> ConstChildPtr;

      private:
      public:
        template<typename Sequence>
	Interface(std::string &name, Sequence members)
            : InterfaceBaseType(name) {
          // Add the member types.
          boost::fusion::for_each(members,
                                  boost::bind(&Interface::push_back,
                                              this,
                                              _1));
	}

	BitSizeType bitsize(void) const {
          // TODO: This depends on ABI rules.
	  return 0;
	}

	/// Get the start of the member type sequence.
	iterator memberBegin(void) {
	  return begin();
	}
	/// Get the start of the member type sequence.
	const_iterator memberBegin(void) const {
	  return begin();
	}
	/// Get the end of the member type sequence.
	iterator memberEnd(void) {
	  return end();
	}
	/// Get the end of the member type sequence.
	const_iterator memberEnd(void) const {
	  return end();
	}
	virtual void accept(mirv::SymbolVisitor &) {
	  error("StructType::accept called");
	}
      };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Derived> > VisitorBaseType;

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
