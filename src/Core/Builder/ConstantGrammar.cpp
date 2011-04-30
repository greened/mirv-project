#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <mirv/Core/Builder/ConstantGrammar.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>

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
        Symbol<Module>::TypeIterator intType = module->typeFind("int32");

        if (intType == module->typeEnd()) {
          ptr<Symbol<Type<TypeBase> > >::const_type type =
            make<Symbol<Type<Integral> > >(32);
          module->typePushBack(type);
          intType = module->typeFind("int32");
          checkInvariant(intType != module->typeEnd(),
                         "Could not create int32 type!");
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

        return make<Expression<TuplePointer> >(str, zeroReference);
      }
    }
  
    ptr<Expression<Base> >::type
    AddStringConstant::operator()(boost::shared_ptr<SymbolTable> symtab,
                                  ptr<Symbol<Constant<Base> > >::type str)
    {
      ptr<Symbol<Variable> >::type temp =
        BinaryConstructSymbol<Symbol<Variable>, ModuleScope>()(
          symtab,
          "__str"
          + boost::lexical_cast<std::string>(symtab->getNextTempNum())
          + "__",
          str->type());
      ptr<Expression<Base> >::type constant =
        mirv::make<Expression<Reference<Variable> > >(temp);
      
      return constant;
    } 
  }
}
