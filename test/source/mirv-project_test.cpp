#include <string>

#include "mirv-project/mirv-project.hpp"

auto main() -> int
{
  auto const exported = exported_class {};

  return std::string("mirv-project") == exported.name() ? 0 : 1;
}
