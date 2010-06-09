#ifndef mirv_Core_Builder_LoopTerminals_hpp
#define mirv_Core_Builder_LoopTerminals_hpp

#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/Builder/Keywords.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// Define a terminal type for the while keyword.
     typedef Wrapper<boost::proto::terminal<keyword::while_>::type> WhileTerminal;
     /// Define a terminal type for the do keyword.
     typedef Wrapper<boost::proto::terminal<keyword::do_>::type> DoTerminal;

     namespace {
       /// A while_ "operator."  This is a protoized object that
       /// implements the subscript operator to kick off dowhile
       /// statement generation.
       const WhileTerminal while_ = {{}};

       /// A do_ "operator."  This is a protoized object that
       /// implements the subscript operator to kick off dowhile
       /// statement generation.
       const DoTerminal do_ = {{}};
     }
   }
}

#endif
