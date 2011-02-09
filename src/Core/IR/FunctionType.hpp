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
      void operator()(const ArgSeq &argTypes, bool vararg) {
        // Print the first argument type.
        out << boost::fusion::front(argTypes)->name();
        // Print the following argument type preceeded by a comma.
        boost::fusion::for_each(boost::fusion::pop_front(argTypes),
                                OutputArg(out));
        if (vararg) {
          out << ", ...";
        }
      }
    };

    /// Specialization for an empty sequence.
    template<>
    class OutputArgs<0> {
    private:
      std::ostream &out;

    public:
      OutputArgs(std::ostream &o) : out(o) {}
      template<typename ArgSeq>
      void operator()(const ArgSeq &, bool vararg) {
        if (vararg) {
          out << "...";
        }
      }
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
      typedef ptr<ChildType>::const_type ChildPtr;
      typedef ptr<ChildType>::const_type ConstChildPtr;

      /// Name some booleans to clarify code when setting a function
      /// vararg.
      enum VarargMark {
        Vararg = true,
        NotVararg = false
      };

    private:
      /// Mark whether this function is vararg.
      VarargMark vararg;

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

      std::string constructName(ChildPtr ReturnType,
                                VarargMark vararg) {
        const char *args = vararg == VarargMark::Vararg ? "(...)" : "()";
        if (ReturnType) {
          return ReturnType->name() + " " + args;
        }
        else {
          return std::string("void ") + args;
        }
      }

      template<typename Sequence>
      std::string constructName(ChildPtr ReturnType, const Sequence &Args, VarargMark vararg) {
        std::stringstream name;
        if (ReturnType) {
          name << ReturnType->name() << " (";
        }
        else {
          name << "void (";
        }

        detail::OutputArgs<
          boost::fusion::result_of::size<Sequence>::type::value
          > outputArgs(name);

        outputArgs(Args, vararg == VarargMark::Vararg);

        name << ")";
        return name.str();
      }

      template<typename Iterator>
      std::string constructName(ChildPtr ReturnType,
                                Iterator start,
                                Iterator end,
                              VarargMark vararg) {
        std::stringstream name;
        if (ReturnType) {
          name << ReturnType->name() << " (";
        }
        else {
          name << "void (";
        }

        while (start != end) {
          name << (*start)->name();
          if (++start != end || vararg == VarargMark::Vararg) {
            name << ", ";
          }
        }
        if (vararg == VarargMark::Vararg) {
          name << "...";
        }
        name << ")";
        return name.str();
      }

    public:
      Interface(ChildPtr returnType, VarargMark v = VarargMark::NotVararg)
          : InterfaceBaseType(constructName(returnType, v)),
              vararg(v) {
        setReturnType(returnType);
      }

      template<typename Iterator>
      Interface(ChildPtr returnType,
                Iterator start,
                Iterator end,
                VarargMark v = VarargMark::NotVararg)
          : InterfaceBaseType(constructName(returnType,
                                            start,
                                            end,
                                            v)),
              vararg(v) {
        setReturnType(returnType);
        // Add the parameter types.
        std::copy(start, end, std::back_inserter(*this));
      }

      template<typename Sequence>
      Interface(ChildPtr returnType,
                const Sequence &args,
                VarargMark v)
          : InterfaceBaseType(constructName(returnType, args, v)),
              vararg(v) {
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

      bool isVararg(void) const {
        return vararg == VarargMark::Vararg;
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

    static std::string
    getName(ptr<Symbol<Type<TypeBase> > >::const_type returnType,
            Interface::VarargMark vararg) {
      const char *args =
        vararg == Symbol<Type<FunctionType> >::VarargMark::Vararg ?
        " (...)" : " ()";

      if (returnType) {
        return returnType->name() + args;
      }
      else {
        return std::string("void") + args;
      }
    }

    template<typename FusionSequence>
    static std::string
    getName(ptr<Symbol<Type<TypeBase> > >::const_type returnType,
            const FusionSequence &argTypes,
            Interface::VarargMark vararg) {
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

      outputArgs(argTypes,
                 vararg == Symbol<Type<FunctionType> >::VarargMark::Vararg);

      name << ")";
      return name.str();
    }
  };
}

#endif
