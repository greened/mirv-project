#ifndef mirv_Core_Filter_FlowAttributeManager_hpp
#define mirv_Core_Filter_FlowAttributeManager_hpp

#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Library/Vector.hpp>

#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

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

      Inherited inheritedPrototype;
      Synthesized synthesizedPrototype;
      typedef std::pair<Synthesized, bool> AttributePair;
      typedef Vector<AttributePair> SynthesizedVector;

      Inherited inherited;
      SynthesizedVector synthesized;

      unsigned int childNum;
      bool hasChild;

      void setSynthesizedBit(unsigned int index) {
        checkInvariant(synthesized.size() > index,
                       "Missing synthesized attribute");
        synthesized[index].second = true;
      }

      template<typename Pair>
      struct select1st
          : public std::unary_function<Pair, typename Pair::first_type> {
        typename Pair::first_type&
        operator()(Pair &x) const { return x.first; }

        const typename Pair::first_type&
        operator()(const Pair &x) const { return x.first; }
      };

      template<typename Pair>
      struct select2nd
          : public std::unary_function<Pair, typename Pair::second_type> {
        typename Pair::second_type&
        operator()(Pair &x) const { return x.second; }

        const typename Pair::second_type&
        operator()(const Pair &x) const { return x.second; }
      };

      typedef boost::filter_iterator<select2nd<AttributePair>,
      typename SynthesizedVector::iterator> filter_iterator;

      typedef boost::filter_iterator<select2nd<AttributePair>,
      typename SynthesizedVector::const_iterator> const_filter_iterator;

    public:
      typedef boost::transform_iterator<select1st<AttributePair>,
      filter_iterator> iterator;
      typedef boost::transform_iterator<select1st<AttributePair>,
        const_filter_iterator> const_iterator;

      AttributeRecord(const Inherited &inh, const Synthesized &syn)
          : inheritedPrototype(inh),
            synthesizedPrototype(syn),
            inherited(inh),
            synthesized(1, std::make_pair(synthesizedPrototype, false)),
            childNum(0),
            hasChild(false) {}

      void enterChild(void) {
        // Reuse slot 0, which is where we put the default synthesized
        // attribute.
        if (hasChild) {
          ++childNum;
          checkInvariant(synthesized.size() == childNum,
                         "Unexpected attribute slot");
          synthesized.push_back(std::make_pair(synthesizedPrototype, false));
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
                             std::make_pair(synthesizedPrototype, false));
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

      iterator begin(void) {
        return boost::make_transform_iterator(
          boost::make_filter_iterator(select2nd<AttributePair>(),
                                      synthesized.begin(),
                                      synthesized.end()),
          select1st<AttributePair>());
      }

      const_iterator begin(void) const {
        return boost::make_transform_iterator(
          boost::make_filter_iterator(select2nd<AttributePair>(),
                                      synthesized.begin(),
                                      synthesized.end()),
          select1st<AttributePair>());
      }

      iterator end(void) {
        return boost::make_transform_iterator(
          boost::make_filter_iterator(select2nd<AttributePair>(),
                                      synthesized.end(),
                                      synthesized.end()),
          select1st<AttributePair>());
      }

      const_iterator end(void) const {
        return boost::make_transform_iterator(
          boost::make_filter_iterator(select2nd<AttributePair>(),
                                      synthesized.end(),
                                      synthesized.end()),
          select1st<AttributePair>());
      }
    };

    typedef Vector<AttributeRecord> AttributeStackType;
    AttributeStackType attributeStack;

    Inherited inheritedPrototype;
    Synthesized synthesizedPrototype;

    void setSynthesizedBit(unsigned int index) {
      checkInvariant(attributeStack.size() > 1, "Attribute stack underflow");
      (attributeStack.end() - 2)->setSynthesizedBit(index);
    };

  public:
    typedef typename AttributeRecord::iterator iterator;
    typedef typename AttributeRecord::const_iterator const_iterator;

    FlowAttributeManager(const Inherited &inherited,
                         const Synthesized &synthesized)
      : inheritedPrototype(inherited), synthesizedPrototype(synthesized) {
      attributeStack.push_back(AttributeRecord(inherited,
                                               synthesized));
    }

    template<typename Manager>
    void swap(Manager &other) {
      attributeStack.swap(other.attributeStack);
    }

    const Inherited &getInheritedAttributePrototype(void) const {
      return inheritedPrototype;
    }

    const Synthesized &getSynthesizedAttributePrototype(void) const {
      return synthesizedPrototype;
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
                                                 getInheritedAttribute(),
                                                 synthesizedPrototype));
      }
      else {
        attributeStack.push_back(AttributeRecord(inheritedPrototype,
                                                 synthesizedPrototype));
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

    iterator begin(void) {
      checkInvariant(!attributeStack.empty(), "Attribute stack underflow");
      return attributeStack.back().begin();
    }

    const_iterator begin(void) const {
      checkInvariant(!attributeStack.empty(), "Attribute stack underflow");
      return attributeStack.back().begin();
    }

    iterator end(void) {
      checkInvariant(!attributeStack.empty(), "Attribute stack underflow");
      return attributeStack.back().end();
    }

    const_iterator end(void) const {
      checkInvariant(!attributeStack.empty(), "Attribute stack underflow");
      return attributeStack.back().end();
    }
  };
}

#endif
