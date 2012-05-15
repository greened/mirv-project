#ifndef mirv_Core_IR_Function_hpp
#define mirv_Core_IR_Function_hpp

#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>

#include <tr1/functional>

namespace mirv {
  namespace detail {
    /// Define the interface for function symbols.
    class FunctionInterface : public Symbol<Global>,
                              public InnerImpl<Symbol<Variable>, Virtual<Symbol<Base> > >,
                              public InnerImpl<Statement<Base>, Virtual<Symbol<Base> > >,
                              public boost::enable_shared_from_this<Symbol<Function> > {
    private:
      typedef Symbol<Global> GlobalBaseType;
     
      typedef InnerImpl<
        Statement<Base>,
        Virtual<Symbol<Base> >
        > StatementBaseType;

      typedef InnerImpl<
        Symbol<Variable>,
        Virtual<Symbol<Base> >
        > VariableBaseType;

    public:
      typedef StatementBaseType::ChildPtr StatementPtr;
      typedef StatementBaseType::ConstChildPtr ConstStatementPtr;
      typedef VariableBaseType::ChildPtr VariablePtr;
      typedef VariableBaseType::ConstChildPtr ConstVariablePtr;

      FunctionInterface(const std::string &n, TypePtr t)
          : GlobalBaseType(n, t) {}

      FunctionInterface(const std::string &n,
                        TypePtr t,
                        StatementPtr s);

      /// Add a local variable to this function.
      void variablePushBack(VariablePtr v);

      typedef VariableBaseType::iterator VariableIterator;
      typedef VariableBaseType::const_iterator ConstVariableIterator;
      /// Get the start of the local variable sequence.
      VariableIterator variableBegin(void) {
        return VariableBaseType::begin();
      }
      /// Get the end of the local variable sequence.
      VariableIterator variableEnd(void) {
        return VariableBaseType::end();
      }
      /// Get the start of the local variable sequence.
      ConstVariableIterator variableBegin(void) const {
        return VariableBaseType::begin();
      }
      /// Get the end of the local variable sequence.
      ConstVariableIterator variableEnd(void) const {
        return VariableBaseType::end();
      }

      VariableIterator variableFind(const std::string &name) {
        return std::find_if(variableBegin(), variableEnd(),
                            std::tr1::bind(SymbolByName<Variable>(), std::tr1::placeholders::_1, name));
        //boost::bind(SymbolByName<Symbol<Variable> >(), _1, name));
      }

      void statementPushBack(StatementPtr stmt);

      /// Get the single block statement child.
      StatementPtr getStatement(void) {
        return *StatementBaseType::begin();
      }
      ConstStatementPtr getStatement(void) const {
        return *StatementBaseType::begin();
      }

      /// Return whether the function does not have a statement.
      bool statementEmpty(void) const {
        return StatementBaseType::empty();
      }

      typedef StatementBaseType::iterator StatementIterator;
      typedef StatementBaseType::const_iterator ConstStatementIterator;
      /// Get the start of the local statement sequence.
      StatementIterator statementBegin(void) {
        return StatementBaseType::begin();
      }
      /// Get the end of the local statement sequence.
      StatementIterator statementEnd(void) {
        return StatementBaseType::end();
      }
      /// Get the start of the local statement sequence.
      ConstStatementIterator statementBegin(void) const {
        return StatementBaseType::begin();
      }
      /// Get the end of the local statement sequence.
      ConstStatementIterator statementEnd(void) const {
        return StatementBaseType::end();
      }

      ptr<Node<Base>> getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<const Node<Base>> getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }
    };
  }

  /// This is a symbol tag for function symbols.
  class Function {
  public:
    static void initialize(ptr<Symbol<Function> > function) {}

    static std::string getName(const std::string &name,
                               ptr<const Symbol<Type<TypeBase> > > type) {
      return name;
    }
  };
}

#endif
