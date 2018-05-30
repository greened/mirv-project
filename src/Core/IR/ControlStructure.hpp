#ifndef mirv_Core_IR_ControlStructure_hpp
#define mirv_Core_IR_ControlStructure_hpp

#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/NodeName.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Sequence.hpp>
#include <mirv/Core/IR/ValueConsumer.hpp>
#include <mirv/Library/List.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>

namespace mirv {
  class Expression;
  class Phi;
  class Producer;
  class Relational;
  class Producer;

  class Block : public Control {
    friend class IRBuilder;

    typedef List<ptr<Control>> StatementList;
    StatementList Stmts;

    typedef StatementList::iterator iterator;
    typedef StatementList::const_iterator const_iterator;
    typedef StatementList::reverse_iterator reverse_iterator;
    typedef StatementList::const_reverse_iterator const_reverse_iterator;

    // typedef List<ptr<Phi>> PhiList;
    // PhiList Phis;

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

    template<typename ...A>
    static ptr<Block> Make(std::string Name, A ...args) {
      return getHandle(new Block(std::move(Name), {args...}));
    }

  protected:
    Block(std::string Name, std::initializer_list<ptr<Control>> L) :
      Control(std::move(Name)), Stmts(L) {}

  public:
    virtual ~Block(void) {
      for (auto s : Statements()) {
        s.destroy();
      }
    }

    typedef TypeList<Control> VisitorBaseTypes;

    typedef StatementList::value_type value_type;

    typedef value_type ControlHandle;
    typedef ptr<const Control> ConstControlHandle;

    typedef Range<iterator> ControlRange;
    typedef Range<const_iterator> ConstControlRange;
    typedef Range<reverse_iterator> ReverseControlRange;
    typedef Range<const_reverse_iterator> ConstReverseControlRange;

    // typedef Range<PhiList::iterator> PhiRange;
    // typedef Range<PhiList::const_iterator> ConstPhiRange;

    ControlRange Statements(void) {
      return MakeRange<iterator>(Stmts.begin(), Stmts.end());
    }
    ConstControlRange Statements(void) const {
      return MakeRange<const_iterator>(Stmts.begin(), Stmts.end());
    }

    ReverseControlRange RStatements(void) {
      return MakeRange<reverse_iterator>(Stmts.rbegin(), Stmts.rend());
    }
    ConstReverseControlRange RStatements(void) const {
      return MakeRange<const_reverse_iterator>(Stmts.rbegin(), Stmts.rend());
    }

    void push_front(ptr<Control> S) {
      checkInvariant(getHandle(this) != S, "Adding block to itself!");
      Stmts.push_front(S);
    }
    void push_back(ptr<Control> S) {
      checkInvariant(getHandle(this) != S, "Adding block to itself!");
      Stmts.push_back(S);
    }

    ControlHandle back(void) {
      return Stmts.back();
    }

    ConstControlHandle back(void) const {
      return Stmts.back();
    }

    void clear(void) {
      Stmts.clear();
    }

    bool empty(void) const {
      return Stmts.empty();
    }
  };

  /// A block whose statements are guaranteed to be
  /// control-independent.
  class CIBlock : public Block {
    friend class IRBuilder;

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

    CIBlock(std::string Name, std::initializer_list<ptr<Control>> L) :
      Block(std::move(Name), L) {}

    template<typename ...A>
    static ptr<CIBlock> Make(std::string Name, A ...args) {
      return getHandle(new CIBlock(std::move(Name), {args...}));
    }

  public:
    typedef TypeList<Control> VisitorBaseTypes;
  };

  class Goto : public Control {
    friend class IRBuilder;

    ptr<Block> Target;  // Non-owning.

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

    Goto(std::string Name, ptr<Block> B) :
      Control(std::move(Name)), Target(B) {}

    template<typename ...A>
    static ptr<Goto> Make(std::string Name, ptr<Block> B) {
      return getHandle(new Goto(std::move(Name), std::move(B)));
    }

  public:
    typedef TypeList<Control> VisitorBaseTypes;

    ptr<Block> getTarget(void) {
      return Target;
    }
    ptr<const Block> getTarget(void) const {
      return Target;
    }
  };

  class Conditional : public ValueControl {
    ptr<Block> TheBlock;

  protected:
    Conditional(const std::string Name, ptr<Relational> C, ptr<Block> B) :
      ValueControl(std::move(Name), std::move(C)), TheBlock(std::move(B)) {}

  public:
    using VisitorBaseTypes = TypeList<ValueControl>;

    virtual ~Conditional(void) {
      TheBlock.destroy();
    }

    ptr<ValueProducer> getCondition(void) {
      return *Operands().begin();
    }
    ptr<const ValueProducer> getCondition(void) const {
      return *Operands().begin();
    }

    ptr<Block> getBlock(void) {
      return TheBlock;
    }
    ptr<const Block> getBlock(void) const {
      return TheBlock;
    }
  };

  /// Specify the if-then statement Interface.
  class IfThen : public Conditional {
    friend class IRBuilder;

    IfThen(const std::string Name, ptr<Relational> C, ptr<Block> S) :
      Conditional(std::move(Name), C, S) {}

    static ptr<IfThen> Make(const std::string Name,
                            ptr<Relational> C,
                            ptr<Block> T) {
      return getHandle(new IfThen(std::move(Name), C, T));
    }

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

  public:
    ptr<Block> getThenBlock(void) {
      return getBlock();
    }
    ptr<const Block> getThenBlock(void) const {
      return getBlock();
    }

