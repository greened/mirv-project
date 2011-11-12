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
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Base> >::type s) {}
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<InnerSymbol>::type s) {
    typedef detail::VisitorBaseTypeOf<InnerSymbol>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<LeafSymbol>::type s) {
    typedef detail::VisitorBaseTypeOf<LeafSymbol>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Typed> >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Typed>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Named> >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Named>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Global> >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Global>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Module> >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Module>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Function> >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Function>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Variable> >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Variable>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<GlobalVariable> >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<GlobalVariable>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<Base> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<Base> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int8_t> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int8_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint8_t> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint8_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int16_t> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int16_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint16_t> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint16_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int32_t> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int32_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint32_t> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint32_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::int64_t> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::int64_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::uint64_t> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::uint64_t> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<float> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<float> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<double> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<double> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<std::string> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<std::string> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }

  SymbolVisitor::result_type SymbolVisitor::visit(ptr<Symbol<Constant<Address> > >::type s) {
    typedef detail::VisitorBaseTypeOfSymbol<Constant<Address> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::type p = fast_cast<VisitorBaseType>(s);
    visit(p);
  }
}
