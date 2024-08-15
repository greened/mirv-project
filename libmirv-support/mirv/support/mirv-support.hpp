#pragma once

#include <iosfwd>
#include <string>

#include <mirv/support/export.hpp>

namespace mirv_support
{
  // Print a greeting for the specified name into the specified
  // stream. Throw std::invalid_argument if the name is empty.
  //
  MIRV_SUPPORT_SYMEXPORT void
  say_hello (std::ostream&, const std::string& name);
}
