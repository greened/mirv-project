#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Library/Range.hpp>

#include <boost/range/algorithm.hpp>
#include <boost/range/join.hpp>

namespace mirv {
  ptr<Module>
  IRBuilder::GetOrCreateModuleImpl(const std::string &Name) {
    auto I = boost::find_if(Modules, [&Name] (ptr<Module> M) {
        return M->getName() == Name;
      });

    if (I != Modules.end()) {
      CurrentModule = *I;
      return getCurrentModuleImpl();
    }

    CurrentModule = Module::Make(Name);
    Modules.emplace_back(CurrentModule);
    return getCurrentModule();
  }

  ptr<Module>
  IRBuilder::GetOrCreateModuleImpl(std::string &&Name) {
    auto I = boost::find_if(Modules, [&Name] (ptr<Module> M) {
        return M->getName() == Name;
      });

    if (I != Modules.end()) {
      CurrentModule = *I;
      return getCurrentModuleImpl();
    }

    CurrentModule = Module::Make(std::move(Name));
    Modules.emplace_back(CurrentModule);
    return getCurrentModule();
  }

  ptr<const IntegerType>
  IRBuilder::getIntegerTypeImpl(IntegerType::SizeType BitSize) {
    return getType(IntegerTypes, BitSize);
  }

  ptr<const FloatingPointType>
  IRBuilder::getFloatingPointTypeImpl(FloatingPointType::SizeType BitSize) {
    return getType(FloatingPointTypes, BitSize);
  }

  ptr<const PointerType>
  IRBuilder::getPointerTypeImpl(ptr<const Type> Base) {
    return getType(PointerTypes, Base);
  }

  ptr<const PlaceholderType>
  IRBuilder::getPlaceholderTypeImpl(const std::string &Name) {
    return getType(PlaceholderTypes, Name);
  }

  ptr<const TupleType>
  IRBuilder::getTupleTypeImpl(ptr<ValueProducer> Size, ptr<const Type> T) {
    return getType(UniformTupleTypes,
                   std::make_pair(T, Size),
                   getTempName(),
                   Size,
                   T);
  }

  ptr<const TupleType>
  IRBuilder::getTupleTypeImpl(const std::string &Name,
                              ptr<ValueProducer> Size,
                              ptr<const Type> T) {
    return getType(UniformTupleTypes,
                   std::make_pair(T, Size),
                   Name,
                   Size,
                   T);
  }

  void IRBuilder::ResolveTypesImpl(ptr<const PlaceholderType> P,
                                   ptr<const Type> R) {
    for (auto T : PointerTypes | MapValues) {
      std::const_pointer_cast<Type>(T)->resolve(P, R);
    }
  }

  ptr<const TupleType>
  IRBuilder::findTupleTypeImpl(const std::string &Name) {
    auto I = NamedTupleTypes.find(Name);
    if (I != NamedTupleTypes.end()) {
      return I->second;
    }

    return ptr<const TupleType>();
  }

  ptr<const PlaceholderType>
  IRBuilder::findPlaceholderTypeImpl(const std::string &Name) {
    auto I = PlaceholderTypes.find(Name);
    if (I != PlaceholderTypes.end()) {
      return I->second;
    }

    return ptr<const PlaceholderType>();
  }

  void IRBuilder::AddToCurrentModule(ptr<const Type> T) {
    getCurrentModule()->AddType(T);
  }

  ptr<IntegerConstant>
  IRBuilder::getIntegerConstantImpl(ptr<const IntegerType> Type,
                                    uint64_t Value) {
    return getConstant(IntegerConstants, Type, Value);
  }

  ptr<FloatingPointConstant>
  IRBuilder::getFloatingPointConstantImpl(ptr<const FloatingPointType> Type,
                                          double Value) {
    return getConstant(FloatingPointConstants, Type, Value);
  }

  ptr<TupleConstant>
  IRBuilder::getTupleConstantImpl(ptr<const TupleType> Type,
                                  const std::string &Value) {
    static char Null[] = { '\0' };

    Vector<ptr<Constant>> Elements;
    boost::copy(boost::join(MakeRange(Value.begin(), Value.end()),
                            MakeRange(std::begin(Null),
                                      std::end(Null))) |
                boost::adaptors::transformed([](char C) {
                    return IRBuilder::
                      getIntegerConstant(IRBuilder::getIntegerType(8),
                                         C);
                  }),
                std::back_inserter(Elements));

    return getConstant(TupleConstants, TupleConstantKey(Type, Elements),
                       Type, MakeRange(Elements.begin(), Elements.end()));
  }

  void IRBuilder::AddToCurrentModule(ptr<Constant> C) {
    getCurrentModule()->AddConstant(C);
  }

  ptr<GlobalVariable>
  IRBuilder::GetOrCreateGlobalVariableImpl(const std::string &Name,
                                           ptr<const Type> T,
                                           ptr<Constant> Init) {
    ptr<GlobalVariable> Result = getCurrentModule()->globalVariableFind(Name);
    if (Result) {
      return Result;
    }

    Result = GlobalVariable::Make(Name, T, Init);
    AddToCurrentModule(Result);
    return Result;
  }

  ptr<GlobalVariable>
  IRBuilder::GetOrCreateGlobalVariableImpl(std::string &&Name,
                                           ptr<const Type> T,
                                           ptr<Constant> Init) {
    ptr<GlobalVariable> Result = getCurrentModule()->globalVariableFind(Name);
    if (Result) {
      return Result;
    }

    Result = GlobalVariable::Make(std::move(Name), T, Init);
    AddToCurrentModule(Result);
    return Result;
  }

  ptr<Function>
  IRBuilder::GetOrCreateFunctionImpl(const std::string &Name,
                                     ptr<const Type> T) {
    ptr<Function> Result = getCurrentModule()->functionFind(Name);
    if (Result) {
      return Result;
    }

    Result = Function::Make(Name, T);
    AddToCurrentModule(Result);
    return Result;
  }

  ptr<Function>
  IRBuilder::GetOrCreateFunctionImpl(std::string &&Name,
                                     ptr<const Type> T) {
    ptr<Function> Result = getCurrentModule()->functionFind(Name);
    if (Result) {
      return Result;
    }

    Result = Function::Make(std::move(Name), T);
    AddToCurrentModule(Result);
    return Result;
  }

  void IRBuilder::AddToCurrentModule(ptr<GlobalVariable> G) {
    getCurrentModule()->AddGlobalVariable(G);
  }

  void IRBuilder::AddToCurrentModule(ptr<Function> F) {
    getCurrentModule()->AddFunction(F);
  }
}
