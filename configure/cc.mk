ifndef CC_MK
CC_MK := 1

$(call debug,cc.mk)

include $(BUILDTOOLS)/configure/configure.mk
include $(BUILDTOOLS)/configure/tool.mk
include $(BUILDTOOLS)/configure/executable.mk
include $(BUILDTOOLS)/configure/execute.mk

ifeq ($(CC),)
  __sink := $(call mc_target_tool,CC,gcc cc)
else
  ifeq ($(origin CC),default)
    $(call debug,Origin CC)
    override CC :=
    __sink := $(call mc_target_tool,CC,gcc cc)
  endif
endif

# See if the C compiler works
override C_COMPILE_SOURCE := int main(void) { return(0); }
override C_COMPILE_ERROR = $(call mc_error,C compiler cannot create executables)
override C_COMPILE_COMPILE_MK = $(FINAL_BUILDDIR)/configure/CCCC.mk
override C_COMPILE_LINK_MK = $(FINAL_BUILDDIR)/configure/CCLD.mk
override C_COMPILE_COMPILE_TRUE = $(call mc_define,CONFIG_CC_COMPILE,yes,$@)
override C_COMPILE_LINK_TRUE = $(call mc_define,CONFIG_CC_LINK,yes,$@)
__sink := $(call mc_try_executable_c,C_COMPILE_SOURCE,C_COMPILE_COMPILE_TRUE,C_COMPILE_ERROR,C_COMPILE_LINK_TRUE,C_COMPILE_ERROR,$(C_COMPILE_COMPILE_MK),$(C_COMPILE_LINK_MK))

# See if this is gcc
C_EXECUTE_RUN = $(CC) --version | grep "gcc (GCC)"
C_EXECUTE_TRUE = $(call mc_info,Using gcc)$(call mc_define,CONFIG_HAVE_GCC,yes,$@)
C_EXECUTE_FALSE = $(call mc_define,CONFIG_HAVE_GCC,,$@)
__sink := $(call mc_try_execute,CC_EXECUTE,C_EXECUTE_RUN,C_EXECUTE_TRUE,C_EXECUTE_FALSE,$(FINAL_BUILDDIR)/configure/GCC_TRY.mk)

#CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/GCC_TRY.mk

ifeq ($(CONFIG_HAVE_GCC),yes)
  include $(BUILDTOOLS)/configure/gcc.mk
endif

endif
