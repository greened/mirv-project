ifndef BOOST_MK
BOOST_MK := 1

$(call debug,[boost] boost.mk)

include $(BUILDTOOLS)/configure/library.mk

# See if the Boost libraries are available.

define BOOST_SOURCE
#include <boost/shared_ptr.hpp>

int main(void)
{
  boost::shared_ptr<int> ptr(new int);
  return 0;
}
endef

override BOOST_LIBRARY =
override BOOST_LINK_FLAGS =
override BOOST_COMPILE_ERROR = $(call mc_error,Cannot find Boost libraries)
override BOOST_LINK_ERROR = $(call mc_error,Cannot link to Boost libraries)
override BOOST_COMPILE_TRUE = $(call mc_define_append_nomsg,CONFIG_BOOST_COMPILE,yes,$@); $(call mc_info_append,C++ compiler can find Boost headers,$@)
override BOOST_LINK_TRUE = $(call mc_define_append_nomsg,CONFIG_BOOST_LINK,yes,$@); $(call mc_info_append,C++ linker can link to Boost libraries,$@)
override BOOST_DEP_MK = $(FINAL_BUILDDIR)/configure/CXX.mk

$(call mc_try_library_cxx,Boost,BOOST_SOURCE,BOOST_LIBRARY,-fpermissive -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS,$(BOOST_LINK_FLAGS),BOOST_COMPILE_TRUE,BOOST_COMPILE_ERROR,BOOST_LINK_TRUE,BOOST_LINK_ERROR,$(BOOST_DEP_MK))

endif
