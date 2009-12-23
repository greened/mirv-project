ifndef TEE_MK
TEE_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(TEE),)
  __sink := $(call mc_target_tool,TEE,tee)
else
  ifeq ($(origin TEE),default)
    override TEE :=
    __sink := $(call debug,Before call mc_target_tools) $(call mc_target_tool,TEE,tee) $(call debug,After call mc_target_tools)
  endif
endif

endif
