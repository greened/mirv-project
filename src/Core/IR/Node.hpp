#ifndef mirv_Core_IR_Node_hpp
#define mirv_Core_IR_Node_hpp

#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/bind/bind.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/vector.hpp>

#include <algorithm>
#include <list>

namespace mirv {
  /// This is a base template that can be used as the root of various
  /// class hierarchies.
   template<typename Tag>
   class Node;

  /// This is the base class for all IR constructs.
   template<>
   class Node<Base> {
   private:
     // FIXME: This really shouldn't need to be mutable.  It is so
     // that when we add a type to a module the module can set itself
     // as the type's parent.  We really ought to pass the module to
     // the type during construction because that's the only place it
     // will ever live.
     mutable ptr<Node<Base>>::weak_type theParent;

   public:
     virtual ~Node<Base>(void);

     virtual ptr<Node<Base>>::type getSharedHandle(void) = 0;
     virtual ptr<Node<Base>>::const_type getSharedHandle(void) const = 0;

     template<typename NodeType>
     typename ptr<NodeType>::type parent(void) const {
       ptr<Node<Base> >::type result(theParent.lock());
       if (!result) {
         return typename ptr<NodeType>::type();
       }
       typename ptr<NodeType>::type casted = dyn_cast<NodeType>(result);
       if (casted) {
         return casted;
       }
       return result->template parent<NodeType>();
     }

     void setParent(ptr<Node<Base>>::weak_type parent) const {
       theParent = parent;
     }
   };
   
  /// This is a tag for all IR constructs that have child elements.
  /// It is an abstract interface that may be inherited virtually
  /// without introducing initialiation issues and contains no state
  /// so that virtual inheritance can be relatively cheap.
  template<typename Traits>
  class Inner {
  public:
    class Interface : public Traits::BaseType {
    public:
      Interface(void) {}
      template<typename Arg>
      Interface(Arg a) : Traits::BaseType(a) {}

      typedef typename Traits::const_reference const_reference;
      typedef typename Traits::value_type value_type;

      typedef typename boost::shared_ptr<typename Traits::Child> ChildPtr;
      typedef typename boost::shared_ptr<const typename Traits::Child>
      ConstChildPtr;

     typedef typename Traits::iterator iterator;
     typedef typename Traits::reverse_iterator reverse_iterator;
     typedef typename Traits::const_iterator const_iterator;
     typedef typename Traits::const_reverse_iterator const_reverse_iterator;

      typedef typename Traits::size_type size_type;

      // TODO: These need not be virtual.  Reference implementations
      // from an implementation template type.

      /// Get the start of the child sequence.
      virtual iterator begin(void) = 0;
      /// Get the start of the child sequence.
      virtual const_iterator begin(void) const = 0;
      /// Get the start of the reverse child sequence.
      virtual reverse_iterator rbegin(void) = 0;
      /// Get the start of the reverse child sequence.
      virtual const_reverse_iterator rbegin(void) const = 0;

      /// Get the end of the child sequence.
      virtual iterator end(void) = 0;
      /// Get the end of the child sequence.
      virtual const_iterator end(void) const = 0;
      /// Get the end of the reverse child sequence.
      virtual reverse_iterator rend(void) = 0;
      /// Get the end of the reverse child sequence.
      virtual const_reverse_iterator rend(void) const = 0;
 
      /// Add a child to the front of the child sequence.
      virtual void push_front(ChildPtr c) = 0;
      /// Add a child to the back of the child sequence.
      virtual void push_back(ChildPtr c) = 0;

      /// Get the front of the child sequence.
      virtual ChildPtr front(void) = 0;
      /// Get the front of the child sequence.
      virtual ConstChildPtr front(void) const = 0;

      /// Get the back of the child sequence.
      virtual ChildPtr back(void) = 0;
      /// Get the back of the child sequence.
      virtual ConstChildPtr back(void) const = 0;

      /// Get the size of the child sequence.
      virtual size_type size(void) const = 0;

      /// Return whether the child sequence is empty.
      virtual bool empty(void) const = 0;
    };
 
    typedef Interface BaseType;
    typedef typename Traits::BaseType VisitorBaseType;
    typedef boost::mpl::vector<> Properties;
 };

