ifndef CXX_MK
CXX_MK := 1

$(call debug,cxx.mk)

include $(BUILDTOOLS)/configure/tool.mk
include $(BUILDTOOLS)/configure/configure.mk
include $(BUILDTOOLS)/configure/executable.mk
include $(BUILDTOOLS)/configure/execute.mk

$(call mc_target_tool,CXX,g++ c++,Could not find C++ compiler)

#ifeq ($(CXX),)
#  $(call mc_target_tool,CXX,g++ c++,Could not find C++ compiler)
#else
#  ifeq ($(origin CXX),default)
#    $(call debug,Origin CXX)
#    override CXX :=
#    $(call mc_target_tool,CXX,g++ c++,Could not find C++ compiler)
#  endif
#endif

# See if the C++ compiler works
override CXX_COMPILE_SOURCE := int main(void) { return(0); }
override CXX_COMPILE_ERROR = $(call mc_error,C++ compiler cannot create objects)
override CXX_LINK_ERROR = $(call mc_error,C++ linker cannot create executables)
override CXX_COMPILE_TRUE = $(call mc_define_append_nomsg,CONFIG_CXX_COMPILE,yes,$@); $(call mc_info_append,C++ compiler can create objects,$@)
override CXX_LINK_TRUE = $(call mc_define_append_nomsg,CONFIG_CXX_LINK,yes,$@); $(call mc_info_append,C++ linker can link executables,$@)
override CXX_COMPILE_DEP_MK = $(FINAL_BUILDDIR)/configure/CXX.mk

$(call mc_try_executable_cxx,CXX,CXX_COMPILE_SOURCE,,,CXX_COMPILE_TRUE,CXX_COMPILE_ERROR,CXX_LINK_TRUE,CXX_LINK_ERROR,$(CXX_COMPILE_DEP_MK))

# See if this is g++
CXX_EXECUTE_RUN = $(CXX) --version | grep "g++"
CXX_EXECUTE_TRUE = $(call mc_define_append_nomsg,CONFIG_HAVE_GXX,yes,$@); $(call mc_info_append,Using g++,$@)
CXX_EXECUTE_FALSE = $(call mc_define_append_nomsg,CONFIG_HAVE_GXX,,$@)
CXX_EXECUTE_DEP_MK = $(FINAL_BUILDDIR)/configure/CXX.mk

$(call mc_try_execute,GXX_EXECUTE,CXX_EXECUTE_RUN,CXX_EXECUTE_TRUE,CXX_EXECUTE_FALSE,$(CXX_EXECUTE_DEP_MK))

#CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/GXX_TRY.mk

endif
