#ifndef mirv_Core_IR_Module_hpp
#define mirv_Core_IR_Module_hpp

#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/GlobalVariableFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

namespace mirv {
  class Module;
  namespace detail {
    class ModuleInterface : public Symbol<Named>,
                            public InnerImpl<Symbol<Function>, Virtual<Symbol<Base> > >,
                            public InnerImpl<const Symbol<Type<TypeBase> >, Virtual<Symbol<Base> > >,
                            public InnerImpl<Symbol<GlobalVariable>, Virtual<Symbol<Base> > >,
                            public boost::enable_shared_from_this<Symbol<Module> > {
    private:
      typedef InnerImpl<Symbol<Function>, Virtual<Symbol<Base> > > FunctionBaseType;
      typedef InnerImpl<const Symbol<Type<TypeBase> >, Virtual<Symbol<Base> > > TypeBaseType;
      typedef InnerImpl<Symbol<GlobalVariable>, Virtual<Symbol<Base> > > GlobalVariableBaseType;

    public:
      ModuleInterface(const std::string &n);

      // Access function information
      typedef FunctionBaseType::ChildPtr FunctionPointer;

      /// Add a function.
      void functionPushBack(FunctionPointer f);
 
      typedef FunctionBaseType::iterator FunctionIterator;
      typedef FunctionBaseType::const_iterator ConstFunctionIterator;
      /// Return the start of the function sequence.
      FunctionIterator functionBegin(void) {
        return FunctionBaseType::begin();
      }
      /// Return the end of the function sequence.
      FunctionIterator functionEnd(void) {
        return FunctionBaseType::end();
      }
      /// Return the start of the function sequence.
      ConstFunctionIterator functionBegin(void) const {
        return FunctionBaseType::begin();
      }
      /// Return the end of the function sequence.
      ConstFunctionIterator functionEnd(void) const {
        return FunctionBaseType::end();
      }

      FunctionIterator functionFind(const std::string &name);

      // Access variable information
      typedef GlobalVariableBaseType::ChildPtr GlobalVariablePointer;

      /// Add a global variable.
      void globalVariablePushBack(GlobalVariablePointer v);

      typedef GlobalVariableBaseType::iterator GlobalVariableIterator;
      typedef GlobalVariableBaseType::const_iterator ConstGlobalVariableIterator;
      /// Return the start of the variable sequence.
      GlobalVariableIterator globalVariableBegin(void) {
        return GlobalVariableBaseType::begin();
      }
      /// Return the end of the variable sequence.
      GlobalVariableIterator globalVariableEnd(void) {
        return GlobalVariableBaseType::end();
      }
      /// Return the start of the variable sequence.
      ConstGlobalVariableIterator globalVariableBegin(void) const {
        return GlobalVariableBaseType::begin();
      }
      /// Return the end of the variable sequence.
      ConstGlobalVariableIterator globalVariableEnd(void) const {
        return GlobalVariableBaseType::end();
      }

      GlobalVariableIterator globalVariableFind(const std::string &name);

      // Access type information
      typedef TypeBaseType::ConstChildPtr TypePointer;

      /// Add a type.
      void typePushBack(TypePointer t);

      typedef TypeBaseType::iterator TypeIterator;
      typedef TypeBaseType::const_iterator ConstTypeIterator;
      /// Return the start of the type sequence.
      TypeIterator typeBegin(void) {
        return TypeBaseType::begin();
      }
      /// Return the end of the type sequence.
      TypeIterator typeEnd(void) {
        return TypeBaseType::end();
      }
      /// Return the start of the type sequence.
      ConstTypeIterator typeBegin(void) const {
        return TypeBaseType::begin();
      }
      /// Return the end of the type sequence.
      ConstTypeIterator typeEnd(void) const {
        return TypeBaseType::end();
      }

      TypeIterator typeFind(const std::string &name);

      ptr<Node<Base>> getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<const Node<Base>> getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }
    };
  }

  /// This is the symbol tag for module symbols.  A module is a
  /// collection of types, global variables and functions packaged
  /// together.
  class Module {
  public:
    static void initialize(ptr<Symbol<Module> > module);

    static std::string getName(const std::string &name) {
      return name;
    }
  };
}

#endif
