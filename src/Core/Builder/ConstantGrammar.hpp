#ifndef mirv_Core_Builder_ConstantGrammar_hpp
#define mirv_Core_Builder_ConstantGrammar_hpp

#include <mirv/Core/Builder/ConstantRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      struct IntegralTypeName {
        typedef std::string result_type;

        result_type operator()(size_t bitsize) {
          return "int" + boost::lexical_cast<std::string>(bitsize);
        }
      };
 
      struct FloatingTypeName {
        typedef std::string result_type;

        result_type operator()(size_t bitsize) {
          return "float"
            + boost::lexical_cast<std::string>(bitsize);
        }
      };

      struct StringTypeName {
        typedef std::string result_type;

        result_type operator()(const std::string &value) {
          std::size_t length = value.size();

          return "int8["
            + boost::lexical_cast<std::string>(length)
            + ']';
        }
      };

      /// This is a callable transform to take a string-type variable
      /// reference and return the address of the first element, to
      /// give C semantics.
      struct GetCStringReference : boost::proto::callable {
        typedef ptr<Expression<Base> >::type result_type;

        template<typename Arg, typename Expr>
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Expression<Base> >::type str) {
          return make<Expression<ArrayRef>
        }
      };
    }

    /// This is a callable transform to add a string constant to a
    /// module.
    struct AddStringConstant : boost::proto::callable {
      typedef ptr<Expression<Base> >::type result_type;

      template<typename Arg, typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             ptr<Constant<std::string>>::type str) {
        ptr<Symbol<Variable> >::type temp =
          BinaryConstructSymbol<Symbol<Variable> >()(
            symtab,
            "__str"
            + boost::lexical_cast<std::string>(symtab->getNextTempNum())
            + "__",
            str->type(),
            ModuleScope);
        ptr<Expression<Reference<Variable> > >::type constant =
          make<Expression<Reference<Constant<std::string>>>>(temp);

        return constant;
      }
    };

    /// This is a callable transform to construct a constant symbol.
    /// If the symbol exists at the current module, it is an error.
    /// This specialization handles string constants since we
    /// calculate the size of a string differently.  "sizeof" won't
    /// work to get the length of a std::string.
    template<>
    struct ConstructConstantSymbol<detail::StringTypeName,
      boost::proto::callable>
        : boost::proto::callable {
      typedef ptr<Symbol<Constant<Base>>>::type result_type;

      template<typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     const Expr &expr) {
        detail::StringTypeName typeGen;

        // Constant type
        ptr<Symbol<Type<TypeBase> > >::type constantType = 
          LookupSymbol<Symbol<Type<TypeBase> > >()(
            symtab,
            typeGen(boost::proto::value(expr)));

        typedef Constant<std::string> ConstantType;

        result_type result =
          mirv::make<Symbol<ConstantType>>(constantType,
                                           boost::proto::value(expr));

	return result;
      }
    };

    /// This is the grammar for constant symbols.
    struct ConstantBuilder : boost::proto::or_<
      boost::proto::when<
        IntegralConstantRule,
        ConstructConstantSymbol<detail::IntegralTypeName>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
            >,
      boost::proto::when<
        FloatingConstantRule,
        ConstructConstantSymbol<detail::FloatingTypeName>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
        >,
      boost::proto::when<
        StringConstantRule,
        detail::GetCStringReference(
          boost::proto::_data,
          AddStringConstant(
            boost::proto::_data,
            ConstructConstantSymbol<detail::StringTypeName>(
              boost::proto::_data,
              // Constant expression
              boost::proto::_expr)))
          >
      > {};
  }
}

#endif
