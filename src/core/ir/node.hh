#ifndef MIRVNode_hh
#define MIRVNode_hh

#include <mirv/ir/base.hh>
#include <mirv/ir/property.hh>
#include <mirv/ir/inherit.hh>
#include <mirv/mem/heap.hh>

#include <list>

namespace MIRV {
   template<typename Tag>
   class Node;

   template<>
   class Node<Base> {};

   typedef Node<Base> BaseNode;

   // A node with children
   template<typename Child, typename BaseType = BaseNode>
   class Inner : public BaseType {
   protected:
      typedef typename ptr<Child>::type child_ptr;
      typedef typename ptr<Child>::const_type const_child_ptr;
      typedef std::list<child_ptr> child_list;

   private:
      child_list children;

   protected:
      typedef typename child_list::iterator iterator;
      typedef typename child_list::const_iterator const_iterator;

      iterator begin(void) { return(children.begin()); }
      const_iterator begin(void) const { return(children.begin()); }

      iterator end(void) { return(children.end()); }
      const_iterator end(void) const { return(children.end()); }

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

   public:
          // Allow some subclasses to make members accessible
          // Do the pain of method forwarding in one place
      template<typename Base>
      class publicize : public Base {
      public:
         typedef typename Base::child_ptr child_ptr;
         typedef typename Base::const_child_ptr const_child_ptr;

         typedef typename Base::iterator iterator;
         typedef typename Base::const_iterator const_iterator;

         iterator begin(void) { return(Base::begin()); }
         const_iterator begin(void) const { return(Base::begin()); }
      
         iterator end(void) { return(Base::end()); }
         const_iterator end(void) const { return(Base::end()); }
      
         void push_back(child_ptr c) {
            Base::push_back(c);
         }

         child_ptr front(void) {
            check_invariant(!empty(),
                            "Attempt to get operand from empty node");
            return(Base::front());
         }

         const_child_ptr front(void) const {
            check_invariant(!empty(),
                            "Attempt to get operand from empty node");
            return(Base::front());
         }

         child_ptr back(void) {
            check_invariant(!empty(),
                            "Attempt to get operand from empty node");
            return(Base::back());
         }

         const_child_ptr back(void) const {
            check_invariant(!empty(),
                            "Attempt to get operand from empty node");
            return(Base::back());
         }

         typedef typename Base::size_type size_type;
         size_type size(void) const { return(Base::size()); }

         bool empty(void) const { return(Base::empty()); }
      };
   };

       // A node without children
   template<typename Tag>
   class Leaf : public Tag {
   };

}

#endif
