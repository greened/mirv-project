#ifndef mirv_Core_Builder_ArrayTypeGrammar_hpp
#define mirv_Core_Builder_ArrayTypeGrammar_hpp

#include <mirv/Core/Builder/ArrayTypeGrammarFwd.hpp>

#include <mirv/Core/Builder/ArrayTypeRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/ArrayTypeFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/bind/bind.hpp>
#include <boost/proto/proto.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      class TypeSubscriptData {
      private:
        ptr<Symbol<Type<TypeBase> > >::const_type baseType;
        typedef std::list<int> dimensionList;
        dimensionList dimensions;

      public:
        TypeSubscriptData(ptr<Symbol<Type<TypeBase> > >::const_type e, int d) 
            : baseType(e) {
                dimensions.push_front(d);
        }

        template<typename List>
        TypeSubscriptData(ptr<Symbol<Type<TypeBase> > >::const_type e,
                          const List &dims) 
            : baseType(e) {
          boost::fusion::for_each(
            dims,
            boost::bind(static_cast<void(dimensionList::*)(const int &)>(
                          &dimensionList::push_front),
                        boost::ref(dimensions),
                        _1));
        }

        ptr<Symbol<Type<TypeBase> > >::const_type elementType(void) const {
          return baseType;
        }

        typedef dimensionList::iterator iterator;

        iterator begin(void) {
          return dimensions.begin();
        }
        iterator end(void) {
          return dimensions.end();
        }

        void push_front(int d) {
          dimensions.push_front(d);
        }
      };

      struct CreateTypeSubscript : boost::proto::callable {
        typedef TypeSubscriptData result_type;

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Symbol<Type<TypeBase> > >::const_type elementType,
                               int dimension) {
          return TypeSubscriptData(elementType, dimension);
        }

        template<typename Expr>
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Symbol<Type<TypeBase> > >::const_type elementType,
                               const Expr &dimensions) {
          return TypeSubscriptData(elementType,
                                   // Extract the integer values.
                                   boost::fusion::transform(
                                     boost::proto::flatten(dimensions),
                                     boost::proto::_value()));
        }
      };

      struct AddTypeSubscript : boost::proto::callable {
        typedef TypeSubscriptData result_type;

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               TypeSubscriptData &subscripts,
                               int dimension) {
          subscripts.push_front(dimension);
          return subscripts;
        }
      };
    }

    /// This is a callable transform to construct a struct type.
    struct ConstructArrayTypeSymbol : boost::proto::callable {
      typedef ptr<Symbol<Type<Array> > >::const_type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             detail::TypeSubscriptData subscripts) {
        return TernaryConstructSymbol<Symbol<Type<Array> > >()(
          symtab,
          subscripts.elementType(),
          subscripts.begin(),
          subscripts.end());
      }
    };

    struct MultiTypeSubscriptBuilder : boost::proto::when<
      MultiTypeSubscriptRule,
      detail::CreateTypeSubscript(
        boost::proto::_data,
        // Element type
        TypeBuilder(boost::proto::_left),
        // Dimensions
        boost::proto::_right)
      > {};

    struct TypeSubscriptBuilder : boost::proto::when<
      TypeSubscriptRule,
      detail::CreateTypeSubscript(
        boost::proto::_data,
        // Element type
        TypeBuilder(boost::proto::_left),
        // Index
        boost::proto::_value(boost::proto::_right))
      > {};

    // struct TypeSubscriptListBuilder;
  
    // struct StrictTypeSubscriptListBuilder : boost::proto::when<
    //   StrictTypeSubscriptList,
    //   detail::AddTypeSubscript(
    //     boost::proto::_data,
    //     TypeSubscriptListBuilder(boost::proto::_left),
    //     boost::proto::_value(boost::proto::_right))
    //   > {};

    // struct TypeSubscriptListBuilder : boost::proto::or_<
    //   //StrictTypeSubscriptListBuilder,
    //   TypeSubscriptBuilder
    //   > {};

    struct TypeDimensionListBuilder : boost::proto::or_<
      TypeSubscriptBuilder,
      MultiTypeSubscriptBuilder
      > {};

    // This is the grammar for struct types.
    struct ArrayTypeBuilder : boost::proto::when<
      ArrayTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        ConstructArrayTypeSymbol(
          boost::proto::_data,
          // TypeSubscript data
          TypeDimensionListBuilder(boost::proto::_)))
      > {};
  }
}

#endif
