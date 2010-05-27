
#ifndef mirv_Core_IR_StructType_hpp
#define mirv_Core_IR_StructType_hpp

#include <mirv/Core/IR/Type.hpp>
#include <boost/bind.hpp>

#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_front.hpp>

namespace mirv {
  /// A struct type.  Struct types have a list of member types.
  struct StructType {
  private:
    typedef Symbol<Type<Derived> > InterfaceBaseType;

    class Interface : public InterfaceBaseType,
                      public boost::enable_shared_from_this<Symbol<Type<StructType> > > {
    public:
      typedef Symbol<Type<TypeBase> > ChildType;
      typedef boost::shared_ptr<ChildType> ChildPtr;
      typedef boost::shared_ptr<const ChildType> ConstChildPtr;

    private:
    public:
      template<typename Sequence>
      Interface(std::string &name, Sequence members)
          : InterfaceBaseType(name) {
        // Add the member types.
        //typedef typename boost::fusion::result_of::begin<Sequence>::type begType;
        //typedef typename boost::fusion::result_of::end<Sequence>::type endType;
        //unsigned dist = boost::fusion::result_of::distance<begType, endType>::type::value;
        //std::cout << dist << " members for struct " << name << '\n';

        boost::fusion::for_each(members,
                                boost::bind(&Interface::push_back,
                                            this,
                                            _1));
      }

      /// Construct a struct type with a single member.
      Interface(std::string &name, ptr<Symbol<Type<TypeBase> > >::type member)
          : InterfaceBaseType(name) {
        push_back(member);
      }

      /// Construct a struct type with no members.
      Interface(std::string &name) : InterfaceBaseType(name) {}

      BitSizeType bitsize(void) const {
        // TODO: This depends on ABI rules.
        return 0;
      }

      /// Get the start of the member type sequence.
      iterator memberBegin(void) {
        return begin();
      }
      /// Get the start of the member type sequence.
      const_iterator memberBegin(void) const {
        return begin();
      }
      /// Get the end of the member type sequence.
      iterator memberEnd(void) {
        return end();
      }
      /// Get the end of the member type sequence.
      const_iterator memberEnd(void) const {
        return end();
      }
      virtual void accept(mirv::SymbolVisitor &) {
        error("StructType::accept called");
      }
    };

  public:
    typedef Interface BaseType;
    typedef Symbol<Type<Derived> > VisitorBaseType;

    static std::string getName(std::string &name) {
      return name;
    }

    template<typename List>
    static std::string getName(std::string &name, List) {
      return getName(name);
    }
  };
}

#endif
