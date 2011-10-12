#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/AddressConstant.hpp>
#include <mirv/Core/Builder/AssignGrammar.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      /// Given a value, construct a statement to store to it.
      ptr<Statement<Base> >::type
      ConstructStore::operator()(boost::shared_ptr<SymbolTable> symtab,
                                 ptr<Expression<Base> >::type address,
                                 ptr<Expression<Base> >::type value) {
        // The address actually comes across as a value, so extract
        // it.
        ptr<Expression<Base> >::type newAddress =
          detail::extractStoreAddress(symtab, address);
        if (!newAddress) {
          std::cerr << "Offending expression:\n";
          printWithTypes(std::cerr, address);
        }

        checkInvariant(newAddress, "Invalid store address");
        return ConstructBinary<Statement<Store> >()(symtab, newAddress, value);
      }
    }
  }
}
