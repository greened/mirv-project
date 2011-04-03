#ifndef mirv_Core_Builder_ArrayTypeGrammar_hpp
#define mirv_Core_Builder_ArrayTypeGrammar_hpp

#include <mirv/Core/Builder/ArrayTypeGrammarFwd.hpp>

#include <mirv/Core/Builder/ArrayTypeRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/bind/bind.hpp>
#include <boost/proto/proto.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>

#include <cstdint>

namespace mirv {
  namespace Builder {
    namespace detail {
      class TypeSubscriptData {
      private:
        ptr<Symbol<Type<TypeBase> > >::const_type baseType;
        typedef std::list<std::uint64_t> dimensionList;
        dimensionList dimensions;

      public:
        TypeSubscriptData(ptr<Symbol<Type<TypeBase> > >::const_type e,
                          std::uint64_t d) 
            : baseType(e) {
          dimensions.push_front(d);
        }

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
            boost::bind(static_cast<void(dimensionList::*)(const std::uint64_t &)>(
                          // Dimensions are listed most significant to
                          // least.  TupleType wants them least
                          // significant to most.
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

        void push_front(std::uint64_t d) {
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

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Symbol<Type<TypeBase> > >::const_type elementType,
                               std::uint64_t dimension) {
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
                               TypeSubscriptData subscripts,
                               std::uint64_t dimension) {
          subscripts.push_front(dimension);
          return subscripts;
        }

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               TypeSubscriptData subscripts,
                               int dimension) {
          subscripts.push_front(dimension);
          return subscripts;
        }
      };
    }

    /// This is a callable transform to construct a struct type.
    struct ConstructArrayTypeSymbol : boost::proto::callable {
      typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             detail::TypeSubscriptData subscripts);
    };

    template<typename ElementTypeBuilder,
      typename Dummy = boost::proto::callable>
    struct MultiTypeSubscriptBuilder : boost::proto::when<
      MultiTypeSubscriptRule,
      detail::CreateTypeSubscript(
        boost::proto::_data,
        // Element type
        ElementTypeBuilder(boost::proto::_left),
        // Dimensions
        boost::proto::_right)
      > {};

    template<typename ElementTypeBuilder,
      typename Dummy = boost::proto::callable>
    struct TypeSubscriptBuilder : boost::proto::when<
      TypeSubscriptRule,
      detail::CreateTypeSubscript(
        boost::proto::_data,
        // Element type
        ElementTypeBuilder(boost::proto::_left),
        // Index
        boost::proto::_value(boost::proto::_right))
      > {};

    template<typename ElementTypeBuilder,
      typename Dummy>
    struct TypeSubscriptListBuilder;
  
    template<typename ElementTypeBuilder,
      typename Dummy = boost::proto::callable>
    struct StrictTypeSubscriptListBuilder : boost::proto::when<
      StrictTypeSubscriptList,
      detail::AddTypeSubscript(
        boost::proto::_data,
        TypeSubscriptListBuilder<ElementTypeBuilder, Dummy>(boost::proto::_left),
        boost::proto::_value(boost::proto::_right))
      > {};
    
    template<typename ElementTypeBuilder,
      typename Dummy = boost::proto::callable>
    struct TypeSubscriptListBuilder : boost::proto::or_<
      StrictTypeSubscriptListBuilder<ElementTypeBuilder>,
      TypeSubscriptBuilder<ElementTypeBuilder>
      > {};

    template<typename ElementTypeBuilder,
      typename Dummy = boost::proto::callable>
    struct TypeDimensionListBuilder : boost::proto::or_<
      TypeSubscriptListBuilder<ElementTypeBuilder>,
      MultiTypeSubscriptBuilder<ElementTypeBuilder>
      > {};

    // This is the grammar for array types.
    struct ArrayTypeBuilder : boost::proto::when<
      ArrayTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        ConstructArrayTypeSymbol(
          boost::proto::_data,
          // TypeSubscript data
          TypeDimensionListBuilder<TypeBuilder>(boost::proto::_)))
      > {};

    // This is the grammar for looking up array types.
    struct ArrayTypeLookupBuilder : boost::proto::when<
      ArrayTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        ConstructArrayTypeSymbol(
          boost::proto::_data,
          // TypeSubscript data
          TypeDimensionListBuilder<TypeLookupBuilder>(boost::proto::_)))
      > {};
  }
}

#endif
