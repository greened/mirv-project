#ifndef mirv_Core_Builder_Builder_hpp
#define mirv_Core_Builder_Builder_hpp

#include <mirv/Core/IR/Constants.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Library/Map.hpp>
#include <mirv/Library/Singleton.hpp>
#include <mirv/Library/Vector.hpp>

#include <string>

namespace mirv {
  class Function;
  class GlobalVariable;

  //class IRBuilder;
  //Singleton<IRBuilder>::Handle Singleton<IRBuilder>::inst;

  // An IRBuilder manages global IR state for a MIRV process.  It owns
  // Types and Modules.
  class IRBuilder : public Singleton<IRBuilder> {
    friend class Singleton<IRBuilder>;

    // Module management.
    Vector<ptr<Module>> Modules;
    ptr<Module> CurrentModule;
    //ptr<Function> CurrentFunction;

    ptr<Module> GetOrCreateModuleImpl(const std::string &Name);
    ptr<Module> GetOrCreateModuleImpl(std::string &&Name);

    ptr<Module> getCurrentModuleImpl(void) {
      checkInvariant(CurrentModule, "No module created");
      return CurrentModule;
    }

    void AddToCurrentModule(ptr<const Type> T);
    void AddToCurrentModule(ptr<Constant> C);
    void AddToCurrentModule(ptr<GlobalVariable> G);
    void AddToCurrentModule(ptr<Function> F);

    // Type maps.
    class TupleTypeKey {
      Vector<ptr<const Type>> ElementTypes;

    public:
      template <typename ...Args>
      TupleTypeKey(Args ...args) :
        ElementTypes({args...}) {}

      bool operator<(const TupleTypeKey &K) const {
        if (ElementTypes.size() != K.ElementTypes.size()) {
          return ElementTypes.size() < K.ElementTypes.size();
        }

        auto ThatElem = K.ElementTypes.begin();

        for (auto ThisElem : ElementTypes) {
          if (ThisElem < *ThatElem) {
            return true;
          }
          else if (*ThatElem > ThisElem) {
            return false;
          }
          ++ThatElem;
        }

        return false;
      }
    };

    class FunctionTypeKey {
      TupleTypeKey TypeKey;
      FunctionType::VarargMark Vararg;

    public:
      template<typename ...Args>
      FunctionTypeKey(FunctionType::VarargMark V,
                      Args ...args) :
        TypeKey{std::move(args)...}, Vararg(V) {}

      bool operator<(const FunctionTypeKey &K) const {
        if (Vararg != K.Vararg) {
          return !Vararg;
        }

        return TypeKey < K.TypeKey;
      }
    };

    ptr<const VoidType> Void;

    Map<IntegerType::SizeType, ptr<const IntegerType>> IntegerTypes;

    Map<FloatingPointType::SizeType,
        ptr<const FloatingPointType>> FloatingPointTypes;

    Map<std::string, ptr<const PlaceholderType>> PlaceholderTypes;

    Map<ptr<const Type>, ptr<const PointerType>> PointerTypes;

    Map<std::string, ptr<const TupleType>> NamedTupleTypes;

    Map<std::pair<ptr<const Type>, ptr<ValueProducer>>,
        ptr<const TupleType>> UniformTupleTypes;

    Map<TupleTypeKey, ptr<const TupleType>> TupleTypes;

    Map<FunctionTypeKey, ptr<const FunctionType>> FunctionTypes;

    // Constant maps
    class TupleConstantKey {
      ptr<const Type> TheType;
      using ElementVector = Vector<ptr<Constant>>;
      ElementVector ElementValues;

    public:
      TupleConstantKey(ptr<const Type> T,
                       const ElementVector &V) :
        TheType(T), ElementValues(V) {}
      TupleConstantKey(ptr<const Type> T,
                       ElementVector &&V) :
        TheType(T), ElementValues(std::move(V)) {}

      bool operator<(const TupleConstantKey &K) const {
        if (TheType != K.TheType) {
          return TheType < K.TheType;
        }

        if (ElementValues.size() != K.ElementValues.size()) {
          // This really should never happen.
          return ElementValues.size() < K.ElementValues.size();
        }

        auto ThatElem = K.ElementValues.begin();

        for (auto ThisElem : ElementValues) {
          if (ThisElem < *ThatElem) {
            return true;
          }
          else if (*ThatElem > ThisElem) {
            return false;
          }
          ++ThatElem;
        }

        return false;
      }
    };

