#ifndef mirv_Core_Builder_StructTypeGrammar_hpp
#define mirv_Core_Builder_StructTypeGrammar_hpp

#include <mirv/Core/Builder/StructTypeGrammarFwd.hpp>

#include <mirv/Core/Builder/StructTypeRules.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>
#include <mirv/Core/Builder/Fusion.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/TupleTypeFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      /// This is a helper functor to translate a proto list to a list
      /// of IR objects.
      template<bool Matches>
      class TranslateListImpl {
      public:
        template<typename OutputIterator, typename List>
        void operator()(boost::shared_ptr<SymbolTable> symtab,
                        OutputIterator out,
                        const List &typeList) {
          FlattenAndTranslateFusionTypeSequence()(symtab, typeList, out);
        }
      };

      /// This is a helper functor to translate a single proto expression
      /// to a type symbol.  It handles the single struct member case.
      template<>
      class TranslateListImpl<false> {
      public:
        template<typename OutputIterator, typename List>
        void operator()(boost::shared_ptr<SymbolTable> symtab,
                        OutputIterator out,
                        const List &typeList) {
          *out = TranslateToSymbol<Symbol<Type<TypeBase> > >(symtab)(typeList);
        }
      };

      /// This is a grammar action to translate the elements of a list
      /// to type symbols.  Various sequence types use it to construct
      /// their member types.
      class TranslateList {
      public:
        template<typename OutputIterator, typename List>
        void operator()(boost::shared_ptr<SymbolTable> symtab,
                        OutputIterator out,
                        const List &theList) {
          TranslateListImpl<
            boost::proto::matches<List, StrictTypeList>::value
            >()(symtab, out, theList);
        }
      };

      template<typename OutputIterator, typename List>
      void translateList(boost::shared_ptr<SymbolTable> symtab,
                         OutputIterator out,
                         const List &typeList) {
        TranslateList()(symtab, out, typeList);
      }
    }

    /// This is a callable transform to construct a struct type.
    struct ConstructStructTypeSymbol : boost::proto::callable {
      typedef ptr<const Symbol<Type<TypeBase> > > result_type;

      // Keeping this outlined means we don't need a definition of Tuple.
      void resolve(boost::shared_ptr<SymbolTable> symtab,
                   const std::string &name,
                   ptr<const Symbol<Type<Placeholder> > > placeholder,
                   ptr<const Symbol<Type<TypeBase> > > replacement);

      template<typename List>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             const std::string &name,
                             const List &memberList) {
        // std::cout << "Building struct:\n";
        // boost::proto::display_expr(memberList);
        ptr<const Symbol<Type<Placeholder> > > placeholder =
          symtab->lookupPlaceholder(name);

        checkInvariant(placeholder, "Missing placeholder!");

        std::vector<ptr<const Symbol<Type<TypeBase> > >> members;
        detail::translateList(symtab,
                              std::back_inserter(members),
                              memberList);
        result_type tuple = BinaryConstructSymbol<Symbol<Type<Tuple> >,
          ModuleScope>()(symtab, members.begin(), members.end());

        resolve(symtab, name, placeholder, tuple);

        return tuple;
      }
    };

    /// Create a Placeholder type with name <name>.
    struct AddPlaceholder : boost::proto::callable {
      typedef std::string result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             const std::string &name) {
        symtab->addPlaceholder(name);
        return name;
      }
    };

    /// Given the name of a Placeholder type, return the corresponding
    /// Placeholder type Symbol.
    struct LookupPlaceholder : boost::proto::callable {
      typedef ptr<const Symbol<Type<TypeBase> > > result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             const std::string &name);
    };

    /// This is the grammar for struct types.
    struct StructTypeDefBuilder : boost::proto::when<
      StructTypeDefRule,
       LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        ConstructStructTypeSymbol(
          boost::proto::_data,
          // Struct name
          AddPlaceholder(
            boost::proto::_data,
            boost::proto::_value(boost::proto::_right(boost::proto::_left))),
          // Member type list
          boost::proto::_right))
      > {};

  /// Given a struct declaration, return a placeholder type for it.
  /// This will get replaced by the real struct type when we see its
  /// definition.
    struct StructTypeDeclBuilder : boost::proto::when<
      StructTypeDeclRule,
      LookupPlaceholder(
        boost::proto::_data,
        AddPlaceholder(
          boost::proto::_data,
          // Struct name
          boost::proto::_value(boost::proto::_right)))
      > {};

  /// This is the grammar to build a tuple type from a struct type.
  struct StructTypeBuilder : boost::proto::or_<
    StructTypeDeclBuilder,
    StructTypeDefBuilder
    > {};

    /// Given a struct type declaration, return its corresponding
    /// Symbol.  This may be a placeholder type.
    struct StructTypeLookupBuilder : boost::proto::when<
      StructTypeDeclRule,
      LookupSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        // Struct name
        boost::proto::_value(boost::proto::_right))
      > {};

    namespace {
      /// A struct_ "operator."  This is a protoized object that
      /// implements the function operator to construct struct types.
      const StructTerminal struct_ = {{}};
    }
  }
}

#endif
