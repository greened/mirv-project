#ifndef mirv_Core_Builder_ConstantRules_hpp
#define mirv_Core_Builder_ConstantRules_hpp

#include <cstdint>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// Define the terminal symbol for signed 8 bit integer
     /// constants.
     struct Int8ConstantTerminal 
         : public boost::proto::terminal<std::int8_t>::type {};
     /// A terminal symbol for unsigned 8 bit integer constants.
     struct UInt8ConstantTerminal 
         : public boost::proto::terminal<std::uint8_t>::type {};
     /// A terminal symbol for signed 16 bit integer constants.
     struct Int16ConstantTerminal 
         : public boost::proto::terminal<std::int16_t>::type {};
     /// A terminal symbol for unsigned 16 bit integer constants.
     struct UInt16ConstantTerminal 
         : public boost::proto::terminal<std::uint16_t>::type {};
     /// A terminal symbol for signed 32 bit integer constants.
     struct Int32ConstantTerminal 
         : public boost::proto::terminal<std::int32_t>::type {};
     /// A terminal symbol for unsigned 32 bit integers.
     struct UInt32ConstantTerminal 
         : public boost::proto::terminal<std::uint32_t>::type {};
     /// A terminal symbol for signed 64 bit integers.
     struct Int64ConstantTerminal
         : public boost::proto::terminal<std::int64_t>::type {};
     /// A terminal symbol for unsigned 64 bit integers.
     struct UInt64ConstantTerminal 
         : public boost::proto::terminal<std::uint64_t>::type {};

     /// This is the rule to match integral constant symbols.  It
     /// matches int_constant.
     struct IntegralConstantRule : boost::proto::or_<
       Int64ConstantTerminal,
       UInt64ConstantTerminal,
       Int32ConstantTerminal,
       UInt32ConstantTerminal,
       Int16ConstantTerminal,
       UInt16ConstantTerminal,
       Int8ConstantTerminal,
       UInt8ConstantTerminal
       > {};

     /// Define a terminal for float constants.
     struct FloatConstantTerminal 
         : public boost::proto::terminal<float>::type {};
     /// Define a terminal for double constants.
     struct DoubleConstantTerminal 
         : public boost::proto::terminal<double>::type {};

     /// This is the rule to match floating constant symbols.  It
     /// matches float_constant.
     struct FloatingConstantRule : boost::proto::or_<
       FloatConstantTerminal,
       DoubleConstantTerminal
       > {};

     struct StringConstantTerminal 
         : public boost::proto::terminal<
           boost::proto::convertible_to<std::string>
           >::type {};

     /// This is the rule to match string constant symbols.  Strings
     /// are really arrays of int8.  As a special case, we return the
     /// address of the first array element to maintain a C-like
     /// syntax.
     struct StringConstantRule : StringConstantTerminal {};
   }
}

#endif
