#ifndef mirv_core_ir_node_hh
#define mirv_core_ir_node_hh

#include <mirv/ir/base.hh>
#include <mirv/ir/property.hh>
#include <mirv/ir/inherit.hh>
#include <mirv/mem/heap.hh>
#include <mirv/util/debug.hh>

#include <list>

namespace mirv {
   template<typename Tag>
   class Node;

   template<>
   class Node<Base> {
     virtual ~Node(void);
   };

   typedef Node<Base> BaseNode;

   // A node with children
   template<typename Child, typename BaseType = BaseNode>
   class InnerImpl : public BaseType {
   protected:
      typedef typename ptr<Child>::type child_ptr;
      typedef typename ptr<Child>::const_type const_child_ptr;
      typedef std::list<child_ptr> child_list;

   private:
      child_list children;

   public:
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
   };

}

#endif
