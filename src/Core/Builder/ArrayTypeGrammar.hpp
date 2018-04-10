#ifndef mirv_Core_Builder_ArrayTypeGrammar_hpp
#define mirv_Core_Builder_ArrayTypeGrammar_hpp

#include <mirv/Core/Builder/ArrayTypeGrammarFwd.hpp>

#include <mirv/Core/Builder/ArrayTypeRules.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>
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
      /// This holds dimension information for an array type.  The
      /// builder adds to the list of dimension information as it
      /// parses dimension expressions.
      class TypeSubscriptData {
      private:
        ptr<const Symbol<Type<TypeBase> > > baseType;
        typedef std::list<std::uint64_t> dimensionList;
        dimensionList dimensions;

      public:
        TypeSubscriptData(ptr<const Symbol<Type<TypeBase> > > e,
                          std::uint64_t d) 
            : baseType(e) {
          dimensions.push_front(d);
        }

        TypeSubscriptData(ptr<const Symbol<Type<TypeBase> > > e, int d) 
            : baseType(e) {
          dimensions.push_front(d);
        }

        template<typename List>
        TypeSubscriptData(ptr<const Symbol<Type<TypeBase> > > e,
                          const List &dims) 
            : baseType(e) {
          boost::fusion::for_each(
            dims,
            [this] (auto V) {
              dimensions.push_front(V);
            });
        }

        ptr<const Symbol<Type<TypeBase> > > elementType(void) const {
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

      /// Create an object representing a particular tuple dimension.
      struct CreateTypeSubscript : boost::proto::callable {
        typedef TypeSubscriptData result_type;

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<const Symbol<Type<TypeBase> > > elementType,
                               int dimension) {
          return TypeSubscriptData(elementType, dimension);
        }

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<const Symbol<Type<TypeBase> > > elementType,
                               std::uint64_t dimension) {
          return TypeSubscriptData(elementType, dimension);
        }

        template<typename Expr>
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<const Symbol<Type<TypeBase> > > elementType,
                               const Expr &dimensions) {
          return TypeSubscriptData(elementType,
                                   // Extract the integer values.
                                   boost::fusion::transform(
                                     boost::proto::flatten(dimensions),
                                     boost::proto::_value()));
        }
      };

      /// This is a grammar action to add dimension information to a
      /// list of dimensions.
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
      typedef ptr<const Symbol<Type<TypeBase> > > result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             detail::TypeSubscriptData subscripts);
    };

    /// This is a grammar to construct a tuple type with multiple
    /// dimensions.
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

    /// This is the grammar to build a tuple type dimension.
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
  
    /// This is a grammar add type dimension information to an
    /// existing list of type dimensions.
    template<typename ElementTypeBuilder,
      typename Dummy = boost::proto::callable>
    struct StrictTypeSubscriptListBuilder : boost::proto::when<
      StrictTypeSubscriptList,
      detail::AddTypeSubscript(
        boost::proto::_data,
        TypeSubscriptListBuilder<ElementTypeBuilder, Dummy>(boost::proto::_left),
        boost::proto::_value(boost::proto::_right))
      > {};
    
    /// Translate a list of array type dimensions to a list of tuple
    /// dimensions.
    template<typename ElementTypeBuilder,
      typename Dummy = boost::proto::callable>
    struct TypeSubscriptListBuilder : boost::proto::or_<
      StrictTypeSubscriptListBuilder<ElementTypeBuilder>,
      TypeSubscriptBuilder<ElementTypeBuilder>
      > {};

    /// This is the grammar to build a list of dimensions for array
    /// types.
    template<typename ElementTypeBuilder,
      typename Dummy = boost::proto::callable>
    struct TypeDimensionListBuilder : boost::proto::or_<
      TypeSubscriptListBuilder<ElementTypeBuilder>,
      MultiTypeSubscriptBuilder<ElementTypeBuilder>
      > {};

    /// This is the grammar for array types.
    struct ArrayTypeBuilder : boost::proto::when<
      ArrayTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        ConstructArrayTypeSymbol(
          boost::proto::_data,
          // TypeSubscript data
        TypeDimensionListBuilder<TypeBuilder>(boost::proto::_)))
      > {};

    /// This is the grammar for looking up array types.
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
