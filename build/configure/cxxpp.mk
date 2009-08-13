ifndef CXXPP_MK
CXXPP_MK := 1

$(call debug,cxxpp.mk)

include $(BUILDTOOLS)/configure/cxx.mk
include $(BUILDTOOLS)/configure/target.mk
include $(BUILDTOOLS)/configure/findfile.mk
include $(BUILDTOOLS)/configure/preprocess.mk
include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk

CXXPP_SOURCE = int main(void) { return(0); }
CXXPP_TRUE = $(call mc_define,CXXPP,$(CXX) -E,$@)
CXXPP_FALSE = $(call mc_error,Cannot run $(CXX) -E)

ifeq ($(origin CXXPP),default)
  override CXXPP :=
endif
#__sink := $(call mc_try_preprocess_cxx,CXXPP_SOURCE,CXXPP_TRUE,CXXPP_FALSE,$(FINAL_BUILDDIR)/configure/CXXPP.mk)

endif
