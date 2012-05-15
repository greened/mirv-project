#ifndef mirv_Core_IR_Symbol_hpp
#define mirv_Core_IR_Symbol_hpp

#include <mirv/Core/IR/SymbolFwd.hpp>

#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/Inherit.hpp>
#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/IR/Visitable.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/mpl/empty_base.hpp>

#include <functional>
#include <string>
#include <sstream>

namespace mirv {
  struct SymbolVisitor;
  struct ConstSymbolVisitor;

  template<typename Tag> class Symbol;  

  /// This is the symbol implementation for all symbol types.  Each
  /// symbol type is an instance of this template (Symbol<Variable>,
  /// Symbol<Function>, etc.).  It keeps all of the property and
  /// visitor logic in one place, hiding the gory details from the
  /// symbol type tags and specific symbol type interfaces.
  template<typename Tag>
  class Symbol
      : public ConstVisitable<Symbol<Tag>, ConstSymbolVisitor, SymbolVisitor> {
  private:
    typedef ConstVisitable<
    Symbol<Tag>,
    ConstSymbolVisitor,
    SymbolVisitor
    > BaseType;

  protected:
    Symbol(void) {}
    template<typename ...Args>
    Symbol(Args ...args) : BaseType(args...) {}

  public:
    static ptr<Symbol<Tag> >
    make(void) {
      ptr<Symbol<Tag> > result(new Symbol<Tag>());
      Tag::initialize(result);
      return result;
    }

    template<typename A1>
    static ptr<Symbol<Tag> >
    make(A1 a1) {
      ptr<Symbol<Tag> > result(new Symbol<Tag>(a1));
      Tag::initialize(result);
      return result;
    }

    template<typename A1, typename A2>
    static ptr<Symbol<Tag> >
    make(A1 a1, A2 a2) {
      ptr<Symbol<Tag> > result(new Symbol<Tag>(a1, a2));
      Tag::initialize(result);
      return result;
    }

    template<typename A1, typename A2, typename A3>
    static ptr<Symbol<Tag> >
    make(A1 a1, A2 a2, A3 a3) {
      ptr<Symbol<Tag> > result(new Symbol<Tag>(a1, a2, a3));
      return result;
    }

    template<typename A1, typename A2, typename A3, typename A4>
    static ptr<Symbol<Tag> >
    make(A1 a1, A2 a2, A3 a3, A4 a4) {
      ptr<Symbol<Tag> > result(new Symbol<Tag>(a1, a2, a3, a4));
      return result;
    }

     template<typename Arg>
     static std::string getName(Arg &a) {
       return Tag::getName(a);
     }

     template<typename Arg1, typename Arg2>
     static std::string getName(Arg1 &a1, Arg2 &a2) {
       return Tag::getName(a1, a2);
     }

    template<typename Arg1, typename Arg2, typename Arg3>
    static std::string getName(Arg1 &a1, Arg2 &a2, Arg3 &a3) {
      return Tag::getName(a1, a2, a3);
     }

    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    static std::string getName(Arg1 &a1, Arg2 &a2, Arg3 &a3, Arg4 &a4) {
      return Tag::getName(a1, a2, a3, a4);
     }
  };

  /// Specialize Visitable for base Symbols.
  template<>
  class Visitable<Symbol<Base>, SymbolVisitor> : public Node<Base> {
  public:
    virtual void accept(SymbolVisitor &V);
  };

  /// Specialize ConstVisitable for base Symbols.
  template<>
  class ConstVisitable<
    Symbol<Base>,
    ConstSymbolVisitor,
    SymbolVisitor
    > : public Visitable<Symbol<Base>, SymbolVisitor> {
  public:
    using Visitable<Symbol<Base>, SymbolVisitor>::accept;

    virtual void accept(ConstSymbolVisitor &V) const;
  };

  /// A specialization for base symbols.
  template<>
  class Symbol<Base>
      : public ConstVisitable<
    Symbol<Base>,
    ConstSymbolVisitor,
    SymbolVisitor
    > {};

