#ifndef mirv_Core_Builder_ConstructSymbolTransform_hpp
#define mirv_Core_Builder_ConstructSymbolTransform_hpp

#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/SymbolGrammarFwd.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <boost/proto/proto.hpp>
#include <boost/mpl/print.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/type_traits.hpp>

namespace mirv {
  namespace Builder {
    enum Scope {
      ModuleScope,
      CurrentScope
    };
    namespace detail {
      // template<typename SymbolType, Scope scope>
      // struct Lookup;

      // /// This is a grammar action to look up a symbol in the corrent
      // /// function.
      // template<typename SymbolType>
      // struct Lookup<SymbolType, CurrentScope> {
      //   typedef ptr<SymbolType> result_type;

      //   template<typename ...Args>
      //   result_type operator()(ptr<SymbolTable> symtavb, Args... a) {
      //     std::string name = SymbolType::getName(a...);
      //     return symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
      //   }
      // };

      // template<typename SymbolType>
      // struct Lookup<SymbolType, ModuleScope> {
      //   typedef ptr<SymbolType> result_type;

      //   template<typename ...Args>
      //   result_type operator()(ptr<SymbolTable> symtab,
      //                          const std::string &name,
      //                          Args... a) {
      //     return symtab->lookupAtModuleScope(name, reinterpret_cast<SymbolType *>(0));
      //   }
      // };

      // template<Scope scope>
      // struct Add;

      /// This is a grammar action to add a symbol to the current
      /// function or module.
      // template<>
      // struct Add<CurrentScope> {
      //   template<typename SymbolType>
      //   void operator()(ptr<SymbolTable> symtab, SymbolType a) {
      //     symtab->addAtCurrentScope(a);
      //   }
      // };

      /// This is a grammar action to add a symbol to a module.
      // template<>
      // struct Add<ModuleScope> {
      //   template<typename SymbolType>
      //   void operator()(ptr<SymbolTable> symtab,
      //                   SymbolType a) {
      //     symtab->addAtModuleScope(a);
      //   }
      // };
    }

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<
      typename SymbolType,
      Scope scope,
      typename Dummy = boost::proto::callable
      >
    struct UnaryConstructSymbol;

    //   : boost::proto::callable {
    //   typedef ptr<SymbolType> result_type;

    //   template<typename Arg>
    //   result_type operator()(ptr<SymbolTable> symtab,
    //                          Arg a) {
    //     std::string name = SymbolType::getName(a);

    //     // Make sure we're not already in the symbol table at the current scope.
    //     ptr<SymbolType> exists = detail::Lookup<SymbolType, scope>()(symtab, a);
    //     if (exists) {
    //       error("Symbol exists");
    //     }
    //     result_type result = IRBuilder::get<SymbolType>(a);

    //     detail::Add<scope>()(symtab, result);

    //     return result;
    //   }
    // };

    /// This is a grammar action to construct Symbols with
    /// one-argument constructors.  Specialize for types, which need
    /// to be const.
    template<>
    struct UnaryConstructSymbol<
      IntegerType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const Type> result_type;

      template<typename Arg>
      result_type operator()(ptr<SymbolTable> symtab, Arg a) {
        // Make sure we're not already in the symbol table at the current scope.
        return  IRBuilder::getIntegerType(a);
      }
    };

    template<>
    struct UnaryConstructSymbol<
      FloatingPointType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const Type> result_type;

