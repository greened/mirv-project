#ifndef mirv_Core_Filter_ConstSymbolVisitor_hpp
#define mirv_Core_Filter_ConstSymbolVisitor_hpp

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
#include <mirv/Core/IR/IntegralTypeFwd.hpp>
#include <mirv/Core/IR/FloatingTypeFwd.hpp>
#include <mirv/Core/IR/FunctionTypeFwd.hpp>
#include <mirv/Core/IR/PlaceholderTypeFwd.hpp>
#include <mirv/Core/IR/PointerTypeFwd.hpp>
#include <mirv/Core/IR/TupleTypeFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// This is the base class for all types that visit symbols.
  struct ConstSymbolVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	ptr<const Symbol<Base> >,
	ptr<const InnerSymbol>,
	ptr<const LeafSymbol>,
	ptr<const Symbol<Named> >,
	ptr<const Symbol<Typed> >,
	ptr<const Symbol<Global> >,
	ptr<const Symbol<Module> >,
	ptr<const Symbol<Function> >,
	ptr<const Symbol<Variable> >,
	ptr<const Symbol<GlobalVariable> >,
	ptr<const Symbol<Constant<Base> > >,
	ptr<const Symbol<Constant<std::int8_t> > >,
	ptr<const Symbol<Constant<std::uint8_t> > >,
	ptr<const Symbol<Constant<std::int16_t> > >,
	ptr<const Symbol<Constant<std::uint16_t> > >,
	ptr<const Symbol<Constant<std::int32_t> > >,
	ptr<const Symbol<Constant<std::uint32_t> > >,
	ptr<const Symbol<Constant<std::int64_t> > >,
	ptr<const Symbol<Constant<std::uint64_t> > >,
	ptr<const Symbol<Constant<float> > >,
	ptr<const Symbol<Constant<double> > >,
	ptr<const Symbol<Constant<std::string> > >,
	ptr<const Symbol<Constant<Address> > >,
	ptr<const Symbol<Type<TypeBase> > >,
	ptr<const LeafType>,
	ptr<const InnerType>,
	ptr<const Symbol<Type<Simple> > >,
	ptr<const Symbol<Type<Derived> > >,
	ptr<const Symbol<Type<Integral> > >,
	ptr<const Symbol<Type<Floating> > >,
	ptr<const Symbol<Type<Pointer> > >,
	ptr<const Symbol<Type<FunctionType> > >,
	ptr<const Symbol<Type<Tuple> > >
	> > {
  public:
    virtual void visit(ptr<const Symbol<Base> >);
    virtual void visit(ptr<const LeafSymbol>);
    virtual void visit(ptr<const InnerSymbol>);
    virtual void visit(ptr<const Symbol<Typed> >);
    virtual void visit(ptr<const Symbol<Named> >);
    virtual void visit(ptr<const Symbol<Global> >);
    virtual void visit(ptr<const Symbol<Module> >);
    virtual void visit(ptr<const Symbol<Function> >);
    virtual void visit(ptr<const Symbol<Variable> >);
    virtual void visit(ptr<const Symbol<GlobalVariable> >);
    virtual void visit(ptr<const Symbol<Constant<Base> > >);
    virtual void visit(ptr<const Symbol<Constant<std::int8_t> > >);
    
    virtual void visit(ptr<const Symbol<Constant<std::uint8_t> > >);
    
    virtual void visit(ptr<const Symbol<Constant<std::int16_t> > >);
    
    virtual void visit(ptr<const Symbol<Constant<std::uint16_t> > >);
    virtual void visit(ptr<const Symbol<Constant<std::int32_t> > >);
    
    virtual void visit(ptr<const Symbol<Constant<std::uint32_t> > >);
    virtual void visit(ptr<const Symbol<Constant<std::int64_t> > >);   
    virtual void visit(ptr<const Symbol<Constant<std::uint64_t> > >);
    virtual void visit(ptr<const Symbol<Constant<float> > >);
    virtual void visit(ptr<const Symbol<Constant<double> > >);
    virtual void visit(ptr<const Symbol<Constant<std::string> > >);
    virtual void visit(ptr<const Symbol<Constant<Address> > >);
    virtual void visit(ptr<const Symbol<Type<TypeBase> > >);
    virtual void visit(ptr<const LeafType>);
    virtual void visit(ptr<const InnerType>);
    virtual void visit(ptr<const Symbol<Type<Simple> > >);
    virtual void visit(ptr<const Symbol<Type<Derived> > >);
    virtual void visit(ptr<const Symbol<Type<Integral> > >);
    virtual void visit(ptr<const Symbol<Type<Floating> > >);
    virtual void visit(ptr<const Symbol<Type<Pointer> > >);
    virtual void visit(ptr<const Symbol<Type<FunctionType> > >);
    virtual void visit(ptr<const Symbol<Type<Tuple> > >);
    virtual void visit(ptr<const Symbol<Type<Placeholder> > >);
  };
}

#endif