  /// This is a function object to allow searching on sets of symbols by name.
  template<typename SymbolTag>
  class SymbolByName :
    public std::binary_function<boost::shared_ptr<const Symbol<SymbolTag> >,
				const std::string &,
				bool> {
  private:
    typedef std::binary_function<ptr<const Symbol<SymbolTag> >,
				 const std::string &,
				 bool> BaseType;
  public:
    bool operator()(ptr<const Symbol<SymbolTag> > symbol,
		    const std::string &name) const {
      return symbol->name() == name;
    }
  };

  /// This is a function class to look up a Symbol by name.
  template<typename TypeTag>
  class SymbolByName<Type<TypeTag> > :
      public std::binary_function<boost::shared_ptr<const Symbol<Type<TypeTag> > >,
				const std::string &,
				bool> {
  private:
    typedef std::binary_function<ptr<const Symbol<Type<TypeTag> > >,
				 const std::string &,
				 bool> BaseType;
  public:
    bool operator()(ptr<const Symbol<Type<TypeTag> > > symbol,
		    const std::string &name) const {
      std::ostringstream typeName;
      print(typeName, symbol);
      return typeName.str() == name;
    }
  };

   namespace detail {
     /// A traits class to define various properties of inner
     /// expressions such as child type, iterator types and other
     /// things.
     class InnerSymbolTraits {
     public:
       typedef Symbol<Base> Child;
       typedef Symbol<Base> BaseType;

     private:
       typedef ptr<Child> ChildPtr;
       typedef std::list<ChildPtr> ChildList;

     public:
       /// Make this compatible with certain standard algorithms.
       typedef ChildPtr value_type;
       typedef const ChildPtr & const_reference;

       typedef ChildList::iterator iterator;
       typedef ChildList::reverse_iterator reverse_iterator;
       typedef ChildList::const_iterator const_iterator;
       typedef ChildList::const_reverse_iterator const_reverse_iterator;

       typedef ChildList::size_type size_type;
     };
   }

  /// This is an inner symbol abstract interface.  It exists because
  /// we need to be able to inherit virtually from inner expressions
  /// (to override push_front and push_back so they do not set parent
  /// points) but we do not want to force subclasses to explicitly
  /// initialize the inner expression object.  Separating the
  /// interface from the implementation solves that problem.
  template<>
  class Symbol<Inner<detail::InnerSymbolTraits> >
      : public virtual detail::BaseTypeOfSymbol<Inner<detail::InnerSymbolTraits> >::BaseType {};

  /// Define the base class for symbols with children.
  class InnerSymbolBase : public Symbol<Inner<detail::InnerSymbolTraits> > {};

  /// This is the implementation of inner symbols.  It is
  /// inherited from once in the hierarchy for any inner symbols.
  /// This holds the child pointers and other data necessary for inner
  /// symbols.
  // TODO: Fix TrackParent use.
  class InnerSymbol : public detail::BaseTypeOf<InnerSymbol>::BaseType {};

  /// This is a symbol with no children.
  class LeafSymbol : public detail::BaseTypeOf<LeafSymbol>::BaseType {};

  namespace detail {
    class TypedInterface : public virtual Symbol<Base> { 
    public:
      typedef ptr<const Symbol<Type<TypeBase> > > TypePtr;

    private:
      TypePtr theType;

    public:
      TypedInterface(TypePtr t) : theType(t) {};

      TypePtr type(void) const {
	return(theType);
      }
    };
  }

  /// A symbol that has a type associated with it.
  class Typed {};

  namespace detail {
    class NamedInterface : public virtual Symbol<Base> { 
    private:
      std::string the_name;

    public:
      NamedInterface(const std::string &n) : the_name(n) {};

      virtual std::string name(void) const {
	return(the_name);
      }
    };
  }

  /// A symbol that has a name associated with it.
  class Named {};

  namespace detail {
    class GlobalInterface : public Symbol<Named>,
                            public Symbol<Typed> { 
    public:
      GlobalInterface(const std::string &n, TypePtr t)
          : Symbol<Named>(n), Symbol<Typed>(t) {};

       // We need these to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(SymbolVisitor &) {
         error("Global::Interface::accept called!");
       }
       virtual void accept(ConstSymbolVisitor &) const {
         error("Global::Interface::accept called!");
       }
    };
  }

  /// A global symbol, a function or global variable.
  class Global {};
}

#include <mirv/Core/IR/Symbol.ipp>

#endif
