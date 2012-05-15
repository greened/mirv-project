#include <mirv/Core/Filter/SymbolVisitor.hpp>
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
//#include <mirv/Core/IR/FunctionType.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>

namespace mirv {
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Base> > s) {}
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<InnerSymbol> s) {
    typedef detail::VisitorBaseTypeOf<InnerSymbol>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<LeafSymbol> s) {
    typedef detail::VisitorBaseTypeOf<LeafSymbol>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Typed> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Typed>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Named> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Named>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Global> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Global>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Module> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Module>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Function> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Function>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Variable> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Variable>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<GlobalVariable> > s) {
    typedef detail::VisitorBaseTypeOfSymbol<GlobalVariable>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<Base> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<Base> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int8_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int8_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint8_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint8_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int16_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int16_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint16_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint16_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int32_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int32_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint32_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint32_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int64_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int64_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint64_t> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint64_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<float> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<float> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<double> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<double> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::string> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::string> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<Address> > > s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<Address> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
}
