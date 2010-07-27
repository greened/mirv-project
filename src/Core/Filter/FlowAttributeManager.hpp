#ifndef mirv_Core_Filter_FlowAttributeManager_hpp
#define mirv_Core_Filter_FlowAttributeManager_hpp

#include <mirv/Core/Containers/Vector.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <utility>

namespace mirv {
  /// The FlowAttributeManager tracks inherited and synthesized attributes
  /// used during flow execution.  Each visited node can query its
  /// inherited attribute and can set synthesized attributes for its
  /// parent to inspect.  Multiple attributes may be set by packaging
  /// them in an aggregate.  Attributes should be relatively
  /// lightweight.
  ///
  /// The FlowAttributeManager follows the following convention:
  ///
  /// Parent nodes own the attribute context for their child nodes.
  /// This means that upon entry to a node, we must push a new
  /// attribute context.  The parent can then set an inherited
  /// attributes for each child without reallocating an attribute
  /// context.  Child nodes send their synthesized attributes back to
  /// the parent using the attribute context the parent created.
  template<typename Inherited, typename Synthesized = Inherited>
  class FlowAttributeManager {
  private:
    /// This is the record type for recording inherited and
    /// synthesized attributes.
    class AttributeRecord {
    private:
      friend class FlowAttributeManager<Inherited, Synthesized>;

      Inherited inherited;
      typename Vector<std::pair<Synthesized, bool> >::type synthesized;
      unsigned int childNum;
      bool hasChild;

      void setSynthesizedBit(unsigned int index) {
        checkInvariant(synthesized.size() > index,
                       "Missing synthesized attribute");
        synthesized[index].second = true;
      }

    public:
      AttributeRecord(void)
          : synthesized(1, std::make_pair(Synthesized(), false)), childNum(0),
              hasChild(false) {}
      AttributeRecord(const Inherited &inh)
          : inherited(inh),
              synthesized(1, std::make_pair(Synthesized(), false)),
              childNum(0),
              hasChild(false) {}

      void enterChild(void) {
        // Reuse slot 0, which is where we put the default synthesized
        // attribute.
        if (hasChild) {
          ++childNum;
          checkInvariant(synthesized.size() == childNum,
                         "Unexpected attribute slot");
          synthesized.push_back(std::make_pair(Synthesized(), false));
        }
        hasChild = true;
      }

      void leaveChild(void) {}

      unsigned int childNumber(void) const {
        return childNum;
      }

      void setInheritedAttribute(const Inherited &inh) {
        inherited = inh;
      }

      void setSynthesizedAttribute(const Synthesized &syn) {
        if (synthesized.size() <= childNumber()) {
          synthesized.resize(childNumber(),
                             std::make_pair(Synthesized(), false));
        }
        
        synthesized[childNumber()].first = syn;
        synthesized[childNumber()].second = true;
      }

      const Inherited &getInheritedAttribute(void) const {
        return inherited;
      }

      const Synthesized &getSynthesizedAttribute(unsigned int index) const {
        checkInvariant(synthesized.size() > index,
                       "Missing synthesized attribute");
        checkInvariant(setSynthesized(index), "No synthesized attribute");
        return synthesized[index].first;
      }

      bool setSynthesized(unsigned int index) const {
        checkInvariant(synthesized.size() > index,
                       "Missing synthesized attribute");
        return synthesized[index].second;
      }
    };

    typedef typename Vector<AttributeRecord>::type AttributeStackType;
    AttributeStackType attributeStack;

    void setSynthesizedBit(unsigned int index) {
      checkInvariant(attributeStack.size() > 1, "Attribute stack underflow");
      (attributeStack.end() - 2)->setSynthesizedBit(index);
    };

  public:
    FlowAttributeManager(const Inherited &inherited) {
      attributeStack.push_back(AttributeRecord(inherited));
    }

    template<typename Manager>
    void swap(Manager &other) {
      attributeStack.swap(other.attributeStack);
    }

    void pushContext(void) {
      bool initialized = !attributeStack.empty();
      if (initialized) {
        // Entering a new child, so increment the child number of the
        // current context.
        attributeStack.back().enterChild();
      }

      if (initialized) {
        // Copy inherited attributes down by default.  We don't use
        // our getInheritedAttribute here because the new context
        // hasn't been pushed yet.
        attributeStack.push_back(AttributeRecord(attributeStack.back().
                                                 getInheritedAttribute()));
      }
      else {
        attributeStack.push_back(AttributeRecord());
      }
    }

    void popContext(void) {
      checkInvariant(attributeStack.size() > 1, "Attribute stack underflow");
      if (!(attributeStack.end() - 2)->
          setSynthesized((attributeStack.end() - 2)->childNumber())) {
        // Copy last synthesized attribute up by default.
        if (setLastSynthesizedAttribute()) {
          // Copy last synthesized attribute up by default.
          setSynthesizedAttribute(getLastSynthesizedAttribute());
        }
        else {
          // Otherwise we already created a default synthesized
          // attribute when we pushed the context.  Set the set bit so
          // the parent can access it.
          setSynthesizedBit((attributeStack.end() - 2)->childNumber());
        }
      }
      attributeStack.pop_back();
      if (!attributeStack.empty()) {
        attributeStack.back().leaveChild();
      }
    }

    /// Set an inherited attribute for the child node we are about to
    /// visit.
    void setInheritedAttribute(const Inherited &inherited) {
      checkInvariant(!attributeStack.empty(), "Attribute stack underflow");
      attributeStack.back().setInheritedAttribute(inherited);
    }

    /// Set a synthesized attribute for the parent we are about return
    /// to.  Since we pushed a child attribute context, our
    /// synthesized attribute actually goes one back from the top of
    /// the stack.
    void setSynthesizedAttribute(const Synthesized &synthesized) {
      checkInvariant(attributeStack.size() > 1, "Attribute stack underflow");
      (attributeStack.end() - 2)->setSynthesizedAttribute(synthesized);
    }

    /// Get the inherited attribute for this node, Since we pushed a
    /// child attribute context, our inherited attribute is actually
    /// one back from the top of stack.
    const Inherited &getInheritedAttribute(void) const {
      checkInvariant(attributeStack.size() > 1, "Attribute stack underflow");
      return (attributeStack.end() - 2)->getInheritedAttribute();
    }

    /// Return whether the just-visited child set a synthesized
    /// attribute.
    bool setLastSynthesizedAttribute(void) const {
      return attributeStack.back().setSynthesized(
        attributeStack.back().childNumber());
    }

    /// Get the synthesized attribute generated by the last child
    /// visited.
    const Synthesized &getLastSynthesizedAttribute(void) const {
      checkInvariant(!attributeStack.empty(), "Attribute stack underflow");
      checkInvariant(setLastSynthesizedAttribute(),
                     "No synthesized attribute to propagate");
      return attributeStack.back().getSynthesizedAttribute(
        attributeStack.back().childNumber());
    }

    /// Get the synthesized attribute generated by the child just
    /// visited.
    const Synthesized &getSynthesizedAttribute(unsigned int index) const {
      checkInvariant(!attributeStack.empty(), "Attribute stack underflow");
      checkInvariant(attributeStack.back().setSynthesized(index),
                     "No synthesized attribute");
      return attributeStack.back().getSynthesizedAttribute(index);
    }

    /// Return whether this curently-being-visited node/child set
    /// synthesized attribute.
    bool setLastSynthesizedAttributeForParent(void) const {
      checkInvariant(attributeStack.size() > 1, "Attribute stack underflow");
      return (attributeStack.end() - 2)->
        setSynthesized((attributeStack.end() - 2)->childNumber());
    }
  };
}

#endif
