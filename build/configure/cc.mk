ifndef CC_MK
CC_MK := 1

$(call debug,cc.mk)

include $(BUILDTOOLS)/configure/configure.mk
include $(BUILDTOOLS)/configure/tool.mk
include $(BUILDTOOLS)/configure/executable.mk
include $(BUILDTOOLS)/configure/execute.mk

$(call mc_target_tool,CC,gcc cc)

#ifeq ($(CC),)
#  $(call mc_target_tool,CC,gcc cc)
#else
#  ifeq ($(origin CC),default)
#    $(call debug,Origin CC)
#    override CC :=
#    $(call mc_target_tool,CC,gcc cc)
#  endif
#endif

# See if the C compiler works
override C_COMPILE_SOURCE := int main(void) { return(0); }
override C_COMPILE_ERROR = $(call mc_error,C compiler cannot create objects)
override C_LINK_ERROR = $(call mc_error,C compiler cannot create executables)
override C_COMPILE_COMPILE_TRUE = $(call mc_define_nomsg,CONFIG_CC_COMPILE,yes,$@)
override C_COMPILE_LINK_TRUE = $(call mc_define_nomsg,CONFIG_CC_LINK,yes,$@)
override C_COMPILE_DEP_MK = $(FINAL_BUILDDIR)/configure/CC.mk

$(call mc_try_executable_c,CC_executable,C_COMPILE_SOURCE,,,C_COMPILE_COMPILE_TRUE,C_COMPILE_ERROR,C_COMPILE_LINK_TRUE,C_LINK_ERROR,$(C_COMPILE_DEP_MK))

# See if this is gcc
C_EXECUTE_RUN = $(CC) --version | grep "gcc (GCC)"
C_EXECUTE_TRUE = $(call mc_info,Using gcc)$(call mc_define_nomsg,CONFIG_HAVE_GCC,yes,$@)
C_EXECUTE_FALSE = $(call mc_define_nomsg,CONFIG_HAVE_GCC,,$@)
C_EXECUTE_DEP_MK = $(FINAL_BUILDDIR)/configure/CC.mk

$(call mc_try_execute,CC_EXECUTE,C_EXECUTE_RUN,C_EXECUTE_TRUE,C_EXECUTE_FALSE,$(C_EXECUTE_DEP_MK))

#CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/GCC_TRY.mk

ifeq ($(CONFIG_HAVE_GCC),yes)
  include $(BUILDTOOLS)/configure/gcc.mk
endif

# See if this is clang
C_EXECUTE_RUN = $(CC) --version | grep "clang"
C_EXECUTE_TRUE = $(call mc_info,Using clang)$(call mc_define_nomsg,CONFIG_HAVE_CLANG,yes,$@)
C_EXECUTE_FALSE = $(call mc_define_nomsg,CONFIG_HAVE_CLANG,,$@)
C_EXECUTE_DEP_MK = $(FINAL_BUILDDIR)/configure/CC.mk

$(call mc_try_execute,CC_EXECUTE,C_EXECUTE_RUN,C_EXECUTE_TRUE,C_EXECUTE_FALSE,$(C_EXECUTE_DEP_MK))

ifeq ($(CONFIG_HAVE_CLANG),yes)
  include $(BUILDTOOLS)/configure/clang.mk
endif

endif
