#ifndef mirv_Core_Filter_SymbolVisitor_hpp
#define mirv_Core_Filter_SymbolVisitor_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Library/TypeList.hpp>

namespace mirv {
  class Symbol;
  class Module;
  class Function;
  class GlobalVariable;
  class Type;
  class DerivedType;
  class VoidType;
  class IntegerType;
  class FloatingPointType;
  class PointerType;
  class FunctionType;
  class TupleType;
  class PlaceholderType;

  /// This is the base class for all types that visit symbols.
  struct SymbolVisitor {
  public:
    virtual ~SymbolVisitor(void) {}

    virtual void visit(Symbol &);
    virtual void visit(Module &);
    virtual void visit(Function &);
    virtual void visit(GlobalVariable &);
    virtual void visit(const Type &);
    virtual void visit(const DerivedType &);
    virtual void visit(const VoidType &);
    virtual void visit(const IntegerType &);
    virtual void visit(const FloatingPointType &);
    virtual void visit(const PointerType &);
    virtual void visit(const FunctionType &);
    virtual void visit(const TupleType &);
    virtual void visit(const PlaceholderType &);
  };
}

#endif
