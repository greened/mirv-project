#ifndef mirv_Core_Builder_ConstantSymbolTransforms_hpp
#define mirv_Core_Builder_ConstantSymbolTransforms_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/IR/Producers.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  class SymbolTable;
  class ValueProducer;

  namespace Builder {
    namespace detail {
      struct IntegerConstantGen {
        typedef ptr<IntegerConstant> result_type;

        result_type operator()(size_t bitsize,
                               int64_t value);
      };

      struct FloatingPointConstantGen {
        typedef ptr<FloatingPointConstant> result_type;

        result_type operator()(size_t bitsize,
                               double value);
      };

      /// This is a grammar action to generate the type for a C-style
      /// string.
      struct StringConstantGen {
        typedef ptr<TupleConstant> result_type;

        result_type operator()(const std::string &value);
      };

      /// This is a callable transform to take a string-type variable
      /// reference and return the address of the first element, to
      /// give C semantics.
      struct GetCStringReference : boost::proto::callable {
        typedef ptr<ValueProducer> result_type;

        result_type operator()(ptr<SymbolTable> symtab,
                               ptr<ValueProducer> str);
      };
    }

    /// This is a callable transform to add a string constant to a
    /// module.
    struct AddStringConstant : boost::proto::callable {
      typedef ptr<ValueProducer> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<Constant> str);
    };

    /// This is a callable transform to construct a constant symbol.
    template<
      typename ConstantGenerator,
      typename Dummy = boost::proto::callable>
    struct ConstructConstantSymbol : boost::proto::callable {
      typedef ptr<Constant> result_type;

      template<typename Expr>
      result_type operator()(ptr<SymbolTable> symtab,
			     const Expr &expr) {
        auto Size = sizeof(typename boost::proto::result_of::value<Expr>::type) * 8;
        ConstantGenerator GenConstant;

        return GenConstant(Size, boost::proto::value(expr));
      }
    };

    /// This is a callable transform to construct a constant symbol.
    /// If the symbol exists at the current module, it is an error.
    /// This specialization handles string constants since we
    /// calculate the size of a string differently.  "sizeof" won't
    /// work to get the length of a std::string.
    template<>
    struct ConstructConstantSymbol<detail::StringConstantGen,
                                   boost::proto::callable>
      : boost::proto::callable {
      typedef ptr<Constant> result_type;

      template<typename Expr>
      result_type operator()(ptr<SymbolTable> symtab,
			     const Expr &expr) {
        detail::StringConstantGen GenConstant;

        return GenConstant(boost::proto::value(expr));
      }
    };

    /// This is a callable transform to construct an address constant
    /// symbol.
    struct ConstructAddressConstantSymbol : boost::proto::callable {
      typedef ptr<Constant> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Global> symbol);
    };

    /// This is a callable transform to construct an integer constant
    /// symbol.
    template<
      std::int64_t Value,
      typename Dummy = boost::proto::callable>
    struct ConstructIntegerConstantSymbol : boost::proto::callable {
      typedef ptr<Constant> result_type;

      result_type operator()(ptr<SymbolTable> symtab) {
        // Constant type
        detail::IntegerConstantGen GenConstant;

        return GenConstant(64, Value);
      }
    };
  }
}

#endif
