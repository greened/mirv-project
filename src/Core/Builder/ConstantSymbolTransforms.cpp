#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>
#include <mirv/Core/Utility/Printer.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/AddressConstant.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Reference.hpp>

#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>

#include <iostream>

namespace mirv {
  namespace Builder {
    namespace detail {
      ptr<const Symbol<Type<TypeBase> > >
      IntegralTypeGen::operator()(ptr<SymbolTable> symtab,
                                  size_t bitsize)
      {
        return UnaryConstructSymbol<Symbol<Type<Integral> >, ModuleScope>()(
          symtab,
          bitsize);
      }

      ptr<const Symbol<Type<TypeBase> > >
      FloatingTypeGen::operator()(ptr<SymbolTable> symtab,
                                  size_t bitsize)
      {
        return UnaryConstructSymbol<Symbol<Type<Floating> >, ModuleScope>()(
          symtab,
          bitsize);
      }

      ptr<const Symbol<Type<TypeBase> > >
      StringTypeGen::operator()(ptr<SymbolTable> symtab,
                                const std::string &value)
      {
        ptr<Symbol<Module> > module = symtab->getModule();
        Symbol<Module>::TypeIterator intType = module->typeFind("int64");

        if (intType == module->typeEnd()) {
          ptr<const Symbol<Type<TypeBase> > > type =
            make<Symbol<Type<Integral> > >(64);
          module->typePushBack(type);
          intType = module->typeFind("int64");
          checkInvariant(intType != module->typeEnd(),
                         "Could not create int64 type!");
        }
        ptr<Symbol<Constant<std::uint64_t> > > size =
          mirv::make<Symbol<Constant<std::uint64_t> > >(*intType, value.size());

        ptr<Expression<Base> > sizeReference =
          mirv::make<Expression<Reference<Constant<Base> > > >(size);

        intType = module->typeFind("int8");

        if (intType == module->typeEnd()) {
          ptr<const Symbol<Type<TypeBase> > > type =
            make<Symbol<Type<Integral> > >(8);
          module->typePushBack(type);
          intType = module->typeFind("int8");
          checkInvariant(intType != module->typeEnd(),
                         "Could not create int8 type!");
        }

        return BinaryConstructSymbol<Symbol<Type<Tuple> >, ModuleScope>()(
          symtab, *intType, sizeReference);
      }

      ptr<Expression<Base> >
      GetCStringReference::operator()(boost::shared_ptr<SymbolTable> symtab,
                                      ptr<Expression<Base> > str)
      {
        ptr<Symbol<Module> > module = symtab->getModule();
        Symbol<Module>::TypeIterator intType = module->typeFind("int32");

        if (intType == module->typeEnd()) {
          ptr<const Symbol<Type<TypeBase> > > type =
            make<Symbol<Type<Integral> > >(32);
          module->typePushBack(type);
          intType = module->typeFind("int32");
          checkInvariant(intType != module->typeEnd(),
                         "Could not create int32 type!");
        }
        ptr<Symbol<Constant<std::uint64_t> > > zero =
          mirv::make<Symbol<Constant<std::uint64_t> > >(*intType, 0);

        ptr<Expression<Base> > zeroReference =
          mirv::make<Expression<Reference<Constant<Base> > > >(zero);

        ptr<Expression<Base> > zeroReference2 =
          mirv::make<Expression<Reference<Constant<Base> > > >(zero);

        ptr<Expression<Load> > load = safe_cast<Expression<Load> >(str);

        ptr<Expression<Base> > address =
          mirv::make<Expression<TuplePointer> >(load->getOperand(),
                                                zeroReference,
                                                zeroReference2);

        return address;
      }
    }
  
    ptr<Expression<Base> >
    AddStringConstant::operator()(boost::shared_ptr<SymbolTable> symtab,
                                  ptr<Symbol<Constant<Base> > > str)
    {
      ptr<Symbol<GlobalVariable> > temp =
        TernaryConstructSymbol<Symbol<GlobalVariable>, ModuleScope>()(
          symtab,
          "__str"
          + boost::lexical_cast<std::string>(symtab->getNextTempNum())
          + "__",
          str->type(),
          make<Expression<Reference<Constant<Base> > > >(str));

      ptr<Expression<Base> > reference =
        ConstructGlobalReference()(symtab, temp);

      return reference;
    } 

    ptr<Symbol<Constant<Base> > >
    ConstructAddressConstantSymbol::operator()(ptr<SymbolTable> symtab,
                                               ptr<Symbol<Global> > symbol)
    {
      ptr<const Symbol<Type<TypeBase> > > constantType = 
        LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(
          symtab,
          mirv::make<Symbol<Type<Pointer> > >(symbol->type()));

      typedef Constant<Address> ConstantType;

      result_type result =
        mirv::make<Symbol<ConstantType>>(constantType, symbol);

      return result;
    }
  }  
}
