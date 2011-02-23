#ifndef mirv_Core_Builder_ConstantRules_hpp
#define mirv_Core_Builder_ConstantRules_hpp

#include <cstdint>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     struct Int8ConstantTerminal 
         : public boost::proto::terminal<std::int8_t>::type {};
     struct UInt8ConstantTerminal 
         : public boost::proto::terminal<std::uint8_t>::type {};
     struct Int16ConstantTerminal 
         : public boost::proto::terminal<std::int16_t>::type {};
     struct UInt16ConstantTerminal 
         : public boost::proto::terminal<std::uint16_t>::type {};
     struct Int32ConstantTerminal 
         : public boost::proto::terminal<std::int32_t>::type {};
     struct UInt32ConstantTerminal 
         : public boost::proto::terminal<std::uint32_t>::type {};
     struct Int64ConstantTerminal 
         : public boost::proto::terminal<std::int64_t>::type {};
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

     struct FloatConstantTerminal 
         : public boost::proto::terminal<float>::type {};
     struct DoubleConstantTerminal 
         : public boost::proto::terminal<double>::type {};

     /// This is the rule to match floating constant symbols.  It
     /// matches float_constant.
     struct FloatingConstantRule : boost::proto::or_<
       FloatConstantTerminal,
       DoubleConstantTerminal
       > {};
   }
}

#endif
