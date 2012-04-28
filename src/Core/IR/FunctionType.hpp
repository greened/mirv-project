#ifndef mirv_Core_IR_FunctionType_hpp
#define mirv_Core_IR_FunctionType_hpp

#include <mirv/Core/IR/FunctionTypeFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  namespace detail {
    class FunctionTypeInterface : public Symbol<Type<Derived> >,
                                  public boost::enable_shared_from_this<Symbol<Type<FunctionType> > > {
    private:
      typedef Symbol<Type<Derived> > BaseType;

    public:
      typedef Symbol<Type<TypeBase> > ChildType;
      typedef ptr<ChildType>::const_type ChildPtr;
      typedef ptr<ChildType>::const_type ConstChildPtr;

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

    public:
      FunctionTypeInterface(ChildPtr returnType, VarargMark v = VarargMark::NotVararg)
          : BaseType(), vararg(v) {
        setReturnType(returnType);
      }

      template<typename Iterator>
      FunctionTypeInterface(ChildPtr returnType,
                            Iterator start,
                            Iterator end,
                            VarargMark v = VarargMark::NotVararg)
          : BaseType(), vararg(v) {
        setReturnType(returnType);
        // Add the parameter types.
        std::copy(start, end, std::back_inserter(*this));
      }

      BitSizeType bitsize(void) const;

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
  }

  /// A function type.  Function types have a return type and a list
  /// of parameter types.  A function that does not return anything
  /// will have a pointer to zero as its return type.  A funtion that
  /// does not take any arguments will have an empty argument
  /// sequence.
  struct FunctionType {};
}

#endif
