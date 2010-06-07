#ifndef mirv_Core_Builder_Symbol_Terminals_hpp
#define mirv_Core_Builder_Symbol_Terminals_hpp

#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/DomainFwd.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <mirv/Core/Builder/Keywords.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// A terminal for string constants.
     typedef Wrapper<boost::proto::terminal<boost::proto::convertible_to<std::string> >::type> StringTerminal;
     /// A terminal for integer constants.
     typedef Wrapper<boost::proto::terminal<int>::type> IntegerTerminal;

     /// Define a terminal type for type symbols.
     typedef Wrapper<boost::proto::terminal<keyword::type>::type> TypeTerminal;
     /// Define a terminal type for variable symbols.
     typedef Wrapper<boost::proto::terminal<keyword::var>::type> VarTerminal;
     /// Define a terminal type for module symbols.
     typedef Wrapper<boost::proto::terminal<keyword::module>::type> ModuleTerminal;
     /// Define a terminal type for function symbols.
     typedef Wrapper<boost::proto::terminal<keyword::function>::type> FunctionTerminal;
   }
}

#endif