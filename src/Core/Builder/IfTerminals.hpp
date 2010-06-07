#ifndef mirv_Core_Builder_IfTerminals_hpp
#define mirv_Core_Builder_IfTerminals_hpp

#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/Builder/Keywords.hpp>
#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// Define a terminal type for if-then and if-else statements.
     typedef Wrapper<boost::proto::terminal<keyword::if_>::type> IfTerminal;

     /// Define a terminal type for the else portion of if-else
     /// statements.
     typedef Wrapper<boost::proto::terminal<keyword::else_>::type> ElseTerminal;

     namespace {
       /// An if_ "operator."  This is a protoized object that implements
       /// the subscript operator to kick off if statement generation.
       const IfTerminal if_ = {{}};
     }
   }
}

#endif
