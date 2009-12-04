ifndef GREP_MK
GREP_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(GREP),)
  __sink := $(call mc_target_tool,GREP,grep)
else
  ifeq ($(origin GREP),default)
    override GREP :=
    __sink := $(call debug,Before call mc_target_tools) $(call mc_target_tool,GREP,grep) $(call debug,After call mc_target_tools)
  endif
endif

endif
