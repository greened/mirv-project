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
   template<typename Tag>
   class Node;

   template<>
   class Node<Base> : public boost::enable_shared_from_this<Node<Base> > {
   public:
     virtual ~Node<Base>(void);
   };

  template<typename Traits>
  class Inner {
  public:
    class interface : public Traits::base_type {
    public:
      typedef typename Traits::const_reference const_reference;

      typedef typename ptr<typename Traits::Child>::type child_ptr;
      typedef typename ptr<typename Traits::Child>::const_type const_child_ptr;

     typedef typename Traits::iterator iterator;
     typedef typename Traits::reverse_iterator reverse_iterator;
     typedef typename Traits::const_iterator const_iterator;
     typedef typename Traits::const_reverse_iterator const_reverse_iterator;

      typedef typename Traits::size_type size_type;

      virtual iterator begin(void) = 0;
      virtual const_iterator begin(void) const = 0;
      virtual reverse_iterator rbegin(void) = 0;
      virtual const_reverse_iterator rbegin(void) const = 0;

      virtual iterator end(void) = 0;
      virtual const_iterator end(void) const = 0;
      virtual reverse_iterator rend(void) = 0;
      virtual const_reverse_iterator rend(void) const = 0;
 
      virtual void push_front(child_ptr c) = 0;
      virtual void push_back(child_ptr c) = 0;

      virtual child_ptr front(void) = 0;
      virtual const_child_ptr front(void) const = 0;

      virtual child_ptr back(void) = 0;
      virtual const_child_ptr back(void) const = 0;

      virtual size_type size(void) const = 0;

      virtual bool empty(void) const = 0;
    };
 
    typedef interface base_type;
    typedef typename Traits::base_type visitor_base_type;
    typedef boost::mpl::vector<> properties;
 };

   // A node with children
    template<typename Child, typename BaseType>
    class InnerImpl : public BaseType {
    public:
      typedef typename ptr<Child>::type child_ptr;
      typedef typename ptr<Child>::const_type const_child_ptr;

   protected:
      typedef std::list<child_ptr> child_list;

   private:
      child_list children;

   public:
      InnerImpl(void) {}
      InnerImpl(const std::string &name) : BaseType(name) {}
      InnerImpl(child_ptr C) {
       children.push_back(C);
     }
     InnerImpl(child_ptr C1, child_ptr C2) {
       children.push_back(C1);
       children.push_back(C2);
     }
 
     typedef typename child_list::iterator iterator;
     typedef typename child_list::reverse_iterator reverse_iterator;
     typedef typename child_list::const_iterator const_iterator;
     typedef typename child_list::const_reverse_iterator const_reverse_iterator;

      iterator begin(void) { return(children.begin()); }
      const_iterator begin(void) const { return(children.begin()); }
      reverse_iterator rbegin(void) { return(children.rbegin()); }
      const_reverse_iterator rbegin(void) const { return(children.rbegin()); }

      iterator end(void) { return(children.end()); }
      const_iterator end(void) const { return(children.end()); }
     reverse_iterator rend(void) { return(children.rend()); }
     const_reverse_iterator rend(void) const { return(children.rend()); }
 
      void push_front(child_ptr c) {
         children.push_front(c);
      }
      void push_back(child_ptr c) {
         children.push_back(c);
      }

      child_ptr front(void) {
         check_invariant(!empty(),
                         "Attempt to get operand from empty node");
         return(children.front());
      }

      const_child_ptr front(void) const {
         check_invariant(!empty(),
                         "Attempt to get operand from empty node");
         return(children.front());
      }

      child_ptr back(void) {
         check_invariant(!empty(),
                         "Attempt to get operand from empty node");
         return(children.back());
      }

       const_child_ptr back(void) const {
         check_invariant(!empty(),
                         "Attempt to get operand from empty node");
         return(children.back());
      }

      typedef typename child_list::size_type size_type;
      size_type size(void) const { return(children.size()); }

      bool empty(void) const { return(children.empty()); }
   };

       // A node without children
   template<typename Tag>
   class LeafImpl : public Tag {
   public:
     LeafImpl(void) {}
     template<typename A>
     LeafImpl(A a) : Tag(a) {}
   };

}

#endif
