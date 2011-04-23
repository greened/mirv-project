#ifndef mirv_Core_Builder_VariableGrammar_hpp
#define mirv_Core_Builder_VariableGrammar_hpp

#include <mirv/Core/Builder/VariableRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/IR/PointerTypeFwd.hpp>
#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for local variable symbols.
    struct VariableBuilder : boost::proto::when<
      VariableRule,
      ExtractVariable(
        boost::proto::_data,
        AddAllocateStatement(
          boost::proto::_data,
          ConstructTernary<
            Statement<Allocate>,
            Statement<Allocate>::ChildPtr,
            Statement<Allocate>::ChildPtr,
            Statement<Allocate>::TypePtr>(
              boost::proto::_data,
              // Variable to hold the address
              ConstructUnary<
                Expression<Reference<Variable> >
                >(boost::proto::_data,
                  BinaryConstructSymbol<
                    Symbol<Variable> >(
                      boost::proto::_data,
                      // Variable name
                      boost::proto::_value(boost::proto::_right(
                                             boost::proto::_left(
                                               boost::proto::_left))),
                      // Variable type (pointer to allocated type)
                      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
                        boost::proto::_data,
                        UnaryConstructSymbol<Symbol<Type<Pointer> > >(
                          boost::proto::_data,
                          TypeAccessBuilder(boost::proto::_right))))),
              // Number of elements
              ConstructUnary<
                Expression<Reference<Constant<Base> > >
                >(boost::proto::_data,
                  ConstructIntegerConstantSymbol<1>(boost::proto::_data)),
              // Type to allocate
              TypeAccessBuilder(boost::proto::_right))))
      > {};

    namespace {
      /// A var "operator."  This is a protoized object that
      /// implements the subscript operator to kick off variable
      /// symbol generation.
      const VarTerminal var = {{}};
    }
  }
}

#endif