      template<typename Arg>
      result_type operator()(ptr<SymbolTable> symtab, Arg a) {
        // Make sure we're not already in the symbol table at the current scope.
        return  IRBuilder::getFloatingPointType(a);
      }
    };

    template<>
    struct UnaryConstructSymbol<
      PointerType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const Type> result_type;

      template<typename Arg>
      result_type operator()(ptr<SymbolTable> symtab, Arg a) {
        // Make sure we're not already in the symbol table at the current scope.
        return IRBuilder::getPointerType(a);
      }
    };

    template<>
    struct UnaryConstructSymbol<
      TupleType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const Type> result_type;

      template<typename Arg>
      result_type operator()(ptr<SymbolTable> symtab, Arg a) {
        // Make sure we're not already in the symbol table at the current scope.
        return IRBuilder::getTupleType({a});
      }

      result_type operator()(ptr<SymbolTable> symtab, const std::string &N) {
        // Make sure we're not already in the symbol table at the current scope.
        auto Result =  IRBuilder::findTupleType(N);
        checkInvariant(Result != nullptr, "Cannot find tuple type");
        return Result;
      }

      result_type operator()(ptr<SymbolTable> symtab, const char *N) {
        // Make sure we're not already in the symbol table at the current scope.
        auto Result =  IRBuilder::findTupleType(N);
        checkInvariant(Result != nullptr, "Cannot find tuple type");
        return Result;
      }
    };

    template<>
    struct UnaryConstructSymbol<
      PlaceholderType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const Type> result_type;

      template<typename Arg>
      result_type operator()(ptr<SymbolTable> symtab, Arg a) {
        // Make sure we're not already in the symbol table at the current scope.
        return IRBuilder::getPlaceholderType({a});
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<
      typename SymbolType,
      Scope scope,
      typename Dummy = boost::proto::callable
      >
    struct BinaryConstructSymbol;
    //   : boost::proto::callable {
    //   typedef ptr<SymbolType> result_type;

    //   template<typename Arg1, typename Arg2>
    //   result_type operator()(ptr<SymbolTable> symtab, Arg1 a1, Arg2 a2) {
    //     std::string name = SymbolType::getName(a1, a2);

    //     // Make sure we're not already in the symbol table at the current scope.
    //     ptr<SymbolType> exists =
    //       detail::Lookup<SymbolType, scope>()(symtab, a1, a2);
    //     if (exists) {
    //       error("Symbol exists");
    //     }
    //     result_type result = IRBuilder::get<SymbolType>(a1, a2);
    //     detail::Add<scope>()(symtab, result);

    //     return result;
    //   }
    // };

    template<>
    struct BinaryConstructSymbol<
      Function,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<Function> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
        		     std::string Name,
        		     ptr<const Type> T) {
        if (IRBuilder::FindFunction(std::move(Name))) {
          error(Name + " already exists at global scope");
        }
        return IRBuilder::GetOrCreateFunction(Name, T);
      }
    };

    template<>
    struct BinaryConstructSymbol<
      GlobalVariable,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<GlobalVariable> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
        		     std::string Name,
        		     ptr<const Type> T) {
        if (IRBuilder::FindGlobalVariable(std::move(Name))) {
          error(Name + " already exists at global scope");
        }
        return IRBuilder::GetOrCreateGlobalVariable(Name, T);
      }
    };

    /// This is a grammar action to construct symbols with
    /// two-argument constructors. Specialize for types, which need to
    /// be const.
    template<>
    struct BinaryConstructSymbol<
      TupleType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const Type> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<const Type> T,
                             ptr<ValueProducer> Size) {
        return IRBuilder::getTupleType(Size, T);
      }

      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<const Type> T1,
                             ptr<const Type> T2) {
        return IRBuilder::getTupleType(T1, T2);
      }

      template<typename Arg2>
      result_type operator()(ptr<SymbolTable> symtab,
                             const std::string &Name,
        		     Arg2 a2) {
        return IRBuilder::getTupleType(Name, a2);
      }

      template<typename Arg2>
      result_type operator()(ptr<SymbolTable> symtab,
        		     std::string &&Name,
        		     Arg2 a2) {
        return IRBuilder::getTupleType(std::move(Name), a2);
      }

      template<typename Arg2>
      result_type operator()(ptr<SymbolTable> symtab,
                             const char *Name,
        		     Arg2 a2) {
        return IRBuilder::getTupleType(std::move(Name), a2);
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<
      typename SymbolType,
      Scope scope,
      typename Dummy = boost::proto::callable
      >
    struct TernaryConstructSymbol;
    //   : boost::proto::callable {
    //   typedef ptr<SymbolType> result_type;

    //   template<typename Arg1, typename Arg2, typename Arg3>
    //   result_type operator()(ptr<SymbolTable> symtab,
    //     		     const std::string &name,
    //     		     Arg2 a2,
    //                          Arg3 a3) {
    //     // Make sure we're not already in the symbol table at the current scope.
    //     ptr<Symbol> exists =
    //       detail::Lookup<SymbolType, scope>()(symtab, name, a2, a3);
    //     if (exists) {
    //       error("Symbol exists");
    //     }
    //     result_type result = IRBuilder::get<SymbolType>(name, a2, a3);
    //     detail::Add<scope>()(symtab, result);

    //     return result;
    //   }
    // };

    template<>
    struct TernaryConstructSymbol<
      GlobalVariable,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<GlobalVariable> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
        		     std::string Name,
        		     ptr<const Type> VarType,
                             ptr<Constant> Init) {
        if (IRBuilder::FindGlobalVariable(std::move(Name))) {
          error(Name + " already exists at global scope");
        }
        return IRBuilder::GetOrCreateGlobalVariable(Name, VarType, Init);
      }
    };

    /// This is a grammar action to construct Symbols with
    /// three-argument constructors.  Specialize for types, which need
    /// to be const.
    template<>
    struct TernaryConstructSymbol<
      TupleType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const TupleType> result_type;

      template<typename Arg1, typename Arg2, typename Arg3>
      result_type operator()(ptr<SymbolTable> symtab,
        		     Arg1 a1,
        		     Arg2 a2,
                             Arg3 a3) {
        return IRBuilder::getTupleType({a1, a2, a3});
      }

      template<typename Arg2, typename Arg3>
      result_type operator()(ptr<SymbolTable> symtab,
        		     const std::string &Name,
        		     Arg2 a2,
                             Arg3 a3) {
        return IRBuilder::getTupleType(Name, {a2, a3});
      }

      template<typename Arg2, typename Arg3>
      result_type operator()(ptr<SymbolTable> symtab,
        		     std::string &&Name,
        		     Arg2 a2,
                             Arg3 a3) {
        return IRBuilder::getTupleType(std::move(Name), {a2, a3});
      }

      template<typename Arg2, typename Arg3>
      result_type operator()(ptr<SymbolTable> symtab,
        		     const char *Name,
        		     Arg2 a2,
                             Arg3 a3) {
        return IRBuilder::getTupleType(std::move(Name), {a2, a3});
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<
      typename SymbolType,
      Scope scope,
      typename Dummy = boost::proto::callable
      >
    struct QuaternaryConstructSymbol;
    //   : boost::proto::callable {
    //   typedef ptr<SymbolType> result_type;

    //   template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    //   result_type operator()(ptr<SymbolTable> symtab,
    //     		     Arg1 a1,
    //     		     Arg2 a2,
    //                          Arg3 a3,
    //                          Arg4 a4) {
    //     std::string name = SymbolType::getName(a1, a2, a3, a4);

    //     // Make sure we're not already in the symbol table at the current scope.
    //     ptr<SymbolType> exists =
    //       detail::Lookup<SymbolType, scope>()(symtab, a1, a2, a3, a4);
    //     if (exists) {
    //       error("Symbol exists");
    //     }
    //     result_type result = IRBuilder::get<SymbolType>(a1, a2, a3, a4);
    //     detail::Add<scope>()(symtab, result);

    //     return result;
    //   }
    // };

    /// This is a grammar action to construct Symbols with
    /// three-argument constructors.  Specialize for types, which need
    /// to be const.
    template<>
    struct QuaternaryConstructSymbol<
      TupleType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const TupleType> result_type;

      template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(ptr<SymbolTable> symtab,
        		     Arg1 a1,
        		     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        return IRBuilder::getTupleType({a1, a2, a3, a4});
      }

      template<typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(ptr<SymbolTable> symtab,
        		     const std::string &Name,
        		     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        return IRBuilder::getTupleType(Name, {a2, a3, a4});
      }

      template<typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(ptr<SymbolTable> symtab,
        		     std::string &&Name,
        		     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        return IRBuilder::getTupleType(std::move(Name), {a2, a3, a4});
      }

      template<typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(ptr<SymbolTable> symtab,
        		     const char *Name,
        		     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        return IRBuilder::getTupleType(std::move(Name), {a2, a3, a4});
      }
    };

    template<>
    struct QuaternaryConstructSymbol<
      FunctionType,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef ptr<const FunctionType> result_type;

      template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(ptr<SymbolTable> symtab,
        		     Arg1 a1,
        		     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        return IRBuilder::getFunctionType(a1, a2, a3, a4);
      }

      template<typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(ptr<SymbolTable> symtab,
        		     const std::string &Name,
        		     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        return IRBuilder::getFunctionType(Name, a2, a3, a4);
      }

      template<typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(ptr<SymbolTable> symtab,
        		     std::string &&Name,
        		     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        return IRBuilder::getFunctionType(std::move(Name), a2, a3, a4);
      }

      template<typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(ptr<SymbolTable> symtab,
        		     const char *Name,
        		     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        return IRBuilder::getFunctionType(std::move(Name), a2, a3, a4);
      }
    };

    /// Given a GlobalVariable Symbol, construct an expression to
    /// reference its value.
    // struct ConstructGlobalReference : boost::proto::callable {
    //   typedef ptr<ValueProducer> result_type;

    //   result_type operator()(ptr<SymbolTable> symtab,
    //                          ptr<GlobalVariable> global);
    // };
  }
}

#endif
