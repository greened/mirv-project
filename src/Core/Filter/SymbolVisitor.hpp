#ifndef mirv_Core_Filter_SymbolVisitor_hpp
#define mirv_Core_Filter_SymbolVisitor_hpp

#include <cstdint>

#include <Visitor.hpp>
 
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>
#include <mirv/Core/IR/ConstantFwd.hpp>
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
	ptr<Symbol<Base> >::type,
	ptr<InnerSymbol>::type,
	ptr<LeafSymbol>::type,
	ptr<Symbol<Named> >::type,
	ptr<Symbol<Typed> >::type,
	ptr<Symbol<Global> >::type,
	ptr<Symbol<Module> >::type,
	ptr<Symbol<Function> >::type,
	ptr<Symbol<Variable> >::type,
	ptr<Symbol<Constant<Base> > >::type,
	ptr<Symbol<Constant<std::int8_t> > >::type,
	ptr<Symbol<Constant<std::uint8_t> > >::type,
	ptr<Symbol<Constant<std::int16_t> > >::type,
	ptr<Symbol<Constant<std::uint16_t> > >::type,
	ptr<Symbol<Constant<std::int32_t> > >::type,
	ptr<Symbol<Constant<std::uint32_t> > >::type,
	ptr<Symbol<Constant<std::int64_t> > >::type,
	ptr<Symbol<Constant<std::uint64_t> > >::type,
	ptr<Symbol<Constant<float> > >::type,
	ptr<Symbol<Constant<double> > >::type
	> > {
  public:
    virtual void visit(ptr<Symbol<Base> >::type);
    virtual void visit(ptr<LeafSymbol>::type);
    virtual void visit(ptr<InnerSymbol>::type);
    virtual void visit(ptr<Symbol<Typed> >::type);
    virtual void visit(ptr<Symbol<Named> >::type);
    virtual void visit(ptr<Symbol<Global> >::type);
    virtual void visit(ptr<Symbol<Module> >::type);
    virtual void visit(ptr<Symbol<Function> >::type);
    virtual void visit(ptr<Symbol<Variable> >::type);
    virtual void visit(ptr<Symbol<Constant<Base> > >::type);
    virtual void visit(ptr<Symbol<Constant<std::int8_t> > >::type);
    
    virtual void visit(ptr<Symbol<Constant<std::uint8_t> > >::type);
    
    virtual void visit(ptr<Symbol<Constant<std::int16_t> > >::type);
    
    virtual void visit(ptr<Symbol<Constant<std::uint16_t> > >::type);
    virtual void visit(ptr<Symbol<Constant<std::int32_t> > >::type);
    
    virtual void visit(ptr<Symbol<Constant<std::uint32_t> > >::type);
    virtual void visit(ptr<Symbol<Constant<std::int64_t> > >::type);   
    virtual void visit(ptr<Symbol<Constant<std::uint64_t> > >::type);
    virtual void visit(ptr<Symbol<Constant<float> > >::type);
    virtual void visit(ptr<Symbol<Constant<double> > >::type);
  };
}

#endif
