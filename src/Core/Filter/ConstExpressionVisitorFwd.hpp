#ifndef mirv_Core_Filter_ConstExpressionVisitorFwd_hpp
#define mirv_Core_Filter_ConstExpressionVisitorFwd_hpp

namespace mirv {
  /// This is the base class for all visitors that know about
  /// expressions.  This includes flows, visitor actions and anything
  /// else that wants to examine or operate on expressions.
  struct ConstExpressionVisitor;
}

#endif