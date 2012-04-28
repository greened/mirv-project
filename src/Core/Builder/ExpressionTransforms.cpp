#include <mirv/Core/Builder/ExpressionTransforms.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Variable.hpp>

namespace mirv {
  namespace Builder {
    ptr<Expression<Base> >::type
    VariableRefTransform::operator()(ptr<SymbolTable>::const_type symtab,
                                     const std::string &name) {
      // Pointer
      return ConstructUnary<Expression<Load> >()(
        symtab,
        ConstructUnary<
          Expression<Reference<Variable> >
          >()(symtab,
              LookupSymbol<Symbol<Variable> >()(symtab, name)));
    }

    ptr<Expression<Base> >::type
    GlobalVariableRefTransform::operator()(ptr<SymbolTable>::type symtab,
                                           const std::string &name) {
      return ConstructGlobalReference()(
        symtab,
        LookupSymbol<Symbol<GlobalVariable> >()(symtab, name));
    }

    ptr<Expression<Base> >::type
    FunctionRefTransform::operator()(ptr<SymbolTable>::const_type symtab,
                                     const std::string &name) {
      return ConstructUnary<Expression<Reference<Function> > >()(
        symtab,
        LookupSymbol<Symbol<Function> >()(symtab, name));
    }

    ptr<Expression<Reference<Constant<Base> > > >::type
    ConstantRefTransform::operator()(ptr<SymbolTable>::const_type symtab,
                                     ptr<Symbol<Constant<Base> > >::type constant) {
      return ConstructUnary<Expression<Reference<Constant<Base> > > >()(
        symtab, constant);
    }

    ptr<Expression<Base> >::type
    ArrayRefSequenceTransform::operator()(ptr<SymbolTable>::const_type symtab,
                                          ptr<Expression<Base> >::type address) {
      return ConstructUnary<Expression<Load> >()(symtab, address);
    }

    ptr<Expression<Base> >::type
    ArrayRefIndexTransform::operator()(ptr<SymbolTable>::type symtab,
                                       ptr<Expression<Base> >::type base,
                                       ptr<Expression<Base> >::type index) {
      return ConstructUnary<Expression<Load> >()(
        symtab, ConstructAddress()(symtab, base, index));
    }

    ptr<Expression<Base> >::type
    ArrayAddressSequenceTransform::operator()(ptr<SymbolTable>::const_type symtab,
                                              ptr<Expression<Base> >::type address) {
      return address;
    }

    ptr<Expression<Base> >::type
    ArrayAddressIndexTransform::operator()(ptr<SymbolTable>::type symtab,
                                           ptr<Expression<Base> >::type base,
                                           ptr<Expression<Base> >::type index) {
      return ConstructAddress()(symtab, base, index);
    }
  }
}
