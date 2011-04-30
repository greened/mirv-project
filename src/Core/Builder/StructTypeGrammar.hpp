#ifndef mirv_Core_Builder_StructTypeGrammar_hpp
#define mirv_Core_Builder_StructTypeGrammar_hpp

#include <mirv/Core/Builder/StructTypeGrammarFwd.hpp>

#include <mirv/Core/Builder/StructTypeRules.hpp>
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
      template<typename ResultType, bool Matches>
      class TranslateListImpl {
      public:
        template<typename List>
        ResultType operator()(boost::shared_ptr<SymbolTable> symtab,
                              const List &typeList) {
          TranslateToSymbol<Symbol<Type<TypeBase> > > translator(symtab);
          return boost::fusion::transform(
            boost::proto::flatten(typeList), translator);
        }
      };

      template<typename ResultType>
      class TranslateListImpl<ResultType, false> {
      public:
        template<typename List>
        ResultType operator()(boost::shared_ptr<SymbolTable> symtab,
                              const List &typeList) {
          return TranslateToSymbol<Symbol<Type<TypeBase> > >(symtab)(typeList);
        }
      };
      
      template<typename List>
      class TranslateList {
      public:
        typedef typename boost::mpl::if_<
        boost::proto::matches<List, StrictTypeList>,
        typename boost::fusion::result_of::transform<
          const typename boost::proto::result_of::flatten<const List>::type,
          TranslateToSymbol<Symbol<Type<TypeBase> > > >::type,
        ptr<Symbol<Type<TypeBase> > >::const_type
        >::type result_type;

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               const List &theList) {
          return TranslateListImpl<result_type,
            boost::proto::matches<List, StrictTypeList>::value>()(symtab, 
                                                                  theList);
        }
      };

      template<typename List>
      typename TranslateList<List>::result_type
      translateList(boost::shared_ptr<SymbolTable> symtab,
                    const List &typeList) {
        return TranslateList<List>()(symtab, typeList);
      }
    }

    /// This is a callable transform to construct a struct type.
    struct ConstructStructTypeSymbol : boost::proto::callable {
      typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;

      // Keeping this outlined means we don't need a definition of Tuple.
      void resolve(boost::shared_ptr<SymbolTable> symtab,
                   const std::string &name,
                   ptr<Symbol<Type<Placeholder> > >::const_type placeholder,
                   ptr<Symbol<Type<TypeBase> > >::const_type replacement);

      template<typename List>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             const std::string &name,
                             const List &memberList) {
        //std::cout << "Building struct:\n";
        //boost::proto::display_expr(memberList);
        
        ptr<Symbol<Type<Placeholder> > >::const_type placeholder =
          symtab->lookupPlaceholder(name);

        checkInvariant(placeholder, "Missing placeholder!");

        result_type tuple = UnaryConstructSymbol<Symbol<Type<Tuple> >,
          ModuleScope>()(
          symtab, detail::translateList(symtab, memberList));

        resolve(symtab, name, placeholder, tuple);

        return tuple;
      }
    };

    struct AddPlaceholder : boost::proto::callable {
      typedef std::string result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             const std::string &name) {
        symtab->addPlaceholder(name);
        return name;
      }
    };

    struct LookupPlaceholder : boost::proto::callable {
      typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             const std::string &name);
    };

    // This is the grammar for struct types.
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

    struct StructTypeDeclBuilder : boost::proto::when<
      StructTypeDeclRule,
      LookupPlaceholder(
        boost::proto::_data,
        AddPlaceholder(
          boost::proto::_data,
          // Struct name
          boost::proto::_value(boost::proto::_right)))
      > {};

    struct StructTypeBuilder : boost::proto::or_<
      StructTypeDeclBuilder,
      StructTypeDefBuilder
      > {};

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
