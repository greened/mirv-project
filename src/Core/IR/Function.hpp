#ifndef mirv_Core_IR_Function_hpp
#define mirv_Core_IR_Function_hpp

#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Global.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/ValueVisitor.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>
#include <mirv/Library/Vector.hpp>

namespace mirv {
  class Allocate;
  class Block;
  class IRBuilder;
  class Module;

  class Function : public Global,
                   public Symbol {
  private:
    friend class IRBuilder;
    friend class Module;

    ptr<Module> ParentModule;

    typedef Vector<ptr<Allocate>> AllocateList;

    AllocateList Allocations;
    ptr<Block> Code;

    Function(const std::string &Name, ptr<const Type> T) :
      Global(Name, T) {}

    Function(std::string &&Name, ptr<const Type> T) :
      Global(std::move(Name), T) {}

    static ptr<Function> Make(const std::string &Name, ptr<const Type> T) {
      return getHandle(new Function(Name, T));
    }
    static ptr<Function> Make(std::string &&Name, ptr<const Type> T) {
      return getHandle(new Function(std::move(Name), T));
    }

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    void acceptImpl(SymbolVisitor &V) override {
      V.visit(*this);
    }

    void setParentModule(ptr<Module> M) {
      ParentModule = M;
    }

  public:
    using Global::accept;
    using Symbol::accept;

    typedef TypeList<Global, Symbol> VisitorBaseTypes;
    //    typedef Range<AllocateList::iterator> AllocateRange;

    ptr<Module> getParentModule(void) const {
      checkInvariant(ParentModule, "No parent module");
      return ParentModule;
    }

    // AllocateRange Allocates(void) {
    //   return MakeRange(Allocations.begin(), Allocations.end());
    // }

    void setBlock(ptr<Block> C) {
      Code = C;
    }
    ptr<Block> getBlock(void) const {
      return Code;
    }

    void dump(void);
  };
}

#endif
