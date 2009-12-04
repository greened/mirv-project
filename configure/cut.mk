ifndef CUT_MK
CUT_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(CUT),)
  __sink := $(call mc_target_tool,CUT,cut)
else
  ifeq ($(origin CUT),default)
    override CUT :=
    __sink := $(call debug,Before call mc_target_tools) $(call mc_target_tool,CUT,cut) $(call debug,After call mc_target_tools)
  endif
endif

endif
