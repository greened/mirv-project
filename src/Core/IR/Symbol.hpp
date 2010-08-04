#ifndef mirv_Core_IR_Symbol_hpp
#define mirv_Core_IR_Symbol_hpp

#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/Inherit.hpp>
#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <functional>
#include <string>

namespace mirv {
  struct SymbolVisitor;
  struct ConstSymbolVisitor;

  /// This is the symbol implementation for all symbol types.  Each
  /// symbol type is an instance of this template (Symbol<Variable>,
  /// Symbol<Function>, etc.).  It keeps all of the property and
  /// visitor logic in one place, hiding the gory details from the
  /// symbol type tags and specific symbol type interfaces.
  template<typename Tag>
  class Symbol : public Tag::BaseType {
  public:
    typedef typename Tag::BaseType BaseType;
    typedef typename Tag::VisitorBaseType VisitorBaseType;

  protected:
    Symbol(void) {}
    template<typename A1>
    Symbol(A1 a1) : BaseType(a1) {}
    template<typename A1, typename A2>
    Symbol(A1 a1, A2 a2) : BaseType(a1, a2) {}
    template<typename A1, typename A2, typename A3>
    Symbol(A1 a1, A2 a2, A3 a3) : BaseType(a1, a2, a3) {}

  public:
    static typename ptr<Symbol<Tag> >::type
    make(void) {
      return typename ptr<Symbol<Tag> >::type(new Symbol<Tag>());
    }

    template<typename A1>
    static typename ptr<Symbol<Tag> >::type
    make(A1 a1) {
      typename ptr<Symbol<Tag> >::type result(new Symbol<Tag>(a1));
      return result;
    }

    template<typename A1, typename A2>
    static typename ptr<Symbol<Tag> >::type
    make(A1 a1, A2 a2) {
      typename ptr<Symbol<Tag> >::type result(new Symbol<Tag>(a1, a2));
      return result;
    }

    template<typename A1, typename A2, typename A3>
    static typename ptr<Symbol<Tag> >::type
    make(A1 a1, A2 a2, A3 a3) {
      typename ptr<Symbol<Tag> >::type result(new Symbol<Tag>(a1, a2, a3));
      return result;
    }

    virtual void accept(SymbolVisitor &V);
    virtual void accept(ConstSymbolVisitor &V) const;

     template<typename Arg>
     static std::string getName(Arg &a) {
       return Tag::getName(a);
     }

     template<typename Arg1, typename Arg2>
     static std::string getName(Arg1 &a1, Arg2 &a2) {
       return Tag::getName(a1, a2);
     }
  };

  /// A specialization for base symbols.
  template<>
  class Symbol<Base> : public Node<Base> { 
  public:
    virtual void accept(SymbolVisitor &V);
    virtual void accept(ConstSymbolVisitor &V) const;
  };

  /// This is a function object to allow searching on sets of symbols by name.
  template<typename SymbolTag>
  class SymbolByName :
    public std::binary_function<boost::shared_ptr<Symbol<SymbolTag> >,
				const std::string &,
				bool> {
  private:
    typedef std::binary_function<typename ptr<Symbol<SymbolTag> >::type,
				 const std::string &,
				 bool> BaseType;
  public:
    bool operator()(typename ptr<Symbol<SymbolTag> >::type symbol,
		    const std::string &name) {
      return symbol->name() == name;
    }
  };

  /// This is the implementation of inner symbols.  It is
  /// inherited from once in the hierarchy for any inner symbols.
  /// This holds the child pointers and other data necessary for inner
  /// symbols.
  class InnerSymbol : public InnerImpl<Symbol<Base>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> {
  public:
    typedef Symbol<Base> VisitorBaseType;
    virtual void accept(SymbolVisitor &V);
    virtual void accept(ConstSymbolVisitor &V) const;
  };

  /// This is a symbol with no children.
  class LeafSymbol : public LeafImpl<VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> {
  public:
    typedef Symbol<Base> VisitorBaseType;
    virtual void accept(SymbolVisitor &V);
    virtual void accept(ConstSymbolVisitor &V) const;
  };

  /// A symbol that has a type associated with it.
  class Typed {
  private:
    typedef Inherit1::apply<Virtual<Symbol<Base> > >::type InterfaceBaseType;

    class Interface : public InterfaceBaseType { 
    public:
      typedef ptr<Symbol<Type<TypeBase> > >::const_type ConstTypePtr;

    private:
      ConstTypePtr theType;

    public:
      Interface(ConstTypePtr t)
	: theType(t) {};

      ConstTypePtr type(void) const {
	return(theType);
      }
      virtual void accept(mirv::SymbolVisitor &) {
	error("Typed::accept called");
      }
      virtual void accept(mirv::ConstSymbolVisitor &) const {
	error("Typed::accept called");
      }
    };

  public:
    typedef Interface BaseType;
    typedef Symbol<Base> VisitorBaseType;
  };

  /// A symbol that has a name associated with it.
  class Named {
  private:
    typedef Inherit1::apply<Virtual<Symbol<Base> > >::type InterfaceBaseType;

    class Interface : public InterfaceBaseType { 
    private:
      std::string the_name;

    public:
      Interface(const std::string &n)
	: the_name(n) {};

      const std::string &name(void) const {
	return(the_name);
      }
      virtual void accept(mirv::SymbolVisitor &) {
	error("Named::accept called");
      }
      virtual void accept(mirv::ConstSymbolVisitor &) const {
	error("Named::accept called");
      }
    };

  public:
    typedef Interface BaseType;
    typedef Symbol<Base> VisitorBaseType;
  };
}

#include <mirv/Core/IR/Symbol.ipp>

#endif
