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
  ConstSymbolVisitor::visit(ptr<const Symbol<Base> > s) {};

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const InnerSymbol> s) {
    typedef detail::VisitorBaseTypeOf<InnerSymbol>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const LeafSymbol> s) {
    typedef detail::VisitorBaseTypeOf<LeafSymbol>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Typed> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Typed>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Named> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Named>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Global> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Global>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Module> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Module>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Function> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Function>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Variable> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Variable>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<GlobalVariable> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<GlobalVariable>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<Base> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<Base> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::int8_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int8_t> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::uint8_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint8_t> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::int16_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int16_t> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::uint16_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint16_t> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::int32_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int32_t> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::uint32_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint32_t> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::int64_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int64_t> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::uint64_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint64_t> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<float> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<float> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<double> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<double> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<std::string> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::string> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<const Symbol<Constant<Address> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<Address> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<TypeBase> > > s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<TypeBase>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  } 
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const InnerType> s) {
    typedef detail::VisitorBaseTypeOfSymbol<InnerType>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const LeafType> s) {
    typedef detail::VisitorBaseTypeOfSymbol<LeafType>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<Simple> > > s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Simple>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<Derived> > > s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Derived>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<Integral> > > s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Integral>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<Floating> > > s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Floating>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<Pointer> > > s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Pointer>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<FunctionType> > > s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<FunctionType>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<Tuple> > > s) {
    typedef detail::VisitorBaseTypeOfTypeSymbol<Tuple>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<const Symbol<Type<Placeholder> > > s) {
    //error("Visiting placeholder type!");
  }
}