  public:
    typedef TypeList<Control> VisitorBaseTypes;
  };

  class IfElse : public Conditional {
    friend class IRBuilder;

    ptr<Block> Else;

    IfElse(const std::string Name,
           ptr<Relational> C,
           ptr<Block> T,
           ptr<Block> E) :
      Conditional(std::move(Name), C, T), Else(E) {}

    ~IfElse(void) {
      Else.destroy();
    }

    static ptr<IfElse> Make(const std::string Name,
                            ptr<Relational> C,
                            ptr<Block> T,
                            ptr<Block> E) {
      return getHandle(new IfElse(std::move(Name), C, T, E));
    }

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

  public:
    typedef TypeList<Control> VisitorBaseTypes;

    ptr<Block> getThenBlock(void) {
      return getBlock();
    }
    ptr<const Block> getThenBlock(void) const {
      return getBlock();
    }

    ptr<Block> getElseBlock(void) {
      return Else;
    }
    ptr<const Block> getElseBlock(void) const {
      return Else;
    }
  };

  class DoWhile : public Conditional {
    friend class IRBuilder;

    DoWhile(const std::string Name,
            ptr<Relational> C,
            ptr<Block> S) : Conditional(std::move(Name), C, S) {}

    static ptr<DoWhile> Make(const std::string Name,
                             ptr<Relational> C,
                             ptr<Block> B) {
      return getHandle(new DoWhile(std::move(Name), C, B));
    }

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

  public:
    typedef TypeList<Control> VisitorBaseTypes;

    ptr<Block> getBody(void) {
      return getBlock();
    }
    ptr<const Block> getBody(void) const {
      return getBlock();
    }
  };

  /// Specify the case statement Interface.
  class Case : public Conditional {
    friend class IRBuilder;

    Case(const std::string Name,
         ptr<Relational> C,
         ptr<Block> S) : Conditional(std::move(Name), C, S) {}

    static ptr<Case> Make(const std::string Name,
                          ptr<Relational> C,
                          ptr<Block> B) {
      return getHandle(new Case(std::move(Name), C, B));
    }

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

  public:
    typedef TypeList<Control> VisitorBaseTypes;

    ptr<Block> getBody(void) {
      return getBlock();
    }
    ptr<const Block> getBody(void) const {
      return getBlock();
    }
  };

  /// Specify the switch statement Interface.
  class Switch : public ValueControl {
    friend class IRBuilder;

    typedef Vector<ptr<Case>> CaseList;
    CaseList TheCases;

    Switch(std::string Name,
           ptr<ValueProducer> C,
           std::initializer_list<ptr<Case>> L) :
      ValueControl(std::move(Name), std::move(C)), TheCases(L) {}

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Switch> Make(const std::string Name,
                            ptr<Relational> C,
                            std::initializer_list<ptr<Case>> L) {
      return getHandle(new Switch(std::move(Name), C, L));
    }

  public:
    typedef TypeList<ValueControl> VisitorBaseTypes;

    typedef Range<CaseList::iterator> CaseRange;
    typedef Range<CaseList::const_iterator> ConstCaseRange;

    ptr<ValueProducer> getSelector(void) {
      return *Operands().begin();
    }
    ptr<const ValueProducer> getSelector(void) const {
      return *Operands().begin();
    }


    CaseRange Cases(void) {
      return CaseRange(TheCases.begin(), TheCases.end());
    }
    ConstCaseRange Cases(void) const {
      return ConstCaseRange(TheCases.begin(), TheCases.end());
    }
  };

  class Return : public Control {
    friend class IRBuilder;

    Return(std::string Name) : Control(std::move(Name)) {};

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

    static ptr<Return> Make(std::string Name) {
      return getHandle(new Return(std::move(Name)));
    }

  public:
    typedef TypeList<Control> VisitorBaseTypes;
  };

  // Does not return a value.
  class SubroutineCall : public Control,
                         public ValueConsumer {
    friend class IRBuilder;

    template<typename ...Arguments>
    SubroutineCall(const std::string Name,
                   ptr<ValueProducer> F,
                   Arguments ...A) :
      Control(std::move(Name)), ValueConsumer{std::move(F), std::move(A)...} {}

    template<typename ...Arguments>
    static ptr<SubroutineCall> Make(const std::string Name,
                                    ptr<ValueProducer> F,
                                    Arguments ...args) {
      return getHandle(new SubroutineCall(std::move(Name),
                                          std::move(F),
                                          std::move(args)...));
    }

    void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }
    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

  public:
    typedef TypeList<Control> VisitorBaseTypes;

    using Control::accept;
    using ValueConsumer::accept;
  };

  // No side effects.
  class PureSubroutineCall : public SubroutineCall {
    friend class IRBuilder;

    template<typename Address, typename ...Arguments>
    PureSubroutineCall(const std::string Name,
                       ptr<ValueProducer> F,
                       Arguments ...A) :
      SubroutineCall(std::move(Name), std::move(F), std::move(A)...) {}

    template<typename ...Arguments>
    static ptr<PureSubroutineCall> Make(const std::string Name,
                                        ptr<ValueProducer> F,
                                        Arguments ...args) {
      return getHandle(new PureSubroutineCall(std::move(Name),
                                              std::move(F),
                                              std::move(args)...));
    }

    void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

  public:
    typedef TypeList<SubroutineCall> VisitorBaseTypes;
  };
}

#endif
