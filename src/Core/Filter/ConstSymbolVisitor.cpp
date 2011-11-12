#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/AddressConstant.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>

namespace mirv {
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Base> >::const_type s) {};

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<InnerSymbol>::const_type s) {
    typedef detail::VisitorBaseTypeOf<InnerSymbol>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<LeafSymbol>::const_type s) {
    typedef detail::VisitorBaseTypeOf<LeafSymbol>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Typed> >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Typed>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Named> >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Named>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Global> >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Global>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Module> >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Module>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Function> >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Function>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Variable>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<GlobalVariable> >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<GlobalVariable>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<Base> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<Base> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::int8_t> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int8_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::uint8_t> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint8_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::int16_t> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int16_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::uint16_t> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint16_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::int32_t> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int32_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::uint32_t> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint32_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::int64_t> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int64_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::uint64_t> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint64_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<float> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<float> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<double> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<double> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::string> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::string> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<Address> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<Address> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<TypeBase>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  } 
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<InnerType>::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<InnerType>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<LeafType>::const_type s) {
    typedef detail::VisitorBaseTypeOfSymbol<LeafType>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Simple> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Simple>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Derived> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Derived>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Integral> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Integral>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Floating> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Floating>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Pointer> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Pointer>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<FunctionType> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<FunctionType>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Tuple> > >::const_type s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Tuple>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Placeholder> > >::const_type s) {
    //error("Visiting placeholder type!");
  }
}
