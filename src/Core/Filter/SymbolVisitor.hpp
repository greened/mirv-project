#ifndef mirv_Core_Filter_Visitor_hpp
#define mirv_Core_Filter_Visitor_hpp

#include <Visitor.hpp>
 
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>
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
	ptr<Symbol<Module> >::type,
	ptr<Symbol<Function> >::type,
	ptr<Symbol<Variable> >::type,
	ptr<Symbol<Type<TypeBase> > >::type,
	ptr<LeafType>::type,
	ptr<InnerType>::type,
	ptr<Symbol<Type<Simple> > >::type,
	ptr<Symbol<Type<Derived> > >::type,
	ptr<Symbol<Type<Integral> > >::type,
	ptr<Symbol<Type<Floating> > >::type,
	ptr<Symbol<Type<Array> > >::type,
	ptr<Symbol<Type<Pointer> > >::type,
	ptr<Symbol<Type<FunctionType> > >::type
	> > {
  public:
    virtual void visit(ptr<Symbol<Base> >::type);
    virtual void visit(ptr<LeafSymbol>::type);
    virtual void visit(ptr<InnerSymbol>::type);
    virtual void visit(ptr<Symbol<Typed> >::type);
    virtual void visit(ptr<Symbol<Named> >::type);
    virtual void visit(ptr<Symbol<Module> >::type);
    virtual void visit(ptr<Symbol<Function> >::type);
    virtual void visit(ptr<Symbol<Variable> >::type);
    virtual void visit(ptr<Symbol<Type<TypeBase> > >::type);
    virtual void visit(ptr<LeafType>::type);
    virtual void visit(ptr<InnerType>::type);
    virtual void visit(ptr<Symbol<Type<Simple> > >::type);
    virtual void visit(ptr<Symbol<Type<Derived> > >::type);
    virtual void visit(ptr<Symbol<Type<Integral> > >::type);
    virtual void visit(ptr<Symbol<Type<Floating> > >::type);
    virtual void visit(ptr<Symbol<Type<Array> > >::type);
    virtual void visit(ptr<Symbol<Type<Pointer> > >::type);
    virtual void visit(ptr<Symbol<Type<FunctionType> > >::type);
    virtual void visit(ptr<Symbol<Type<StructType> > >::type);
  };
}

#endif