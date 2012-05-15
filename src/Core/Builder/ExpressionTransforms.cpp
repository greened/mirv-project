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
    ptr<Expression<Base> >
    VariableRefTransform::operator()(ptr<const SymbolTable> symtab,
                                     const std::string &name) {
      // Pointer
      return ConstructUnary<Expression<Load> >()(
        symtab,
        ConstructUnary<
          Expression<Reference<Variable> >
          >()(symtab,
              LookupSymbol<Symbol<Variable> >()(symtab, name)));
    }

    ptr<Expression<Base> >
    GlobalVariableRefTransform::operator()(ptr<SymbolTable> symtab,
                                           const std::string &name) {
      return ConstructGlobalReference()(
        symtab,
        LookupSymbol<Symbol<GlobalVariable> >()(symtab, name));
    }

    ptr<Expression<Base> >
    FunctionRefTransform::operator()(ptr<const SymbolTable> symtab,
                                     const std::string &name) {
      return ConstructUnary<Expression<Reference<Function> > >()(
        symtab,
        LookupSymbol<Symbol<Function> >()(symtab, name));
    }

    ptr<Expression<Reference<Constant<Base> > > >
    ConstantRefTransform::operator()(ptr<const SymbolTable> symtab,
                                     ptr<Symbol<Constant<Base> > > constant) {
      return ConstructUnary<Expression<Reference<Constant<Base> > > >()(
        symtab, constant);
    }

    ptr<Expression<Base> >
    ArrayRefSequenceTransform::operator()(ptr<const SymbolTable> symtab,
                                          ptr<Expression<Base> > address) {
      return ConstructUnary<Expression<Load> >()(symtab, address);
    }

    ptr<Expression<Base> >
    ArrayRefIndexTransform::operator()(ptr<SymbolTable> symtab,
                                       ptr<Expression<Base> > base,
                                       ptr<Expression<Base> > index) {
      return ConstructUnary<Expression<Load> >()(
        symtab, ConstructAddress()(symtab, base, index));
    }

    ptr<Expression<Base> >
    ArrayAddressSequenceTransform::operator()(ptr<const SymbolTable> symtab,
                                              ptr<Expression<Base> > address) {
      return address;
    }

    ptr<Expression<Base> >
    ArrayAddressIndexTransform::operator()(ptr<SymbolTable> symtab,
                                           ptr<Expression<Base> > base,
                                           ptr<Expression<Base> > index) {
      return ConstructAddress()(symtab, base, index);
    }
  }
}