    Map<std::pair<ptr<const IntegerType>, IntegerConstant::ValueType>,
        ptr<IntegerConstant>> IntegerConstants;

    Map<std::pair<ptr<const FloatingPointType>,
                  FloatingPointConstant::ValueType>,
        ptr<FloatingPointConstant>> FloatingPointConstants;

    Map<TupleConstantKey, ptr<TupleConstant>> TupleConstants;

    // Type constructors
    template<typename Map, typename Structure>
    typename Map::mapped_type
    getType(Map &M,
            const typename Map::key_type &K,
            Structure &S) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;

      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(S);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(K, Result));
      return Result;
    }

    template<typename Map, typename Structure>
    typename Map::mapped_type
    getType(Map &M,
            typename Map::key_type &&K,
            Structure &S) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;

      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(S);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map, typename Structure>
    typename Map::mapped_type
    getType(Map &M,
            const typename Map::key_type &K,
            std::string &&Name) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;
      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(std::move(Name));
      AddToCurrentModule(Result);
      M.insert(std::make_pair(K, Result));
      return Result;
    }

    template<typename Map, typename Structure>
    typename Map::mapped_type
    getType(Map &M,
            typename Map::key_type &&K,
            std::string &&Name) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;
      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(std::move(Name));
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map, typename Arg1, typename Arg2>
    typename Map::mapped_type
    getType(Map &M,
            typename Map::key_type &&K,
            std::string &&Name,
            const Arg1 a1) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;
      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(std::move(Name), a1);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map, typename Arg1, typename Arg2>
    typename Map::mapped_type
    getType(Map &M,
            typename Map::key_type &&K,
            const Arg1 a1,
            const Arg2 a2) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;
      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(a1, a2);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map, typename Arg1, typename Arg2>
    typename Map::mapped_type
    getType(Map &M,
            typename Map::key_type &&K,
            std::string &Name,
            const Arg1 a1,
            const Arg2 a2) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;
      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(Name, a1, a2);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map, typename Arg1, typename Arg2, typename Arg3>
    typename Map::mapped_type
    getType(Map &M,
            typename Map::key_type &&K,
            Arg1 a1,
            Arg2 a2,
            Arg3 a3) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;
      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(a1, a2, a3);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map,
             typename Arg1,
             typename Arg2,
             typename Arg3,
             typename Arg4>
    typename Map::mapped_type
    getType(Map &M,
            typename Map::key_type &&K,
            Arg1 a1,
            Arg2 a2,
            Arg3 a3,
            Arg4 a4) {
      using TheType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;
      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = TheType::Make(a1, a2, a3, a4);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map, typename Structure>
    typename Map::mapped_type
    getType(Map &M,
            Structure &S) {
      return getType(M, S, S);
    }

    template<typename Map, typename Structure>
    typename Map::mapped_type
    getType(Map &M,
            Structure &&S) {
      return getType(M, std::move(S), S);
    }

    template<typename Map>
    typename Map::mapped_type
    getType(Map &M,
            std::string &&Name) {
      return getType(M, std::move(Name));
    }

    ptr<const VoidType> getVoidTypeImpl(void) {
      if (Void) {
        return Void;
      }
      Void = VoidType::Make();
      return Void;
    }

    ptr<const IntegerType> getIntegerTypeImpl(IntegerType::SizeType BitSize);

    ptr<const FloatingPointType>
    getFloatingPointTypeImpl(FloatingPointType::SizeType BitSize);

    ptr<const PointerType> getPointerTypeImpl(ptr<const Type> Base);

    ptr<const PlaceholderType> getPlaceholderTypeImpl(const std::string &Name);

    ptr<const TupleType>
    getTupleTypeImpl(ptr<ValueProducer> Size, ptr<const Type> T);

    ptr<const TupleType>
    getTupleTypeImpl(const std::string &Name,
                     ptr<ValueProducer> Size,
                     ptr<const Type> T);

    template<typename ...Args>
    ptr<const TupleType>
    getTupleTypeImpl(ptr<const Type> T, Args ...args) {
      return getType(TupleTypes,
                     TupleTypeKey(T, args...),
                     getTempName(),
                     T,
                     args...);
    }

    template<typename ...Args>
    ptr<const TupleType>
    getTupleTypeImpl(const std::string &Name,
                     ptr<const Type> T,
                     Args ...args) {
      auto Result = findTupleTypeImpl(Name);
      if (Result) {
        return Result;
      }
      Result = TupleType::Make(Name, T, std::move(args)...);
      AddToCurrentModule(Result);
      NamedTupleTypes.insert(std::make_pair(Name, Result));
      return Result;
    }

    template<typename ...Args>
    ptr<const FunctionType>
    getFunctionTypeImpl(FunctionType::VarargMark V,
                        ptr<const Type> ReturnType,
                        Args ...args) {
      return getType(FunctionTypes,
                     FunctionTypeKey(V, ReturnType, args...),
                     V,
                     ReturnType,
                     args...);
    }
    void ResolveTypesImpl(ptr<const PlaceholderType> P,
                          ptr<const Type> R);

    ptr<const TupleType> findTupleTypeImpl(const std::string &Name);
    ptr<const PlaceholderType> findPlaceholderTypeImpl(const std::string &Name);

    // Constant constructors
    template<typename Map, typename Value>
    typename Map::mapped_type
    getConstant(Map &M,
                const typename Map::key_type &K,
                const Value &V) {
      using ConstantType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;

      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = ConstantType::Make(V);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(K, Result));
      return Result;
    }

    template<typename Map, typename Value>
    typename Map::mapped_type
    getConstant(Map &M,
                typename Map::key_type &&K,
                const Value &V) {
      using ConstantType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;

      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = ConstantType::Make(V);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map, typename T, typename Value>
    typename Map::mapped_type
    getConstant(Map &M,
                const typename Map::key_type &K,
                ptr<const T> Type,
                const Value &V) {
      using ConstantType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;

      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = ConstantType::Make(Type, V);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(K, Result));
      return Result;
    }

    template<typename Map, typename T, typename Value>
    typename Map::mapped_type
    getConstant(Map &M,
                typename Map::key_type &&K,
                ptr<const T> Type,
                const Value &V) {
      using ConstantType = typename std::remove_const<
        typename std::remove_pointer<typename Map::mapped_type>::type>::type;

      auto I = M.find(K);
      if (I != M.end()) {
        return I->second;
      }

      auto Result = ConstantType::Make(Type, V);
      AddToCurrentModule(Result);
      M.insert(std::make_pair(std::move(K), Result));
      return Result;
    }

    template<typename Map, typename T, typename Value>
    typename Map::mapped_type
    getConstant(Map &M,
                ptr<const T> Type,
                const Value &V) {
      return getConstant(M, std::make_pair(Type, V), Type, V);
    }

    ptr<IntegerConstant>
    getIntegerConstantImpl(ptr<const IntegerType> Type,
                           uint64_t Value);

    ptr<FloatingPointConstant>
    getFloatingPointConstantImpl(ptr<const FloatingPointType> Type,
                                 double Value);

    ptr<TupleConstant>
    getTupleConstantImpl(ptr<const TupleType> Type,
                         const std::string &Value);

    // Symbol constructors.
    Map<std::string, ptr<Allocate>> Allocates;

    ptr<GlobalVariable>
    GetOrCreateGlobalVariableImpl(const std::string &Name, ptr<const Type> T,
                                  ptr<Constant> Init);
    ptr<GlobalVariable>
    GetOrCreateGlobalVariableImpl(std::string &&Name, ptr<const Type> T,
                                  ptr<Constant> Init);

    ptr<GlobalVariable>
    FindGlobalVariableImpl(const std::string &Name) {
      return getCurrentModule()->globalVariableFind(Name);
    }

    ptr<Allocate>
    FindAllocateImpl(const std::string &Name) {
      auto I = Allocates.find(Name);
      checkInvariant(I != Allocates.end(), "Could not find Allocate");
      return I->second;
    }

    ptr<Allocate>
    getOrCreateAllocateImpl(const std::string &Name, ptr<const Type> T) {
      auto I = Allocates.find(Name);
      if (I != Allocates.end()) {
        return I->second;
      }

      auto Alloc = Allocate::Make(Name, T);

      Allocates.emplace(Name, Alloc);
      return Alloc;
    }

    ptr<Function>
    GetOrCreateFunctionImpl(const std::string &Name, ptr<const Type> T);
    ptr<Function>
    GetOrCreateFunctionImpl(std::string &&Name, ptr<const Type> T);

    ptr<Function>
    FindFunctionImpl(const std::string &Name) {
      return getCurrentModule()->functionFind(Name);
    }

    unsigned int tempNum;

    unsigned int getNextTempNum(void) {
      return tempNum++;
    }

    IRBuilder(void) : tempNum(0) {}

  public:
    static std::string getTempName(void) {
      return "@" + std::to_string(instance().getNextTempNum());
    }

    // Module management.
    static ptr<Module>
    GetOrCreateModule(const std::string &Name) {
      return instance().GetOrCreateModuleImpl(Name);
    }
    static ptr<Module>
    GetOrCreateModule(std::string &&Name) {
      return instance().GetOrCreateModuleImpl(std::move(Name));
    }
    static ptr<Module>
    CreateNewModule(void) {
      return GetOrCreateModule("Module" +
                               std::to_string(instance().getNextTempNum()));
    }

    static ptr<Module>
    getCurrentModule(void) {
      return instance().getCurrentModuleImpl();
    }

    // Type constructors.
    static ptr<const VoidType>
    getVoidType(void) {
      return instance().getVoidTypeImpl();
    }

    static ptr<const IntegerType>
    getIntegerType(IntegerType::SizeType BitSize) {
      return instance().getIntegerTypeImpl(BitSize);
    }

    static ptr<const FloatingPointType>
    getFloatingPointType(FloatingPointType::SizeType BitSize) {
      return instance().getFloatingPointTypeImpl(BitSize);
    }

    static ptr<const PointerType>
    getPointerType(ptr<const Type> Base) {
      return instance().getPointerTypeImpl(Base);
    }

    static ptr<const PlaceholderType>
    getPlaceholderType(const std::string &Name) {
      return instance().getPlaceholderTypeImpl(Name);
    }

    static ptr<const TupleType>
    getTupleType(ptr<ValueProducer> Size, ptr<const Type> T) {
      return instance().getTupleTypeImpl(Size, T);
    }

    static ptr<const TupleType>
    getTupleType(const std::string &Name,
                 ptr<ValueProducer> Size,
                 ptr<const Type> T) {
      return instance().getTupleTypeImpl(Name, Size, T);
    }

    template<typename ...Args>
    static ptr<const TupleType>
    getTupleType(ptr<const Type> T, Args ...args) {
      return instance().getTupleTypeImpl(T, args...);
    }

    template<typename ...Args>
    static ptr<const TupleType>
    getTupleType(const std::string &Name, ptr<const Type> T, Args ...args) {
      return instance().getTupleTypeImpl(Name, T, args...);
    }

    template<typename ...Args>
    static ptr<const FunctionType>
    getFunctionType(FunctionType::VarargMark V,
                    ptr<const Type> ReturnType,
                    Args ...args) {
      return instance().getFunctionTypeImpl(V, ReturnType, args...);
    }

    static ptr<const TupleType>
    findTupleType(const std::string &Name) {
      return instance().findTupleTypeImpl(Name);
    }

    static ptr<const PlaceholderType>
    findPlaceholderType(const std::string &Name) {
      return instance().findPlaceholderTypeImpl(Name);
    }

    // Constant constructors
    static ptr<IntegerConstant>
    getIntegerConstant(ptr<const IntegerType> Type,
                       uint64_t Value) {
      return instance().getIntegerConstantImpl(Type, Value);
    }

    static ptr<FloatingPointConstant>
    getFloatingPointConstant(ptr<const FloatingPointType> Type,
                             double Value) {
      return instance().getFloatingPointConstantImpl(Type, Value);
    }

    static ptr<TupleConstant>
    getTupleConstant(ptr<const TupleType> Type,
                     const std::string &Value) {
      return instance().getTupleConstantImpl(Type, Value);
    }

    static void ResolveTypes(ptr<const PlaceholderType> P,
                             ptr<const Type> R) {
      instance().ResolveTypesImpl(P, R);
    }

    // Symbol constructors.
    static ptr<GlobalVariable>
    GetOrCreateGlobalVariable(const std::string &Name, ptr<const Type> T,
                              ptr<Constant> Init = nullptr) {
      return instance().GetOrCreateGlobalVariableImpl(Name, T, Init);
    }
    static ptr<GlobalVariable>
    GetOrCreateGlobalVariable(std::string &&Name, ptr<const Type> T,
                              ptr<Constant> Init = nullptr){
      return instance().GetOrCreateGlobalVariableImpl(std::move(Name), T, Init);
    }

    static ptr<GlobalVariable>
    FindGlobalVariable(const std::string &Name) {
      return instance().FindGlobalVariableImpl(Name);
    }

    static ptr<Allocate>
    FindAllocate(const std::string &Name) {
      return instance().FindAllocateImpl(Name);
    }

    static ptr<Allocate>
    getOrCreateAllocate(const std::string &Name, ptr<const Type> T) {
      return instance().getOrCreateAllocateImpl(Name, T);
    }

    static ptr<Function>
    GetOrCreateFunction(const std::string &Name, ptr<const Type> T) {
      return instance().GetOrCreateFunctionImpl(Name, T);
    }
    static ptr<Function>
    GetOrCreateFunction(std::string &&Name, ptr<const Type> T){
      return instance().GetOrCreateFunctionImpl(std::move(Name), T);
    }

    static ptr<Function>
    FindFunction(const std::string &Name) {
      return instance().FindFunctionImpl(Name);
    }

    // General IR constructors.
    template<typename IRType, typename ...Args,
             typename std::enable_if<std::is_base_of<Control, IRType>::value>::type* = nullptr>
    static ptr<IRType>
    get(Args ...args) {
      return IRType::Make(instance().getTempName(), args...);
    }

    template<typename IRType, typename ...Args,
             typename std::enable_if<std::is_base_of<Producer, IRType>::value>::type* = nullptr>
    static ptr<IRType>
    get(Args ...args) {
      return IRType::Make(instance().getTempName(), args...);
    }

    template<typename IRType, typename ...Args,
             typename std::enable_if<std::is_base_of<Control, IRType>::value>::type* = nullptr>
    static ptr<IRType>
    get(const char *Name, Args ...args) {
      return IRType::Make(Name, args...);
    }

    template<typename IRType, typename ...Args,
             typename std::enable_if<std::is_base_of<Producer, IRType>::value>::type* = nullptr>
    static ptr<IRType>
    get(const char *Name, Args ...args) {
      return IRType::Make(Name, args...);
    }

    template<typename IRType, typename ...Args,
             typename std::enable_if<std::is_base_of<Control, IRType>::value>::type* = nullptr>
    static ptr<IRType>
    get(const std::string &Name, Args ...args) {
      return IRType::Make(Name, args...);
    }

    template<typename IRType, typename ...Args,
             typename std::enable_if<std::is_base_of<Producer, IRType>::value>::type* = nullptr>
    static ptr<IRType>
    get(const std::string &Name, Args ...args) {
      return IRType::Make(Name, args...);
    }

    template<typename IRType, typename ...Args,
             typename std::enable_if<std::is_base_of<Control, IRType>::value>::type* = nullptr>
    static ptr<IRType>
    get(std::string &&Name, Args ...args) {
      return IRType::Make(std::move(Name), args...);
    }

    template<typename IRType, typename ...Args,
             typename std::enable_if<std::is_base_of<Producer, IRType>::value>::type* = nullptr>
    static ptr<IRType>
    get(std::string &&Name, Args ...args) {
      return IRType::Make(std::move(Name), args...);
    }
  };
}

// Order is important here.  In particular, Domain.hpp must be included
// after Grammar.hpp.
//#include <mirv/Core/Builder/SymbolTerminals.hpp>
//#include <mirv/Core/Builder/ExpressionRules.hpp>
//#include <mirv/Core/Builder/Grammar.hpp>
//#include <mirv/Core/Builder/Wrapper.hpp>
//#include <mirv/Core/Builder/Domain.hpp>
//#include <mirv/Core/Builder/Make.hpp>
//#include <mirv/Core/Builder/Translate.hpp>

#endif
