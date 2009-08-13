ifndef CXX_MK
CXX_MK := 1

$(call debug,cxx.mk)

include $(BUILDTOOLS)/configure/tool.mk
include $(BUILDTOOLS)/configure/configure.mk
include $(BUILDTOOLS)/configure/executable.mk
include $(BUILDTOOLS)/configure/execute.mk

ifeq ($(CXX),)
  __sink := $(call mc_target_tool,CXX,g++ c++,Could not find C++ compiler)
else
  ifeq ($(origin CXX),default)
    $(call debug,Origin CXX)
    override CXX :=
    __sink := $(call mc_target_tool,CXX,g++ c++,Could not find C++ compiler)
  endif
endif

# See if the C compiler works
override CXX_COMPILE_SOURCE := int main(void) { return(0); }
override CXX_COMPILE_ERROR = $(call mc_error,C++ compiler cannot create executables)
override CXX_COMPILE_COMPILE_MK = $(FINAL_BUILDDIR)/configure/CXXCXX.mk
override CXX_COMPILE_LINK_MK = $(FINAL_BUILDDIR)/configure/CXXLD.mk
override CXX_COMPILE_COMPILE_TRUE = $(call mc_define,CONFIG_CXX_COMPILE,yes,$@)
override CXX_COMPILE_LINK_TRUE = $(call mc_define,CONFIG_CXX_LINK,yes,$@)
__sink := $(call mc_try_executable_cxx,CXX_COMPILE_SOURCE,CXX_COMPILE_COMPILE_TRUE,CXX_COMPILE_ERROR,CXX_COMPILE_LINK_TRUE,CXX_COMPILE_ERROR,$(CXX_COMPILE_COMPILE_MK),$(CXX_COMPILE_LINK_MK))
#__sink := $(call mc_try_compile_cxx,CXX_COMPILE_SOURCE,CXX_COMPILE_COMPILE_TRUE,CXX_COMPILE_ERROR,$(CXX_COMPILE_COMPILE_MK))

# See if this is g++
CXX_EXECUTE_RUN = $(CXX) --version | grep "g++ (GCC)"
CXX_EXECUTE_TRUE = $(call mc_info,Using g++)$(call mc_define,CONFIG_HAVE_GXX,yes,$@)
CXX_EXECUTE_FALSE = $(call mc_define,CONFIG_HAVE_GXX,,$@)
__sink := $(call mc_try_execute,CXX_EXECUTE,CXX_EXECUTE_RUN,CXX_EXECUTE_TRUE,CXX_EXECUTE_FALSE,$(FINAL_BUILDDIR)/configure/GXX_TRY.mk,$(FINAL_BUILDDIR)/configure/CXX.mk)

#CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/GXX_TRY.mk

endif
