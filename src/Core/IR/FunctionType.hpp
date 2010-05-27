#ifndef mirv_Core_IR_FunctionType_hpp
#define mirv_Core_IR_FunctionType_hpp

#include <mirv/Core/IR/Type.hpp>

#include <boost/bind.hpp>

#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/size.hpp>

namespace mirv {

  namespace detail {
     class OutputArg {
       std::ostream &out;

     public:
       OutputArg(std::ostream &o) : out(o) {}
       typedef void result_type;
       template<typename Item>
       result_type operator()(Item i) const {
         out << ", " << i->name();
       }
     };

    /// A helper template selected on sequence size so we don't
    /// attempt to front() or pop_front() an empty list.
    template<int size>
    class OutputArgs {
    private:
      std::ostream &out;

    public:
      OutputArgs(std::ostream &o) : out(o) {}
      template<typename ArgSeq>
      void operator()(ArgSeq argTypes) {
        // Print the first argument type.
        out << boost::fusion::front(argTypes)->name();
        // Print the following argument type preceeded by a comma.
        boost::fusion::for_each(boost::fusion::pop_front(argTypes),
                                OutputArg(out));
      }
    };

    /// Specilization for an empty sequence.
    template<>
    class OutputArgs<0> {
    public:
      OutputArgs(std::ostream &) {}
      template<typename ArgSeq>
      void operator()(ArgSeq) {}
    };
  }

  /// A function type.  Function types have a return type and a list
  /// of parameter types.  A function that does not return anything
  /// will have a pointer to zero as its return type.  A funtion that
  /// does not take any arguments will have an empty argument
  /// sequence.
   struct FunctionType {
   private:
     typedef Symbol<Type<Derived> > InterfaceBaseType;

     class Interface : public InterfaceBaseType,
                       public boost::enable_shared_from_this<Symbol<Type<FunctionType> > > {
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

        template<typename Sequence>
        std::string constructName(ChildPtr ReturnType, Sequence Args) {
          std::stringstream name;
          if (ReturnType) {
            name << ReturnType->name() << " (";
          }
          else {
            name << "void ";
          }

          detail::OutputArgs<
          boost::fusion::result_of::size<Sequence>::type::value
            > outputArgs(name);

          outputArgs(Args);

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
            : InterfaceBaseType(constructName(returnType, args)) {
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

       detail::OutputArgs<
       boost::fusion::result_of::size<FusionSequence>::type::value
         > outputArgs(name);

     outputArgs(argTypes);

       name << ")";
       return name.str();
     }
   };
}

#endif
