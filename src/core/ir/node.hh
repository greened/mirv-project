#ifndef mirv_core_ir_node_hh
#define mirv_core_ir_node_hh

#include <mirv/core/ir/base.hh>
#include <mirv/core/ir/property.hh>
#include <mirv/core/ir/inherit.hh>
#include <mirv/core/mem/heap.hh>
#include <mirv/core/util/debug.hh>

#include <boost/mpl/vector.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <list>

namespace mirv {
  /// This is a base template that can be used as the root of various
  /// class hierarchies.
   template<typename Tag>
   class Node;

  /// This is the base class for all IR constructs.
   template<>
   class Node<Base> : public boost::enable_shared_from_this<Node<Base> > {
   public:
     virtual ~Node<Base>(void);
   };

  /// This is a tag for all IR constructs that have child elements.
  /// It is an abstrat interface that may be inherited virtually
  /// without introducing initialiation issues and contains no state
  /// so that virtual inheritance can be relatively cheap.
  template<typename Traits>
  class Inner {
  public:
    class Interface : public Traits::BaseType {
    public:
      typedef typename Traits::const_reference const_reference;

      typedef typename ptr<typename Traits::Child>::type ChildPtr;
      typedef typename ptr<typename Traits::Child>::const_type ConstChildPtr;

     typedef typename Traits::iterator iterator;
     typedef typename Traits::reverse_iterator reverse_iterator;
     typedef typename Traits::const_iterator const_iterator;
     typedef typename Traits::const_reverse_iterator const_reverse_iterator;

      typedef typename Traits::size_type size_type;

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
    template<typename Child, typename BaseType>
    class InnerImpl : public BaseType {
    public:
      typedef typename ptr<Child>::type ChildPtr;
      typedef typename ptr<Child>::const_type ConstChildPtr;

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
         children.push_front(c);
      }
      /// Add a child to the back of the child sequence.
      void push_back(ChildPtr c) {
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
