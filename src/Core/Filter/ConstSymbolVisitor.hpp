#ifndef mirv_Core_Filter_ConstSymbolVisitor_hpp
#define mirv_Core_Filter_ConstSymbolVisitor_hpp

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
  struct ConstSymbolVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	ptr<Symbol<Base> >::const_type,
	ptr<InnerSymbol>::const_type,
	ptr<LeafSymbol>::const_type,
	ptr<Symbol<Named> >::const_type,
	ptr<Symbol<Typed> >::const_type,
	ptr<Symbol<Module> >::const_type,
	ptr<Symbol<Function> >::const_type,
	ptr<Symbol<Variable> >::const_type,
	ptr<Symbol<Constant<Base> > >::const_type,
	ptr<Symbol<Constant<std::int8_t> > >::const_type,
	ptr<Symbol<Constant<std::uint8_t> > >::const_type,
	ptr<Symbol<Constant<std::int16_t> > >::const_type,
	ptr<Symbol<Constant<std::uint16_t> > >::const_type,
	ptr<Symbol<Constant<std::int32_t> > >::const_type,
	ptr<Symbol<Constant<std::uint32_t> > >::const_type,
	ptr<Symbol<Constant<std::int64_t> > >::const_type,
	ptr<Symbol<Constant<std::uint64_t> > >::const_type,
	ptr<Symbol<Constant<float> > >::const_type,
	ptr<Symbol<Constant<double> > >::const_type,
	ptr<Symbol<Type<TypeBase> > >::const_type,
	ptr<LeafType>::const_type,
	ptr<InnerType>::const_type,
	ptr<Symbol<Type<Simple> > >::const_type,
	ptr<Symbol<Type<Derived> > >::const_type,
	ptr<Symbol<Type<Integral> > >::const_type,
	ptr<Symbol<Type<Floating> > >::const_type,
	ptr<Symbol<Type<Array> > >::const_type,
	ptr<Symbol<Type<Pointer> > >::const_type,
	ptr<Symbol<Type<FunctionType> > >::const_type
	> > {
  public:
    virtual void visit(ptr<Symbol<Base> >::const_type);
    virtual void visit(ptr<LeafSymbol>::const_type);
    virtual void visit(ptr<InnerSymbol>::const_type);
    virtual void visit(ptr<Symbol<Typed> >::const_type);
    virtual void visit(ptr<Symbol<Named> >::const_type);
    virtual void visit(ptr<Symbol<Module> >::const_type);
    virtual void visit(ptr<Symbol<Function> >::const_type);
    virtual void visit(ptr<Symbol<Variable> >::const_type);
    virtual void visit(ptr<Symbol<Constant<Base> > >::const_type);
    virtual void visit(ptr<Symbol<Constant<std::int8_t> > >::const_type);
    
    virtual void visit(ptr<Symbol<Constant<std::uint8_t> > >::const_type);
    
    virtual void visit(ptr<Symbol<Constant<std::int16_t> > >::const_type);
    
    virtual void visit(ptr<Symbol<Constant<std::uint16_t> > >::const_type);
    virtual void visit(ptr<Symbol<Constant<std::int32_t> > >::const_type);
    
    virtual void visit(ptr<Symbol<Constant<std::uint32_t> > >::const_type);
    virtual void visit(ptr<Symbol<Constant<std::int64_t> > >::const_type);   
    virtual void visit(ptr<Symbol<Constant<std::uint64_t> > >::const_type);
    virtual void visit(ptr<Symbol<Constant<float> > >::const_type);
    virtual void visit(ptr<Symbol<Constant<double> > >::const_type);
    virtual void visit(ptr<Symbol<Type<TypeBase> > >::const_type);
    virtual void visit(ptr<LeafType>::const_type);
    virtual void visit(ptr<InnerType>::const_type);
    virtual void visit(ptr<Symbol<Type<Simple> > >::const_type);
    virtual void visit(ptr<Symbol<Type<Derived> > >::const_type);
    virtual void visit(ptr<Symbol<Type<Integral> > >::const_type);
    virtual void visit(ptr<Symbol<Type<Floating> > >::const_type);
    virtual void visit(ptr<Symbol<Type<Array> > >::const_type);
    virtual void visit(ptr<Symbol<Type<Pointer> > >::const_type);
    virtual void visit(ptr<Symbol<Type<FunctionType> > >::const_type);
    virtual void visit(ptr<Symbol<Type<StructType> > >::const_type);
  };
}

#endif
