#ifndef mirv_Core_Filter_SymbolVisitor_hpp
#define mirv_Core_Filter_SymbolVisitor_hpp

#include <cstdint>

#include <Visitor.hpp>
 
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>
#include <mirv/Core/IR/GlobalVariableFwd.hpp>
#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/AddressConstantFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// This is the base class for all types that visit symbols.
  struct SymbolVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	ptr<Symbol<Base> >,
	ptr<InnerSymbol>,
	ptr<LeafSymbol>,
	ptr<Symbol<Named> >,
	ptr<Symbol<Typed> >,
	ptr<Symbol<Global> >,
	ptr<Symbol<Module> >,
	ptr<Symbol<Function> >,
	ptr<Symbol<Variable> >,
	ptr<Symbol<GlobalVariable> >,
	ptr<Symbol<Constant<Base> > >,
	ptr<Symbol<Constant<std::int8_t> > >,
	ptr<Symbol<Constant<std::uint8_t> > >,
	ptr<Symbol<Constant<std::int16_t> > >,
	ptr<Symbol<Constant<std::uint16_t> > >,
	ptr<Symbol<Constant<std::int32_t> > >,
	ptr<Symbol<Constant<std::uint32_t> > >,
	ptr<Symbol<Constant<std::int64_t> > >,
	ptr<Symbol<Constant<std::uint64_t> > >,
	ptr<Symbol<Constant<float> > >,
	ptr<Symbol<Constant<double> > >,
	ptr<Symbol<Constant<std::string> > >,
	ptr<Symbol<Constant<Address> > >
	> > {
  public:
    virtual void visit(ptr<Symbol<Base> >);
    virtual void visit(ptr<LeafSymbol>);
    virtual void visit(ptr<InnerSymbol>);
    virtual void visit(ptr<Symbol<Typed> >);
    virtual void visit(ptr<Symbol<Named> >);
    virtual void visit(ptr<Symbol<Global> >);
    virtual void visit(ptr<Symbol<Module> >);
    virtual void visit(ptr<Symbol<Function> >);
    virtual void visit(ptr<Symbol<Variable> >);
    virtual void visit(ptr<Symbol<GlobalVariable> >);
    virtual void visit(ptr<Symbol<Constant<Base> > >);
    virtual void visit(ptr<Symbol<Constant<std::int8_t> > >);
    
    virtual void visit(ptr<Symbol<Constant<std::uint8_t> > >);
    
    virtual void visit(ptr<Symbol<Constant<std::int16_t> > >);
    
    virtual void visit(ptr<Symbol<Constant<std::uint16_t> > >);
    virtual void visit(ptr<Symbol<Constant<std::int32_t> > >);
    
    virtual void visit(ptr<Symbol<Constant<std::uint32_t> > >);
    virtual void visit(ptr<Symbol<Constant<std::int64_t> > >);   
    virtual void visit(ptr<Symbol<Constant<std::uint64_t> > >);
    virtual void visit(ptr<Symbol<Constant<float> > >);
    virtual void visit(ptr<Symbol<Constant<double> > >);
    virtual void visit(ptr<Symbol<Constant<std::string> > >);
    virtual void visit(ptr<Symbol<Constant<Address> > >);
  };
}

#endif
