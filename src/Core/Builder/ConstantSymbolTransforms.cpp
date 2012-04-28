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
      ptr<Symbol<Type<TypeBase> > >::const_type
      IntegralTypeGen::operator()(ptr<SymbolTable>::type symtab,
                                  size_t bitsize)
      {
        return UnaryConstructSymbol<Symbol<Type<Integral> >, ModuleScope>()(
          symtab,
          bitsize);
      }

      ptr<Symbol<Type<TypeBase> > >::const_type
      FloatingTypeGen::operator()(ptr<SymbolTable>::type symtab,
                                  size_t bitsize)
      {
        return UnaryConstructSymbol<Symbol<Type<Floating> >, ModuleScope>()(
          symtab,
          bitsize);
      }

      ptr<Symbol<Type<TypeBase> > >::const_type
      StringTypeGen::operator()(ptr<SymbolTable>::type symtab,
                                const std::string &value)
      {
        ptr<Symbol<Module> >::type module = symtab->getModule();
        Symbol<Module>::TypeIterator intType = module->typeFind("int64");

        if (intType == module->typeEnd()) {
          ptr<Symbol<Type<TypeBase> > >::const_type type =
            make<Symbol<Type<Integral> > >(64);
          module->typePushBack(type);
          intType = module->typeFind("int64");
          checkInvariant(intType != module->typeEnd(),
                         "Could not create int64 type!");
        }
        ptr<Symbol<Constant<std::uint64_t> > >::type size =
          mirv::make<Symbol<Constant<std::uint64_t> > >(*intType, value.size());

        ptr<Expression<Base> >::type sizeReference =
          mirv::make<Expression<Reference<Constant<Base> > > >(size);

        intType = module->typeFind("int8");

        if (intType == module->typeEnd()) {
          ptr<Symbol<Type<TypeBase> > >::const_type type =
            make<Symbol<Type<Integral> > >(8);
          module->typePushBack(type);
          intType = module->typeFind("int8");
          checkInvariant(intType != module->typeEnd(),
                         "Could not create int8 type!");
        }

        return BinaryConstructSymbol<Symbol<Type<Tuple> >, ModuleScope>()(
          symtab, *intType, sizeReference);
      }

      ptr<Expression<Base> >::type
      GetCStringReference::operator()(boost::shared_ptr<SymbolTable> symtab,
                                      ptr<Expression<Base> >::type str)
      {
        ptr<Symbol<Module> >::type module = symtab->getModule();
        Symbol<Module>::TypeIterator intType = module->typeFind("int32");

        if (intType == module->typeEnd()) {
          ptr<Symbol<Type<TypeBase> > >::const_type type =
            make<Symbol<Type<Integral> > >(32);
          module->typePushBack(type);
          intType = module->typeFind("int32");
          checkInvariant(intType != module->typeEnd(),
                         "Could not create int32 type!");
        }
        ptr<Symbol<Constant<std::uint64_t> > >::type zero =
          mirv::make<Symbol<Constant<std::uint64_t> > >(*intType, 0);

        ptr<Expression<Base> >::type zeroReference =
          mirv::make<Expression<Reference<Constant<Base> > > >(zero);

        ptr<Expression<Base> >::type zeroReference2 =
          mirv::make<Expression<Reference<Constant<Base> > > >(zero);

        ptr<Expression<Load> >::type load = safe_cast<Expression<Load> >(str);

        ptr<Expression<Base> >::type address =
          mirv::make<Expression<TuplePointer> >(load->getOperand(),
                                                zeroReference,
                                                zeroReference2);

        return address;
      }
    }
  
    ptr<Expression<Base> >::type
    AddStringConstant::operator()(boost::shared_ptr<SymbolTable> symtab,
                                  ptr<Symbol<Constant<Base> > >::type str)
    {
      ptr<Symbol<GlobalVariable> >::type temp =
        TernaryConstructSymbol<Symbol<GlobalVariable>, ModuleScope>()(
          symtab,
          "__str"
          + boost::lexical_cast<std::string>(symtab->getNextTempNum())
          + "__",
          str->type(),
          make<Expression<Reference<Constant<Base> > > >(str));

      ptr<Expression<Base> >::type reference =
        ConstructGlobalReference()(symtab, temp);

      return reference;
    } 

    ptr<Symbol<Constant<Base> > >::type
    ConstructAddressConstantSymbol::operator()(ptr<SymbolTable>::type symtab,
                                               ptr<Symbol<Global> >::type symbol)
    {
      ptr<Symbol<Type<TypeBase> > >::const_type constantType = 
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