  /// This provides the implementation for inner IR nodes.  It
  /// contains the sequence state and should never be inherited
  /// virtually.  A class hierarchy instance should inherit this once
  /// throughout the instance.  It may inherit from Inner<> multiply.
     // TODO: TrackParent is a big hack.  Fix with the above TODO.
     template<typename Child, typename BaseType, bool TrackParent = true>
    class InnerImpl : public BaseType {
    public:
       typedef Child ChildType;
      typedef typename boost::shared_ptr<Child> ChildPtr;
      typedef typename boost::shared_ptr<const Child> ConstChildPtr;

   protected:
      typedef std::list<ChildPtr> ChildList;

   private:
      ChildList children;

   public:
      InnerImpl(void) {}
      InnerImpl(const std::string &name) : BaseType(name) {}
      InnerImpl(ChildPtr C) {
       children.push_back(C);
     }
      InnerImpl(ChildPtr C1, ChildPtr C2) {
       children.push_back(C1);
       children.push_back(C2);
     }
       template<typename Sequence>
       InnerImpl(ChildPtr C1, Sequence Children) {  
         children.push_back(C1);
         boost::fusion::for_each(Children,
                                 boost::bind(static_cast<void (ChildList::*)(const ChildPtr &)>(&ChildList::push_back), &children, _1));
       }
       template<typename InputIterator>
       InnerImpl(ChildPtr C1, InputIterator start, InputIterator end) {  
         children.push_back(C1);
         std::for_each(start, end,
                       boost::bind(static_cast<void (ChildList::*)(const ChildPtr &)>(&ChildList::push_back), &children, _1));
       }
       
     typedef typename ChildList::iterator iterator;
     typedef typename ChildList::reverse_iterator reverse_iterator;
     typedef typename ChildList::const_iterator const_iterator;
     typedef typename ChildList::const_reverse_iterator const_reverse_iterator;

      
      /// Get the start of the child sequence.
      iterator begin(void) { return(children.begin()); }
      /// Get the start of the child sequence.
      const_iterator begin(void) const { return(children.begin()); }
      /// Get the start of the reverse child sequence.
      reverse_iterator rbegin(void) { return(children.rbegin()); }
      /// Get the start of the reverse child sequence.
      const_reverse_iterator rbegin(void) const { return(children.rbegin()); }

      /// Get the end of the child sequence.
      iterator end(void) { return(children.end()); }
      /// Get the end of the child sequence.
      const_iterator end(void) const { return(children.end()); }
      /// Get the end of the reverse child sequence.
     reverse_iterator rend(void) { return(children.rend()); }
      /// Get the end of the reverse child sequence.
     const_reverse_iterator rend(void) const { return(children.rend()); }
 
      /// Add a child to the front of the child sequence.
      void push_front(ChildPtr c) {
        if (TrackParent) {
          c->setParent(this->getSharedHandle());
        }
        children.push_front(c);
      }
      /// Add a child to the back of the child sequence.
      void push_back(ChildPtr c) {
        if (TrackParent) {
          c->setParent(this->getSharedHandle());
        }
        children.push_back(c);
      }

      /// Get the front of the child sequence.
      ChildPtr front(void) {
         checkInvariant(!empty(),
			"Attempt to get operand from empty node");
         return(children.front());
      }

      /// Get the front of the child sequence.
      ConstChildPtr front(void) const {
         checkInvariant(!empty(),
			"Attempt to get operand from empty node");
         return(children.front());
      }

      /// Get the back of the child sequence.
      ChildPtr back(void) {
         checkInvariant(!empty(),
			"Attempt to get operand from empty node");
         return(children.back());
      }

      /// Get the back of the child sequence.
       ConstChildPtr back(void) const {
         checkInvariant(!empty(),
			"Attempt to get operand from empty node");
         return(children.back());
      }

      typedef typename ChildList::size_type size_type;
      /// Get the size of the child sequence.
      size_type size(void) const { return(children.size()); }

      /// Return whether the child sequence is empty.
      bool empty(void) const { return(children.empty()); }
   };

  /// This is the implementation class for IR nodes without children.
   template<typename Tag>
   class LeafImpl : public Tag {
   public:
     LeafImpl(void) {}
     template<typename A>
     LeafImpl(A a) : Tag(a) {}
   };
}

#endif
