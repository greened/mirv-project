#ifndef MIRV_LIBRARY_VISITOR_HPP
#define MIRV_LIBRARY_VISITPR_HPP

#include <mirv/Library/Dispatch.hpp>

namespce mirv {
  // This sequencer visits a node once.
  class SingleSequencer {
  public:
    template<typename VisitorType>
    class apply {
    public:
      template<, typename ...Args>
      static ReturnType sequence(VisitorType *visitor,
                                 typename VisitorType::ObjectType object,
                                 typename VisitorType::ObjectType::Kind K,
                                 Args ...args) {
        return Dispatcher<typename VisitorType::ReturnType,
                          typename VisitorType::VisitDispatcher,
                          typename VisitorType::ObjectType::Kinds>::
          dispatch(std::make_tuple(visitor, object), K, args...);
      }
    };
  };

  // This sequencer potentially visits a node more than once, each
  // type as a different subkind.  The order of subtype visitation is
  // not guaranteed.
  class SubkindSequencer {
  private:
      template<typename Object, typename ReturnType, typename T>
      class SubkindDispatcher {};

  public:
    template<typename VisitorType>
    class apply {
    public:
      template<typename ...Args>
      static ReturnType sequence(VisitorType *visitor,
                                 typename VisitorType::ObjectType object,
                                 typename VisitorType::ObjectType::Kind K,
                                 Args ...args) {
        // Visit as the "current" kind.
        SingleSequenceer::template apply<VisitorType>::
          sequence(object, K, args...);

        // Visit as the subkinds.
        return SubkindDispatcher<Object,
                                 ReturnType,
                                 typename Object::VisitKinds::type>::
          dispatch(visitor, object, args...);
      }
    };
  };

  template<typename Object,
           typename ReturnType,
           typename Object::kind K,
           typename Object::kind ...Rest>
    class SubkindSequencer::SubkindDispatcher<Object,
                                              KindTuple<Object, K, Rest...>> {
  public:
    template<typename VisitorType, typename ...Args>
      static ReturnType
      dispatch(VisitorType *visitor, Object object, Args ...args) {
      // Visit as the first kind K and its subkinds.
      SubkindSequencer::template apply<VisitorType>::
        sequence(visitor, object, K, args...);

      // Visit as the rest of the kinds Rest and their subkinds.
      return SubkindSequencer<Object, ReturnType, KindTuple<Object, Rest...>::
        dispatch(visitor, object, args...);
    }
  };

  template<typename Object, typename ReturnType, typename Object::kind K>
    class SubkindSequencer::SubkindDispatcher<Object, KindTuple<Object, K>> {
  public:
    template<typename VisitorType, typename ...Args>
      static ReturnType
      dispatch(VisitorType *visitor, Object object, Args ...args) {
      // Visit as the last subkind K and its subkinds.
      return SubkindSequencer::template apply<VisitorType>::
        sequence(visitor, object, K, args...);
    }
  };

  // U is to distinguish among different kinds of visitors for the same
  // object type.
  template<typename Object,
           typename VisitorKind,
           typename Return,
           typename Sequencer,
           typename ...Args>
    class Visitor {
  public:
    typedef Object ObjectType;
    typedef VisitorKind VisitorKindType;
    typedef Return ReturnType;
    typedef Sequencer SequencerType;

  private:
    typedef Visitor<ObjectType,
                    VisitorKindType,
                    ReturnType,
                    SequencerType,
                    Args...> This;

    // This dispatcher expects a single object so wrap the visitor and
    // the visited object together.
    typedef std::tuple<This *, Object> Payload;

    class VisitDispatcher {
    public:
      template<typename ObjectType::kind K>
      static ReturnType dispatch(typename Payload object,
                                 Args ...args) {
        return std::get<0>(object)->
          template visitImpl<K>(std::get<1>(object), args...);
      }
    };

    template<typename ObjectType::Kind K>
    ReturnType visitImpl(ObjectType object, Args ...args) {}

  public:
    ReturnType visit(ObjectType object, Args ...args) {
      return SequencerType::apply<This>::sequence(this,
                                                  object,
                                                  object.kind(),
                                                  args...);
    }
  };
}

#endif
