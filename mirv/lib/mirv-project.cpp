#include <string>

#include "mirv-project/mirv-project.hpp"

exported_class::exported_class()
    : m_name {"mirv-project"}
{
}

auto exported_class::name() const -> char const*
{
  return m_name.c_str();
}
