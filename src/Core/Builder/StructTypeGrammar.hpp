#ifndef mirv_Core_Builder_StructTypeGrammar_hpp
#define mirv_Core_Builder_StructTypeGrammar_hpp

#include <mirv/Core/Builder/StructTypeGrammarFwd.hpp>

#include <mirv/Core/Builder/StructTypeRules.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>
#include <mirv/Core/Builder/Fusion.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>

#include <boost/proto/proto.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>

namespace mirv {
  namespace Builder {
    // namespace detail {
    //   /// This is a helper functor to translate a proto list to a list
    //   /// of IR objects.
    //   template<bool Matches>
    //   class TranslateListImpl {
    //   public:
    //     template<typename OutputIterator, typename List>
    //     void operator()(ptr<SymbolTable> symtab,
    //                     OutputIterator out,
    //                     const List &typeList) {
    //       auto Build = [] (auto ...args) -> ptr<const FunctionType> {
    //         return IRBuilder::get<FunctionType>(std::move(args)...);
    //       };

    //       using boost::fusion::invoke;
    //       using boost::fusion::transform;
    //       using boost::fusion::pop_front;

    //       TranslateToSymbol<const Type> translator(symtab);
    //       return invoke(Build,
    //                     transform(pop_front(expr),
    //                               translator));
    //       // FlattenAndTranslateFusionTypeSequence()(symtab, typeList, out);
    //     }
    //   };

    //   /// This is a helper functor to translate a single proto expression
    //   /// to a type symbol.  It handles the single struct member case.
    //   template<>
    //   class TranslateListImpl<false> {
    //   public:
    //     template<typename OutputIterator, typename List>
    //     void operator()(ptr<SymbolTable> symtab,
    //                     OutputIterator out,
    //                     const List &typeList) {
    //       *out = TranslateToSymbol<Type>(symtab)(typeList);
    //     }
    //   };

    //   /// This is a grammar action to translate the elements of a list
    //   /// to type symbols.  Various sequence types use it to construct
    //   /// their member types.
    //   class TranslateList {
    //   public:
    //     template<typename OutputIterator, typename List>
    //     void operator()(ptr<SymbolTable> symtab,
    //                     OutputIterator out,
    //                     const List &theList) {
    //       TranslateListImpl<
    //         boost::proto::matches<List, StrictTypeList>::value
    //         >()(symtab, out, theList);
    //     }
    //   };

    //   template<typename OutputIterator, typename List>
    //   void translateList(ptr<SymbolTable> symtab,
    //                      OutputIterator out,
    //                      const List &typeList) {
    //     TranslateList()(symtab, out, typeList);
    //   }
    // }

    /// This is a callable transform to construct a struct type.
    struct ConstructStructMultiTypeSymbol : boost::proto::callable {
      typedef ptr<const Type> result_type;

      // Keeping this outlined means we don't need a definition of Tuple.
      // void resolve(ptr<SymbolTable> symtab,
      //              const std::string &name,
      //              ptr<const PlaceholderType> placeholder,
      //              ptr<const Type> replacement);

      template<typename List>
      result_type operator()(ptr<SymbolTable> symtab,
                             const std::string &name,
                             const List &memberList) {
        // std::cout << "Building struct:\n";
        // boost::proto::display_expr(memberList);
        auto Build = [&name] (auto ...args) -> ptr<const TupleType> {
          return IRBuilder::getTupleType(name, args...);
        };

        using boost::fusion::invoke;
        using boost::fusion::transform;
        using boost::proto::flatten;

        TranslateToSymbol<const Type> translator(symtab);
        auto Tuple = invoke(Build,
                            transform(flatten(memberList),
                                      translator));
        // std::vector<ptr<const Type>> members;
        // detail::translateList(symtab,
        //                       std::back_inserter(members),
        //                       memberList);
        // result_type tuple = IRBuilder::getTupleType(name,
        //                                             members.begin(),
        //                                             members.end());

        auto Placeholder = IRBuilder::findPlaceholderType(name);

        checkInvariant(Placeholder, "Missing placeholder!");

        IRBuilder::ResolveTypes(Placeholder, Tuple);

        return Tuple;
      }
    };

        /// This is a callable transform to construct a struct type.
    struct ConstructStructSingleTypeSymbol : boost::proto::callable {
      typedef ptr<const Type> result_type;

      // Keeping this outlined means we don't need a definition of Tuple.
      // void resolve(ptr<SymbolTable> symtab,
      //              const std::string &name,
      //              ptr<const PlaceholderType> placeholder,
      //              ptr<const Type> replacement);

      template<typename Member>
      result_type operator()(ptr<SymbolTable> symtab,
                             const std::string &name,
                             const Member &member) {
        // std::cout << "Building struct:\n";
        // boost::proto::display_expr(member);

        TranslateToSymbol<const Type> translator(symtab);
        auto Tuple = IRBuilder::getTupleType(name, translator(member));

        auto Placeholder = IRBuilder::findPlaceholderType(name);
        checkInvariant(Placeholder, "Missing placeholder!");

        IRBuilder::ResolveTypes(Placeholder, Tuple);

        return Tuple;
      }
    };

    /// Create a Placeholder type with name <name>.
    struct AddPlaceholder : boost::proto::callable {
      typedef std::string result_type;

      result_type operator()(ptr<SymbolTable> symtab,
                             const std::string &name) {
        IRBuilder::getPlaceholderType(name);
        return name;
      }
    };

    /// Given the name of a Placeholder type, return the corresponding
    /// Placeholder type Symbol.
    struct LookupPlaceholder : boost::proto::callable {
      typedef ptr<const Type> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
                             const std::string &name);
    };

    /// This is the grammar for struct types.
    struct StructMultiTypeDefBuilder : boost::proto::when<
      StructMultiTypeDefRule,
       // LookupAndAddSymbol<Type>(
       //  boost::proto::_data,
        ConstructStructMultiTypeSymbol(
          boost::proto::_data,
          // Struct name
          AddPlaceholder(
            boost::proto::_data,
            boost::proto::_value(boost::proto::_right(boost::proto::_left))),
          // Member type list
          boost::proto::_right)
          // )
      > {};

    struct StructSingleTypeDefBuilder : boost::proto::when<
      StructSingleTypeDefRule,
       // LookupAndAddSymbol<Type>(
       //  boost::proto::_data,
        ConstructStructSingleTypeSymbol(
          boost::proto::_data,
          // Struct name
          AddPlaceholder(
            boost::proto::_data,
            boost::proto::_value(boost::proto::_right(boost::proto::_left))),
          // Member type
          boost::proto::_right)
          // )
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
    StructSingleTypeDefBuilder,
    StructMultiTypeDefBuilder
    > {};

    /// Given a struct type declaration, return its corresponding
    /// Symbol.  This may be a placeholder type.
    struct StructTypeLookupBuilder : boost::proto::when<
      StructTypeDeclRule,
      LookupSymbol<Type>(
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
