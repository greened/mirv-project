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
    ptr<InnerSymbol::VisitorBaseType>::const_type p =
      fast_cast<const InnerSymbol::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<LeafSymbol>::const_type s) {
    ptr<LeafSymbol::VisitorBaseType>::const_type p =
      fast_cast<const LeafSymbol::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Typed> >::const_type s) {
    ptr<Symbol<Typed>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Typed>::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Named> >::const_type s) {
    ptr<Symbol<Named>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Named>::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Global> >::const_type s) {
    ptr<Symbol<Global>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Global>::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Module> >::const_type s) {
    ptr<Symbol<Module>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Module>::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Function> >::const_type s) {
    ptr<Symbol<Function>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Function>::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type s) {
    ptr<Symbol<Variable>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Variable>::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<GlobalVariable> >::const_type s) {
    ptr<Symbol<GlobalVariable>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<GlobalVariable>::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<Base> > >::const_type s) {
    ptr<Symbol<Constant<Base> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<Base> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::int8_t> > >::const_type s) {
    ptr<Symbol<Constant<std::int8_t> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::int8_t> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::uint8_t> > >::const_type s) {
    ptr<Symbol<Constant<std::uint8_t> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::uint8_t> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::int16_t> > >::const_type s) {
    ptr<Symbol<Constant<std::int16_t> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::int16_t> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::uint16_t> > >::const_type s) {
    ptr<Symbol<Constant<std::uint16_t> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::uint16_t> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::int32_t> > >::const_type s) {
    ptr<Symbol<Constant<std::int32_t> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::int32_t> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::uint32_t> > >::const_type s) {
    ptr<Symbol<Constant<std::uint32_t> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::uint32_t> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::int64_t> > >::const_type s) {
    ptr<Symbol<Constant<std::int64_t> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::int64_t> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::uint64_t> > >::const_type s) {
    ptr<Symbol<Constant<std::uint64_t> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::uint64_t> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<float> > >::const_type s) {
    ptr<Symbol<Constant<float> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<float> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<double> > >::const_type s) {
    ptr<Symbol<Constant<double> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<double> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<std::string> > >::const_type s) {
    ptr<Symbol<Constant<std::string> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<std::string> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type ConstSymbolVisitor::visit(ptr<Symbol<Constant<Address> > >::const_type s) {
    ptr<Symbol<Constant<Address> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Constant<Address> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::const_type s) {
    ptr<Symbol<Type<TypeBase> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<TypeBase> >::VisitorBaseType>(s);
    visit(p);
  } 
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<InnerType>::const_type s) {
    ptr<InnerType::VisitorBaseType>::const_type p =
      fast_cast<const InnerType::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<LeafType>::const_type s) {
    ptr<LeafType::VisitorBaseType>::const_type p =
      fast_cast<const LeafType::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Simple> > >::const_type s) {
    ptr<Symbol<Type<Simple> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Simple> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Derived> > >::const_type s) {
    ptr<Symbol<Type<Derived> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Derived> >::VisitorBaseType>(s);
    visit(p);
  }
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Integral> > >::const_type s) {
    ptr<Symbol<Type<Integral> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Integral> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Floating> > >::const_type s) {
    ptr<Symbol<Type<Floating> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Floating> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Pointer> > >::const_type s) {
    ptr<Symbol<Type<Pointer> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Pointer> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<FunctionType> > >::const_type s) {
    ptr<Symbol<Type<FunctionType> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<FunctionType> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Tuple> > >::const_type s) {
    ptr<Symbol<Type<Tuple> >::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Type<Tuple> >::VisitorBaseType>(s);
    visit(p);
  }

  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Type<Placeholder> > >::const_type s) {
    //error("Visiting placeholder type!");
  }
}
